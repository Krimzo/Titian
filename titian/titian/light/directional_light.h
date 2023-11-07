#pragma once

#include "light/light.h"
#include "components/texture.h"
#include "scene/camera.h"


namespace titian {
	class DirectionalLight : public Light
	{
	public:
		static constexpr uint32_t CASCADE_COUNT = 4;
		static constexpr float CASCADE_SPLITS[CASCADE_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };

        float point_size = 1.0f;
		kl::Float3 color{ 1.0f };

		DirectionalLight(physx::PxPhysics* physics, bool dynamic, kl::GPU* gpu, uint32_t map_resolution);

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        kl::Float3 light_at_point(const kl::Float3& point) const override;

        uint32_t map_resolution() const;

        void set_direction(const kl::Float3& direction);
        kl::Float3 direction() const;

        kl::dx::DepthView depth_view(uint32_t cascade_index) const;
		kl::dx::ShaderView shader_view(uint32_t cascade_index) const;

		kl::Float4x4 light_matrix(Camera* camera, uint32_t cascade_index) const;

	private:
        uint32_t m_map_resolution = 0;
        kl::Float3 m_direction = { 0.0f, -1.0f, 0.0f };
		kl::Object<Texture> m_cascades[CASCADE_COUNT] = {};
	};
}
