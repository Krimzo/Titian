#pragma once

#include "klibrary.h"


namespace titian {
	using ID = int;
	using IDStorage = std::unordered_set<ID>;

	namespace _priv {
		inline IDStorage _ids = {};
	}

	inline const IDStorage& ref_ids()
	{
		using namespace _priv;
		return _ids;
	}

	inline ID generate_id()
	{
		using namespace _priv;

		ID id;
		while (true) {
			id = rand();
			if (_ids.contains(id)) {
				continue;
			}
			_ids.insert(id);
			return id;
		}
	}

	inline void reset_ids()
	{
		using namespace _priv;
		_ids.clear();
	}
}

namespace titian {
	class Unique
	{
	public:
		ID id = generate_id();

		inline Unique() = default;
	};
}
