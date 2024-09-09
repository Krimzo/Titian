#pragma once

#include "storage/video.h"


namespace titian {
	struct Frame : kl::NoCopy
	{
		dx::Texture texture;
		dx::ShaderView shader_view;
		dx::AccessView access_view;

		Frame();

		void upload(const RAWImage& image);
		void retrieve(RAWImage& image) const;

		Int2 size() const;
		void resize(const Int2& size);

	private:
		dx::Texture m_staging_texture;
		Int2 m_size;
	};
}
