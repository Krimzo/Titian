#include "fuze.h"


titian::Video::Video(const String& path)
	: m_path(path)
{
	kl::VideoReader temp_reader(path, {}, false);
	m_resolution = temp_reader.frame_size();
	m_duration = temp_reader.duration_seconds();
	m_fps = temp_reader.fps();
}

float titian::Video::duration() const
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

			const int start_index = core * frames_per_core;
			const int end_index = (core + 1) * frames_per_core;

			RAWImage frame;
			if (start_index > 0) {
				reader.seek(start_index / m_fps);
			}
			while (true) {
				int index = -1;
				if (!reader.read_frame(frame, &index)) {
					break;
				}
				if (index > end_index) {
					break;
				}
				frame.save_to_vector(&temp_frames[index], RAWImageType::JPG);
			}

			lock.lock();
			m_frames.insert(temp_frames.begin(), temp_frames.end());
			lock.unlock();
		});
	}
	queue.finalize();
}

void titian::Video::store_frame(const float time)
{
	const int frame_index = int(time * m_fps);
	if (m_frames.contains(frame_index)) {
		out_frame.load_from_vector(m_frames[frame_index]);
	}
}
