#pragma once

#include "editor/game_layer.h"
#include "render/render_states.h"
#include "render/render_pass.h"
#include "scene/camera.h"


namespace titian {
	class RenderLayer : public Layer
	{
	public:
		GameLayer* game_layer = nullptr;

		std::vector<kl::Object<RenderPass>> passes = {};
		kl::Object<RenderStates> states = nullptr;

		kl::Object<Texture> render_texture = nullptr;
		kl::Object<Texture> depth_texture = nullptr;
		kl::Object<Texture> picking_texture = nullptr;
		kl::Object<Texture> staging_texture = nullptr;

		kl::dx::Buffer screen_mesh = nullptr;

		kl::Float4 background = kl::Color(30, 30, 30);
		bool render_wireframe = false;
		bool v_sync = true;

		RenderLayer(GameLayer* game_layer);

		bool update() override;

		void present() const;
		void resize(const kl::Int2& new_size);

	private:
		kl::Int2 get_render_texture_size() const;
	};
}
