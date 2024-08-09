#pragma once

#include "serialization/serialization.h"


namespace titian {
	class Texture : public Serializable
	{
	public:
		// jpg has really good compression but isn't lossless
		// png is lossless but the compression isn't as good
		static constexpr kl::ImageType COMPRESSION_TYPE = kl::ImageType::JPG;
		using Data = kl::Image;

		Data data_buffer = {};
		dx::Texture graphics_buffer;

		dx::TargetView target_view;
		dx::DepthView depth_view;
		dx::ShaderView shader_view;
		dx::AccessView access_view;

		Texture(kl::GPU* gpu);

		void serialize(Serializer* serializer, const void* helper_data) const override;
		void deserialize(const Serializer* serializer, const void* helper_data) override;

		void reload_as_2D(bool has_unordered_access = false, bool is_target = false);
		bool reload_as_cube();

		void create_target_view(const dx::TargetViewDescriptor* descriptor = nullptr);
		void create_depth_view(const dx::DepthViewDescriptor* descriptor = nullptr);
		void create_shader_view(const dx::ShaderViewDescriptor* descriptor = nullptr);
		void create_access_view(const dx::AccessViewDescriptor* descriptor = nullptr);

		bool is_cube() const;
		Int2 graphics_buffer_size() const;

	private:
		kl::GPU* m_gpu = nullptr;
		bool m_is_cube = false;
	};
}
