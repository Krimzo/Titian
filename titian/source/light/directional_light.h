#pragma once

#include "light/light.h"
#include "components/texture.h"
#include "scene/camera.h"


namespace titian {
	struct DirectionalLight : Light
	{
		static constexpr int CASCADE_COUNT = 4;

		Float3 color{ 1.0f };
		float point_size = 1.0f;
		float cascade_splits[CASCADE_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };

		DirectionalLight(px::PxPhysics* physics, kl::GPU* gpu);

        void serialize(Serializer* serializer, const void* helper_data) const override;
        void deserialize(const Serializer* serializer, const void* helper_data) override;

        Float3 light_at_point(const Float3& point) const override;

		void set_resolution(int resolution);
		int resolution() const;

        void set_direction(const Float3& direction);
        Float3 direction() const;

        dx::DepthView depth_view(int cascade_index) const;
		dx::ShaderView shader_view(int cascade_index) const;

		Float4x4 light_matrix(Camera* camera, int cascade_index) const;

	private:
		kl::GPU* m_gpu = nullptr;
		int m_resolution = 0;
        Float3 m_direction = { 0.0f, -1.0f, 0.0f };
		Ref<Texture> m_cascades[CASCADE_COUNT] = {};
	};
}
