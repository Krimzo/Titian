#pragma once

#include "effects/effect_package.h"
#include "storage/image.h"
#include "storage/frame.h"


namespace titian {
	struct ImageEffect : kl::NoCopy
	{
		Frame temp_frame;
		kl::ShaderHolder<dx::ComputeShader> shader{ nullptr };
		Float4x4 custom_data;

		ImageEffect();
		virtual ~ImageEffect();

		virtual bool needs_copy() const = 0;
		virtual String get_source() const = 0;

		virtual String get_name() const = 0;
		virtual void display_gui() = 0;

		virtual void init() final;
		virtual void apply(const EffectPackage& package, Frame& frame) final;

		virtual Ref<ImageEffect> make_copy() const = 0;
	};
}
