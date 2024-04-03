#pragma once

#include "utility/time_util.h"


namespace titian {
	class Layer : public BenchmarkInfo
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer() = default;

		virtual bool update() = 0;

	protected:
		void panic() const;
	};
}
