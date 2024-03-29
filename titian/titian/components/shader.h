#pragma once

#include "serialization/serializable.h"


namespace titian {
	class Shader : public Serializable
	{
	public:
		using Data = std::string;

		Data data_buffer = {};
		kl::RenderShaders graphics_buffer = {};

		Shader(kl::GPU* gpu);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		void reload();

	private:
		kl::GPU* m_gpu = nullptr;

		std::string process_source() const;
	};
}
