#pragma once

#include "render/render_layer.h"
#include "gui/gui_section.h"


namespace titian {
	class GUILayer : public Layer
	{
	public:
		RenderLayer* render_layer = nullptr;

		std::vector<kl::Object<GUISection>> sections = {};
		float dpi_scaling = 1.0f;

		kl::Float4 special_color = kl::colors::WHITE;
		kl::Float4 alternate_color = kl::colors::BLACK;

		ImFont* roboto_font_small = nullptr;
		ImFont* roboto_font_large = nullptr;
		
		GUILayer(RenderLayer* render_layer);

		bool update() override;

	private:
		void load_custom_fonts();
		void load_custom_theme() const;
	};
}
