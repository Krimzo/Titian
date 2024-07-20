#pragma once

#include "editor/game_layer.h"
#include "render/render_states.h"
#include "render/render_pass.h"
#include "scene/camera.h"


namespace titian {
	class RenderLayer : public Layer
	{
	public:
		Vector<Ref<RenderPass>> passes = {};
		Ref<RenderStates> states = nullptr;

		Ref<Texture> screen_texture = nullptr;
		Ref<Texture> game_color_texture = nullptr;
		Ref<Texture> game_depth_texture = nullptr;
		Ref<Texture> editor_picking_texture = nullptr;
		Ref<Texture> editor_staging_texture = nullptr;

		kl::dx::Buffer screen_mesh = nullptr;

		kl::Float4 background = kl::Color(30, 30, 30);
		bool render_wireframe = false;
		bool v_sync = false;

		RenderLayer();

		void init();
		bool update() override;

		void present() const;
		void resize(const kl::Int2& new_size);
		void resize_staging(const kl::Int2& new_size);

		kl::Int2 get_render_texture_size() const;
	};
}
