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

		Image frame;
		if (reader.get_frame(time, frame)) {
			m_frames_lock.lock();
			if (!m_frames.contains(first_frame_index)) {
				frame.save_to_vector(&m_frames[first_frame_index], kl::ImageType::JPG);
			}
			m_frames_lock.unlock();
		}

		for (int i = 1; i < next_queue_size; i++) {
			const int frame_index = first_frame_index + i;
			if (!reader.read_frame(frame)) {
				break;
			}

			m_frames_lock.lock();
			if (!m_frames.contains(frame_index)) {
				frame.save_to_vector(&m_frames[frame_index], kl::ImageType::JPG);
			}
			m_frames_lock.unlock();
		}
		--m_current_work;
	});
	return false;
}
