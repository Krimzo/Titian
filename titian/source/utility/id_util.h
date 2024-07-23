#pragma once

#include "standard.h"


namespace titian {
	inline int generate_id() noexcept
	{
		static int _counter = 0;
		return ++_counter;
	}
}

namespace titian {
	class Unique
	{
	public:
		const int id = generate_id();

		inline Unique() = default;
	};
}
