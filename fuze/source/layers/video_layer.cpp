#include "fuze.h"


titian::VideoLayer::VideoLayer()
	: Layer("VideoLayer")
{}

void titian::VideoLayer::init()
{
	kl::Window* window = &Layers::get<AppLayer>()->window;
	window->set_icon("package/textures/video_editor_icon.ico");

	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

	auto load_shader = [&](dx::ComputeShader& shader, const char* filename)
	{
		const String source = kl::read_file_string(kl::format("package/shaders/", filename));
		shader = gpu->create_compute_shader(source);
		kl::assert(shader, "Failed to init [", filename, "] shaders");
	};

	WorkQueue queue;
	queue.add_task([&] { load_shader(clear_shader, "video_clear_shader.hlsl"); });
	queue.add_task([&] { load_shader(mix_shader, "video_mix_shader.hlsl"); });
	queue.finalize();
}

bool titian::VideoLayer::update()
{
	AppLayer* app_layer = Layers::get<AppLayer>();
	kl::GPU* gpu = &app_layer->gpu;
	kl::Timer* timer = &app_layer->timer;

	if (m_playing) {
		current_time = m_last_time + timer->elapsed();
	}
	gpu->clear_internal(kl::colors::BLACK);
	return true;
}

void titian::VideoLayer::play()
{
	m_playing = true;
	m_last_time = current_time;
	play_audio();
}

void titian::VideoLayer::stop()
{
	m_playing = false;
	current_time = m_last_time;
}

bool titian::VideoLayer::playing() const
{
	return m_playing;
}

float titian::VideoLayer::start_time() const
{
	if (tracks.empty())
		return 0.0f;

	float result = std::numeric_limits<float>::infinity();
	for (auto& track : tracks) {
		for (auto& [offset, _] : track->media) {
			result = std::min(result, offset);
		}
	}
	return result;
}

float titian::VideoLayer::end_time() const
{
	float result = 0.0f;
	for (auto& track : tracks) {
		for (auto& [offset, media] : track->media) {
			result = std::max(result, offset + media->duration);
		}
	}
	return result;
}

void titian::VideoLayer::get_frame(Frame& out) const
{
	for (int i = int(tracks.size()) - 1; i >= 0; i--) {
		auto& track = tracks[i];
		float offset = 0.0f;
		if (Ref media = track->get_media(current_time, offset)) {
			if (media->get_frame(current_time - offset)) {
				mix_frame(out, media->out_frame);
			}
		}
	}
}

void titian::VideoLayer::load_image(const String& path)
{
	if (tracks.empty()) {
		tracks.emplace_back(new Track());
	}

	Ref media = new Media();
	media->image = new Image(path);
	media->duration = 5.0f;
	media->type = MediaType::IMAGE;
	media->name = fs::path(path).filename().string();
	tracks.front()->insert_media(current_time, media);
}

void titian::VideoLayer::load_audio(const String& path)
{
	if (tracks.empty()) {
		tracks.emplace_back(new Track());
	}

	Ref media = new Media();
	media->audio = new Audio(path);
	media->duration = media->audio->duration_seconds();
	media->type = MediaType::AUDIO;
	media->name = fs::path(path).filename().string();
	tracks.front()->insert_media(current_time, media);
}

void titian::VideoLayer::load_video(const String& path)
{
	if (tracks.empty()) {
		tracks.emplace_back(new Track());
	}

	Ref media = new Media();
	media->audio = new Audio(path);
	media->video = new Video(path);
	media->duration = media->video->duration_seconds();
	media->type = MediaType::VIDEO;
	media->name = fs::path(path).filename().string();

	media->video->cache_frames(0.0f, 30.0f);
	tracks.front()->insert_media(current_time, media);
}

void titian::VideoLayer::mix_frame(Frame& first, const Frame& second) const
{
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
	gpu->bind_access_view_for_compute_shader(first.access_view, 0);
	gpu->bind_access_view_for_compute_shader(second.access_view, 1);
	const Int2 dispatch_size = kl::min(first.size(), second.size()) / 32 + Int2(1);
	gpu->execute_compute_shader(mix_shader, dispatch_size.x, dispatch_size.y, 1);
	gpu->unbind_access_view_for_compute_shader(1);
	gpu->unbind_access_view_for_compute_shader(0);
}

void titian::VideoLayer::play_audio()
{
	prepare_audio();

	if (!m_audio.empty()) {
		const int index = std::clamp(m_audio.sample_index(current_time), 0, (int) m_audio.size() - 1);
		m_audio.erase(m_audio.begin(), m_audio.begin() + index);
	}

	m_audio_worker = {};
	m_audio_worker = std::async(std::launch::async, [&]
	{
		m_audio_device.play_audio(m_audio, [this] { return this->m_playing; });
	});
}

void titian::VideoLayer::prepare_audio()
{
	m_audio.set_duration(end_time());
	kl::async_for(0, (int) m_audio.size(), [&](const int i)
	{
		m_audio[i] = 0.0f;
	});

	for (auto& track : tracks) {
		for (auto& [offset, media] : track->media) {
			media->get_audio();
			kl::async_for(0, (int) m_audio.size(), [&](const int i)
			{
				const float time = m_audio.sample_time(i) - offset;
				const int sample_index = media->out_audio.sample_index(time);
				if (sample_index >= 0 && sample_index < media->out_audio.size()) {
					m_audio[i] += media->out_audio[sample_index];
				}
			});
		}
	}
}
