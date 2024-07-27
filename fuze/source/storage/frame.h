#pragma once

#include "storage/video.h"


namespace titian {
	class Frame : kl::NoCopy
	{
	public:
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
		Int2 m_size = {};
	};
}
