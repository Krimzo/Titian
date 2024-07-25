#include "fuze.h"


titian::Video::Video(const String& path)
{
	m_reader = new kl::VideoReader(path, true);
}

float titian::Video::duration_seconds() const
{
	return m_reader->duration_seconds();
}

bool titian::Video::get_frame(const float time, Image& out)
{
	const int first_index = int(time * m_reader->fps());
	if (m_frames.contains(first_index)) {
		out.load_from_vector(m_frames[first_index]);
		return true;
	}

	Image frame;
	if (m_reader->get_frame(time, frame)) {
		frame.save_to_vector(&m_frames[first_index], kl::ImageType::JPG);
		out = frame;
	}
	else {
		return false;
	}

	for (int i = 1; i < int(buffer_range * m_reader->fps()); i++) {
		const int frame_index = first_index + i;
		if (m_frames.contains(frame_index)) {
			continue;
		}
		if (!m_reader->read_frame(frame)) {
			break;
		}
		frame.save_to_vector(&m_frames[frame_index], kl::ImageType::JPG);
	}
	return true;
}
