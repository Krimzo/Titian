#pragma once

#include "klibrary.h"


namespace titian {
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual bool update() = 0;

	protected:
		void panic() const;
	};
}
