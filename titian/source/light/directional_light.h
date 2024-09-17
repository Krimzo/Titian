#pragma once

#include "components/texture.h"
#include "scene/camera.h"


namespace titian {
	struct DirectionalLight : Entity
	{
		static constexpr int CASCADE_COUNT = 4;

		Float3 color{ 1.0f };
		float point_size = 1.0f;
		float cascade_splits[CASCADE_COUNT + 1] = { 0.0f, 0.075f, 0.2f, 0.5f, 1.0f };

		DirectionalLight();

        void serialize(Serializer* serializer) const override;
        void deserialize(const Serializer* serializer) override;

		void set_resolution(int resolution);
		int resolution() const;

        void set_direction(const Float3& direction);
        Float3 direction() const;

        dx::DepthView depth_view(int cascade_index) const;
		dx::ShaderView shader_view(int cascade_index) const;

		Float4x4 light_matrix(Camera* camera, int cascade_index) const;

	private:
		int m_resolution = 0;
        Float3 m_direction = { 0.0f, -1.0f, 0.0f };
		Ref<Texture> m_cascades[CASCADE_COUNT] = {};
	};
}
