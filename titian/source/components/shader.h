#pragma once

#include "serialization/serialization.h"


namespace titian {
	inline std::set<String, std::less<>> HLSL_KEYWORDS;
	inline std::set<String, std::less<>> HLSL_TYPES;
	inline std::set<String, std::less<>> HLSL_MEMBERS;
	inline std::set<String, std::less<>> HLSL_FUNCTIONS;
}

namespace titian {
	enum ShaderType : int32_t
	{
		MATERIAL = 0,
		CAMERA = 1,
	};
}

namespace titian {
	class Shader : public Serializable
	{
	public:
		int32_t shader_type = ShaderType::MATERIAL;

		String data_buffer;
		kl::RenderShaders graphics_buffer;

		Shader(kl::GPU* gpu, ShaderType type);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		void reload();

	private:
		kl::GPU* m_gpu = nullptr;

		void reload_for_material();
		void reload_for_camera();
	};
}
