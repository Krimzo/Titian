#include "fuze.h"


titian::VideoLayer::VideoLayer()
	: Layer("VideoLayer")
{}

void titian::VideoLayer::init()
{
	kl::Window* window = &Layers::get<AppLayer>()->window;
	window->set_icon("package/textures/video_editor_icon.ico");

	for (int i = 0; i < 5; i++) {
		tracks.emplace_back(new Track());
	}

	m_handler.init();
}

bool titian::VideoLayer::update()
{
	AppLayer* app_layer = Layers::get<AppLayer>();
	kl::GPU* gpu = &app_layer->gpu;
	kl::Timer* timer = &app_layer->timer;

	if (m_playing) {
		current_time = m_last_time + timer->elapsed();
	}
	if (m_renderer) {
		if (m_renderer->progress() >= 1.0f) {
			m_renderer = {};
		}
		else {
			m_renderer->load_frame();
		}
	}

	gpu->clear_internal(kl::colors::BLACK);
	return true;
}

void titian::VideoLayer::play()
{
	if (m_playing || m_renderer) {
		return;
	}
	m_playing = true;
	m_last_time = current_time;
	play_audio();
}

void titian::VideoLayer::stop()
{
	if (!m_playing) {
		return;
	}
	m_playing = false;
	current_time = m_last_time;
}

bool titian::VideoLayer::playing() const
{
	return m_playing;
}

void titian::VideoLayer::start_rendering(
	const Int2 frame_size,
	const int fps,
	const int video_bit_rate,
	const int audio_sample_rate)
{
	if (m_renderer || m_playing) {
		return;
	}
	if (auto file = kl::choose_file(true)) {
		if (auto format = classify_video_format(file.value())) {
			m_renderer = new FuzeRenderer(file.value(), format.value(), frame_size, fps, video_bit_rate, audio_sample_rate);
		}
		else if (auto type = classify_audio_format(file.value())) {
			m_renderer = new FuzeRenderer(file.value(), type.value(), audio_sample_rate);
		}
		if (m_renderer) {
			m_renderer->load_tracks(tracks);
			m_renderer->load_audio();
		}
	}
}

void titian::VideoLayer::stop_rendering()
{
	if (!m_renderer) {
		return;
	}
	m_renderer = {};
}

bool titian::VideoLayer::rendering() const
{
	return m_renderer;
}

float titian::VideoLayer::render_progress() const
{
	if (m_renderer) {
		return m_renderer->progress();
	}
	return 0.0f;
}

bool titian::VideoLayer::can_edit() const
{
	return !m_playing && !m_renderer;
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

void titian::VideoLayer::store_frame(const Int2 size)
{
	m_handler.prepare_frame(size);

	EffectPackage package;
	package.current_time = current_time;
	for (int i = int(tracks.size()) - 1; i >= 0; i--) {
		float offset = 0.0f;
		if (Ref media = tracks[i]->get_media(current_time, offset)) {
			package.media_start = offset;
			package.media_end = offset + media->duration;
			media->store_frame(package, false);
			m_handler.mix_frame(media->out_frame);
		}
	}
}

titian::Int2 titian::VideoLayer::frame_size() const
{
	return m_handler.out_frame.size();
}

dx::ShaderView titian::VideoLayer::get_shader_view() const
{
	return m_handler.out_frame.shader_view;
}

void titian::VideoLayer::retrieve_frame(RAWImage& out_image) const
{
	m_handler.out_frame.retrieve(out_image);
}

void titian::VideoLayer::load_file(const StringView& path)
{
	static const StringSet image_extensions = { ".bmp", ".png", ".jpg", ".jpeg" };
	static const StringSet audio_extensions = { ".wav", ".m4a", ".mp3" };
	static const StringSet video_extensions = { ".mkv", ".mpg", ".mp4" };

	const String extension = fs::path(path).extension().string();
	if (image_extensions.contains(extension)) {
		load_image(path);
	}
	else if (audio_extensions.contains(extension)) {
		load_audio(path);
	}
	else if (video_extensions.contains(extension)) {
		load_video(path);
	}
}

void titian::VideoLayer::load_image(const StringView& path)
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

	media->image->cache_frame(m_handler.out_frame.size());
	selected_track->insert_media(current_time, media);
}

void titian::VideoLayer::load_audio(const StringView& path)
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

void titian::VideoLayer::load_video(const StringView& path)
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

	media->video->cache_scale = m_handler.out_frame.size();
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
				goto media_loop_end;
			}
		}
	}
media_loop_end:

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

titian::Optional<kl::VideoType> titian::VideoLayer::classify_video_format(const StringView& path)
{
	const String extension = fs::path(path).extension().string();
	if (extension == ".mp4") {
		return kl::VideoType::h264();
	}
	return std::nullopt;
}

titian::Optional<kl::AudioType> titian::VideoLayer::classify_audio_format(const StringView& path)
{
	const String extension = fs::path(path).extension().string();
	if (extension == ".wav") {
		return kl::AudioType::WAV;
	}
	if (extension == ".mp3") {
		return kl::AudioType::MP3;
	}
	return std::nullopt;
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

	EffectPackage package;
	package.current_time = current_time;
	for (auto& track : tracks) {
		for (auto& [offset, media] : track->media) {
			package.media_start = offset;
			package.media_end = offset + media->duration;
			media->store_audio(package);
			kl::async_for(0, (int) m_audio.size(), [&](const int i)
			{
				const float time = m_audio.index_to_time(i) - offset;
				m_audio[i] += media->out_audio.sample(time);
			});
		}
	}
}
