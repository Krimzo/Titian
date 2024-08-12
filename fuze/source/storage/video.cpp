#include "fuze.h"


titian::Video::Video(const String& path)
	: m_path(path)
{
	kl::VideoReader temp_reader(path, {}, false);
	m_resolution = temp_reader.frame_size();
	m_duration = temp_reader.duration_seconds();
	m_fps = temp_reader.fps();

	m_buffering_states.resize(size_t(m_duration / BUFFERING_LENGTH) + 1);
	m_buffering_sections.resize(size_t(m_duration / BUFFERING_LENGTH) + 1);
}

titian::Video::~Video()
{
	this->wait();
}

float titian::Video::duration() const
{
	return m_duration;
}

void titian::Video::store_frame(const float time, const bool wait)
{
	cache_frames(time);
	cache_frames(time + BUFFERING_LENGTH * 0.5f);
	if (wait) {
		this->wait();
	}

	const int frame_index = int(time * m_fps);
	m_frames_lock.lock();
	if (m_frames.contains(frame_index)) {
		out_frame.load_from_buffer(m_frames[frame_index]);
	}
	m_frames_lock.unlock();
}

void titian::Video::cache_frames(const float time)
{
	int section_index = int(time / BUFFERING_LENGTH);
	section_index = kl::clamp(section_index, 0, (int) m_buffering_states.size() - 1);
	if (m_buffering_states[section_index]) {
		return;
	}
	m_buffering_states[section_index] = true;
	m_threads.emplace_back([this, section_index]
	{
		const int frames_per_section = int(BUFFERING_LENGTH * m_fps);
		const int frames_per_core = frames_per_section / SECTION_THREAD_COUNT;

		WorkQueue queue;
		for (int core_index = 0; core_index < SECTION_THREAD_COUNT; core_index++) {
			queue.add_task([this, section_index, frames_per_section, frames_per_core, core_index]
			{
				kl::VideoReader reader{ m_path, cache_scale, true };
				Map<int, String> temp_frames;

				const int start_index = section_index * frames_per_section + core_index * frames_per_core;
				const int end_index = start_index + frames_per_core;

				RAWImage frame;
				if (start_index > 0) {
					if (!reader.seek(start_index / m_fps)) {
						return;
					}
				}
				int index = 0;
				while (true) {
					if (!reader.read_frame(frame, &index)) {
						break;
					}
					if (index > end_index) {
						break;
					}
					if (index % int(m_fps) == 0) {
						m_buffering_sections[section_index][core_index] = { false, kl::unlerp((float) index, (float) start_index, (float) end_index) };
					}
					frame.save_to_buffer(temp_frames[index], RAWImageType::JPG);
				}

				m_frames_lock.lock();
				m_buffering_sections[section_index][core_index] = { true, kl::unlerp((float) index, (float) start_index, (float) end_index) };
				m_frames.insert(temp_frames.begin(), temp_frames.end());
				m_frames_lock.unlock();
			});
		}
		queue.finalize();
	});
}

void titian::Video::wait()
{
	m_threads.clear();
}

titian::Vector<titian::Vector<titian::Pair<bool, float>>> titian::Video::buffering_sections() const
{
	Vector<Vector<Pair<bool, float>>> result;
	for (const auto& section : m_buffering_sections) {
		Vector<Pair<bool, float>> section_data;
		for (auto& [state, value] : section) {
			section_data.emplace_back(state, value);
		}
		result.push_back(section_data);
	}
	return result;
}

titian::Ref<titian::Video> titian::Video::make_copy() const
{
	Ref video = new Video(m_path);
	return video;
}
