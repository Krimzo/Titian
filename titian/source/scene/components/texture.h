#pragma once

#include "serial/serial.h"


namespace titian {
	struct Texture : Serializable
	{
		static constexpr kl::ImageType COMPRESSION_TYPE = kl::ImageType::JPG;

		kl::Image image;
		dx::Texture texture;

		dx::TargetView target_view;
		dx::DepthView depth_view;
		dx::ShaderView shader_view;
		dx::AccessView access_view;

		Texture();

		void serialize(Serializer& serializer) const override;
		void deserialize(const Serializer& serializer) override;

		void reload_as_2D();
		bool reload_as_cube();

		void create_target_view(const dx::TargetViewDescriptor* descriptor = nullptr);
		void create_depth_view(const dx::DepthViewDescriptor* descriptor = nullptr);
		void create_shader_view(const dx::ShaderViewDescriptor* descriptor = nullptr);
		void create_access_view(const dx::AccessViewDescriptor* descriptor = nullptr);

		bool is_cube() const;
		Int2 resolution() const;

		void copy_other(const dx::Texture& texture);
	};
}
