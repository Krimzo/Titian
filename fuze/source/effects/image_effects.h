#pragma once

#include "effects/image_effect.h"


namespace titian {
	struct ImageEffectSclRotPos : ImageEffect
	{
		ImageEffectSclRotPos();

		String name() const override;
		bool needs_copy() const override;
		void display_gui() override;
		Ref<ImageEffect> make_copy() const override;

		String get_source() const;

		Float2& scale();
		float& rotation();
		Float2& position();
	};
}
