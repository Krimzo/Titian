#pragma once

#include "effects/image_effect.h"


namespace titian {
	class ImageEffectScaleRotPos : public ImageEffect
	{
	public:
		ImageEffectScaleRotPos();

		bool needs_copy() const override;
		String get_source() const override;

		String get_name() const override;
		void display_gui() override;

		Ref<ImageEffect> make_copy() const override;

		Float2& scale();
		float& rotation();
		Float2& position();
	};
}
