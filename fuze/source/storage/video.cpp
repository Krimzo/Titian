#include "fuze.h"


titian::Video::Video(const String& path)
	: m_path(path)
{
	kl::VideoReader temp_reader(path, {}, false);
	m_resolution = temp_reader.frame_size();
	m_duration = temp_reader.duration_seconds();
	m_fps = temp_reader.fps();
}

float titian::Video::duration_seconds() const
{
	return m_duration;
}

void titian::Video::cache_frames(const Int2& size)
{
	const int frame_count = int(m_duration * m_fps);
	const int core_count = kl::CPU_CORE_COUNT;
	const int frames_per_core = frame_count / core_count;

	WorkQueue queue;
	std::mutex lock;
	for (int core = 0; core <= core_count; core++) {
		queue.add_task([&, core]
		{
			kl::VideoReader reader(m_path, size, true);
			Map<int, Vector<byte>> temp_frames;

			int index = core * frames_per_core + 0;

			Image frame;
			if (!reader.get_frame(index / m_fps, frame)) {
				return;
			}
			frame.save_to_vector(&temp_frames[index], kl::ImageType::JPG);

			for (int i = 1; i < frames_per_core; i++) {
				index = core * frames_per_core + i;
				if (!reader.read_frame(frame)) {
					break;
				}
				frame.save_to_vector(&temp_frames[index], kl::ImageType::JPG);
			}

			lock.lock();
			m_frames.insert(temp_frames.begin(), temp_frames.end());
			lock.unlock();
		});
	}
	queue.finalize();
}

bool titian::Video::get_frame(const float time, Image& out)
{
	const int frame_index = int(time * m_fps);
	if (!m_frames.contains(frame_index)) {
		return false;
	}
	out.load_from_vector(m_frames[frame_index]);
	return true;
}
