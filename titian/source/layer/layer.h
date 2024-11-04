#pragma once

#include "utility/bench_util.h"
#include "utility/helper_util.h"


namespace titian {
	struct Layer : kl::NoCopy, BenchmarkInfo
	{
		template<typename T>
		friend struct Layer_T;

		virtual ~Layer() = default;
		virtual bool update() = 0;

	protected:
		void panic() const;

	private:
		Layer(const StringRef& name);
	};
}

namespace titian {
	template<typename T>
	struct Layer_T : Layer
	{
		inline Layer_T(const StringRef& name)
			: Layer(name)
		{}

		static T& get()
		{
			static T inst{};
			return inst;
		}
	};
}
