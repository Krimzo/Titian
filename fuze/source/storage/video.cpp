#include "fuze.h"


titian::Video::Video(const String& path)
	: m_path(path)
{
	kl::VideoReader temp_reader(path, false);
	m_resolution = temp_reader.frame_size();
	m_duration = temp_reader.duration_seconds();
	m_fps = temp_reader.fps();
}

float titian::Video::duration_seconds() const
{
	return m_duration;
}

void titian::Video::cache_frames(const float time, const float duration)
{
	Image dummy;
	const int count = int(duration / buffer_range) + 1;
	for (int i = 0; i < count; i++) {
		get_frame(time + i * buffer_range, dummy);
	}
}

bool titian::Video::get_frame(const float time, Image& out)
{
	const int first_frame_index = int(time * m_fps);

	m_frames_lock.lock();
	if (m_frames.contains(first_frame_index)) {
		out.load_from_vector(m_frames[first_frame_index]);
		m_frames_lock.unlock();
		return true;
	}
	m_frames_lock.unlock();

	if (m_queued_frames.contains(first_frame_index) || m_current_work.load() >= MAX_WORK_COUNT) {
		return false;
	}
	const int next_queue_size = int(buffer_range * m_fps);
	for (int i = 0; i < next_queue_size; i++) {
		m_queued_frames.insert(first_frame_index + i);
	}

	if (m_current_work.load() == 0) {
		m_work.finalize();
	}
	++m_current_work;
	m_work.add_task([this, time, first_frame_index, next_queue_size]
	{
		kl::VideoReader reader(m_path, true);
		Map<int, Vector<byte>> temp_frames;
		
		Set<int> existing_frames;
		m_frames_lock.lock();
		for (const auto& [index, _] : m_frames) {
			existing_frames.insert(index);
		}
		m_frames_lock.unlock();

		Image frame;
		if (reader.get_frame(time, frame)) {
			if (!existing_frames.contains(first_frame_index)) {
				frame.save_to_vector(&temp_frames[first_frame_index], kl::ImageType::JPG);
			}
		}

		for (int i = 1; i < next_queue_size; i++) {
			const int frame_index = first_frame_index + i;
			if (!reader.read_frame(frame)) {
				break;
			}
			if (!existing_frames.contains(frame_index)) {
				frame.save_to_vector(&temp_frames[frame_index], kl::ImageType::JPG);
			}
		}

		m_frames_lock.lock();
		m_frames.insert(temp_frames.begin(), temp_frames.end());
		m_frames_lock.unlock();

		--m_current_work;
	});
	return false;
}
