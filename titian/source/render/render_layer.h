#pragma once

#include "editor/game_layer.h"
#include "render/render_pass.h"
#include "scene/camera.h"

#include "render/states/raster_states.h"
#include "render/states/depth_states.h"
#include "render/states/sampler_states.h"
#include "render/states/shader_states.h"
#include "render/states/blend_states.h"


namespace titian {
	class RenderLayer : public Layer
	{
	public:
		Vector<Ref<RenderPass>> passes;
		
		Ref<RasterStates> raster_states;
		Ref<DepthStates> depth_states;
		Ref<SamplerStates> sampler_states;
		Ref<ShaderStates> shader_states;
		Ref<BlendStates> blend_states;

		Ref<Texture> screen_texture;
		Ref<Texture> game_color_texture;
		Ref<Texture> game_depth_texture;
		Ref<Texture> editor_picking_texture;
		Ref<Texture> editor_staging_texture;

		dx::Buffer screen_mesh;

		Float4 background = Color{ 30, 30, 30 };
		bool render_wireframe = false;
		bool v_sync = true;

		RenderLayer();

		void init();
		bool update() override;

		void present() const;
		void resize(const Int2& new_size);
		void resize_staging(const Int2& new_size);

		Int2 get_render_texture_size() const;
	};
}
