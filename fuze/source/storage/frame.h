#pragma once

#include "storage/video.h"


namespace titian {
	class Frame
	{
	public:
		dx::Texture texture;
		dx::ShaderView shader_view;
		dx::AccessView access_view;

		Frame();

		Frame(const Frame&) = delete;
		Frame(Frame&&) = delete;

		void operator=(const Frame&) = delete;
		void operator=(Frame&&) = delete;

		void upload(const RAWImage& image);
		void retrieve(RAWImage& image) const;

		Int2 size() const;
		void resize(const Int2& size);

	private:
		dx::Texture m_staging_texture;
		Int2 m_size = {};
	};
}
