#pragma once

#include "serialization/serializable.h"


namespace titian {
	class Texture : public Serializable
	{
	public:
		// jpg has really good compression but isn't lossless
		// png is lossless but the compression isn't as good
		static constexpr const char* COMPRESSION_TYPE = "jpg";
		using Data = kl::Image;

		Data data_buffer = {};
		kl::dx::Texture graphics_buffer = nullptr;

		kl::dx::TargetView target_view = nullptr;
		kl::dx::DepthView depth_view = nullptr;
		kl::dx::ShaderView shader_view = nullptr;
		kl::dx::AccessView access_view = nullptr;

		Texture(kl::GPU* gpu);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		void reload_as_2D(bool has_unordered_access = false, bool is_target = false);
		bool reload_as_cube();

		void create_target_view(const kl::dx::TargetViewDescriptor* descriptor = nullptr);
		void create_depth_view(const kl::dx::DepthViewDescriptor* descriptor = nullptr);
		void create_shader_view(const kl::dx::ShaderViewDescriptor* descriptor = nullptr);
		void create_access_view(const kl::dx::AccessViewDescriptor* descriptor = nullptr);

		bool is_cube() const;
		kl::Int2 graphics_buffer_size() const;

	private:
		kl::GPU* m_gpu = nullptr;
		bool m_is_cube = false;
	};
}
