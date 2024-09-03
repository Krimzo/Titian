#pragma once

#include "standard.h"


namespace titian {
	inline int generate_id() noexcept
	{
		static int id = 0;
		return ++id;
	}
}

namespace titian {
	struct Unique
	{
		const int id = generate_id();
	};
}
