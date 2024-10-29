#pragma once

#include "effects/effect_package.h"
#include "storage/image.h"
#include "storage/frame.h"


namespace titian {
	struct ImageEffect : kl::NoCopy
	{
		Frame temp_frame;
		kl::ComputeShader compute_shader;
		Float4x4 custom_data;

		ImageEffect(const StringView& source);
		virtual ~ImageEffect() = default;

		virtual void apply(const EffectPackage& package, Frame& frame) final;

		virtual String name() const = 0;
		virtual bool needs_copy() const = 0;
		virtual void display_gui() = 0;
		virtual Ref<ImageEffect> make_copy() const = 0;
	};
}
