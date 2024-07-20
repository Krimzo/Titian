#pragma once

#include "serialization/serializable.h"


namespace titian {
	enum ShaderType : int
	{
		MATERIAL = 0,
		CAMERA = 1,
	};
}

namespace titian {
	class Shader : public Serializable
	{
	public:
		using Data = String;

		ShaderType type = ShaderType::MATERIAL;
		Data data_buffer = {};
		kl::RenderShaders graphics_buffer = {};

		Shader(ShaderType type, kl::GPU* gpu);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		void reload();

	private:
		kl::GPU* m_gpu = nullptr;

		void reload_for_material();
		void reload_for_camera();
	};
}
