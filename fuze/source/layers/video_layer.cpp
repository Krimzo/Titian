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
	queue.add_task([&] { load_shader(m_clear_shader, "video_clear_shader.hlsl"); });
	queue.add_task([&] { load_shader(m_mix_shader, "video_mix_shader.hlsl"); });
	queue.finalize();

	for (int i = 0; i < 5; i++)
		tracks.emplace_back(new Track());
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
			result = kl::min(result, offset);
		}
	}
	return result;
}

float titian::VideoLayer::end_time() const
{
	float result = 0.0f;
	for (auto& track : tracks) {
		for (auto& [offset, media] : track->media) {
			result = kl::max(result, offset + media->duration);
		}
	}
	return result;
}

void titian::VideoLayer::store_frame()
{
	out_frame.resize(viewport_size);
	this->clear_frame();
	for (int i = int(tracks.size()) - 1; i >= 0; i--) {
		float offset = 0.0f;
		if (Ref media = tracks[i]->get_media(current_time, offset)) {
			media->store_frame(current_time - offset);
			mix_frame(media->out_frame);
		}
	}
}

void titian::VideoLayer::load_image(const String& path)
{
	if (!selected_track) {
		if (tracks.empty()) {
			tracks.emplace_back(new Track());
		}
		selected_track = tracks.front();
	}

	Ref media = new Media();
	media->image = new Image(path);
	media->duration = 5.0f;
	media->type = MediaType::IMAGE;
	media->name = fs::path(path).filename().string();

	media->image->cache_frame(viewport_size);
	selected_track->insert_media(current_time, media);
}

void titian::VideoLayer::load_audio(const String& path)
{
	if (!selected_track) {
		if (tracks.empty()) {
			tracks.emplace_back(new Track());
		}
		selected_track = tracks.front();
	}

	Ref media = new Media();
	media->audio = new Audio(path);
	media->duration = media->audio->duration();
	media->type = MediaType::AUDIO;
	media->name = fs::path(path).filename().string();
	selected_track->insert_media(current_time, media);
}

void titian::VideoLayer::load_video(const String& path)
{
	if (!selected_track) {
		if (tracks.empty()) {
			tracks.emplace_back(new Track());
		}
		selected_track = tracks.front();
	}

	Ref media = new Media();
	media->audio = new Audio(path);
	media->video = new Video(path);
	media->duration = media->video->duration();
	media->type = MediaType::VIDEO;
	media->name = fs::path(path).filename().string();

	media->video->cache_scale = viewport_size;
	selected_track->insert_media(current_time, media);
}

int titian::VideoLayer::find_track(const Ref<Track>& track) const
{
	for (int i = 0; i < (int) tracks.size(); i++) {
		if (tracks[i] == track) {
			return i;
		}
	}
	return -1;
}

void titian::VideoLayer::delete_track(const Ref<Track>& track)
{
	for (int i = 0; i < (int) tracks.size(); i++) {
		if (tracks[i] == track) {
			tracks.erase(tracks.begin() + i);
			return;
		}
	}
}

void titian::VideoLayer::move_track_up(const Ref<Track>& track)
{
	const int index = find_track(track);
	if (index > 0) {
		std::swap(tracks[index], tracks[(size_t) index - 1]);
	}
}

void titian::VideoLayer::move_track_down(const Ref<Track>& track)
{
	const int index = find_track(track);
	if (index >= 0 && index < (int) tracks.size() - 1) {
		std::swap(tracks[index], tracks[(size_t) index + 1]);
	}
}

titian::Ref<titian::Track> titian::VideoLayer::find_track(const Ref<Media>& media) const
{
	for (const auto& track : tracks) {
		for (const auto& [_, med] : track->media) {
			if (med == media) {
				return track;
			}
		}
	}
	return {};
}

void titian::VideoLayer::delete_media(const Ref<Media>& media)
{
	for (auto& track : tracks) {
		for (const auto& [offset, med] : track->media) {
			if (med == media) {
				track->media.erase(offset);
				return;
			}
		}
	}
}

float titian::VideoLayer::get_offset(const Ref<Media>& media) const
{
	for (const auto& track : tracks) {
		for (const auto& [offset, med] : track->media) {
			if (med == media) {
				return offset;
			}
		}
	}
	return 0.0f;
}

void titian::VideoLayer::update_offset(const Ref<Media>& media, const float offset)
{
	Ref track = find_track(media);
	if (!track) {
		return;
	}
	if (track->media.contains(offset)) {
		return;
	}
	track->remove_media(media);
	track->insert_media(offset, media);
}

void titian::VideoLayer::split_audio(Ref<Media>& media)
{
	if (media->audio->duration() <= 0.0f) {
		return;
	}

	int next_track_index = -1;
	float media_offset = 0.0f;
	for (int i = 0; i < (int) tracks.size(); i++) {
		for (auto& [offset, med] : tracks[i]->media) {
			if (med == media) {
				next_track_index = i + 1;
				media_offset = offset;
				i = (int) tracks.size();
				break;
			}
		}
	}

	if (next_track_index < 0) {
		return;
	}
	if (next_track_index >= (int) tracks.size()) {
		tracks.resize((size_t) next_track_index + 1);
		for (auto& track : tracks) {
			if (!track) {
				track = new Track();
			}
		}
	}

	Ref new_media = new Media();
	new_media->audio = new Audio();
	new_media->audio->out_audio = media->audio->out_audio;
	media->audio->out_audio.clear();
	new_media->duration = new_media->audio->duration();
	new_media->type = MediaType::AUDIO;
	new_media->name = media->name;
	tracks[next_track_index]->insert_media(media_offset, new_media);
}

void titian::VideoLayer::clear_frame() const
{
	VideoLayer* video_layer = Layers::get<VideoLayer>();
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;

	gpu->bind_access_view_for_compute_shader(out_frame.access_view, 0);
	const Int2 dispatch_size = out_frame.size() / 32 + Int2(1);
	gpu->execute_compute_shader(video_layer->m_clear_shader, dispatch_size.x, dispatch_size.y, 1);
	gpu->unbind_access_view_for_compute_shader(0);
}

void titian::VideoLayer::mix_frame(const Frame& frame) const
{
	kl::GPU* gpu = &Layers::get<AppLayer>()->gpu;
	gpu->bind_access_view_for_compute_shader(out_frame.access_view, 0);
	gpu->bind_access_view_for_compute_shader(frame.access_view, 1);
	const Int2 dispatch_size = kl::min(out_frame.size(), frame.size()) / 32 + Int2(1);
	gpu->execute_compute_shader(m_mix_shader, dispatch_size.x, dispatch_size.y, 1);
	gpu->unbind_access_view_for_compute_shader(1);
	gpu->unbind_access_view_for_compute_shader(0);
}

void titian::VideoLayer::play_audio()
{
	prepare_audio();

	if (!m_audio.empty()) {
		const int index = kl::clamp(m_audio.time_to_index(current_time), 0, (int) m_audio.size() - 1);
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
			media->store_audio();
			kl::async_for(0, (int) m_audio.size(), [&](const int i)
			{
				const float time = m_audio.index_to_time(i) - offset;
				m_audio[i] += media->out_audio.sample(time);
			});
		}
	}
}
