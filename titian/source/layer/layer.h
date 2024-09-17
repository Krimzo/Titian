#pragma once

#include "utility/bench_util.h"
#include "utility/helper_util.h"


namespace titian {
	struct Layer : kl::NoCopy, BenchmarkInfo
	{
		Layer(const StringView& name);
		virtual ~Layer() = default;

		virtual bool update() = 0;

	protected:
		void panic() const;
	};
}

namespace titian {
	struct Layers
	{
		template<typename T>
		static constexpr void bind(T* ptr) noexcept
		{
			m_data[type_id<T>] = ptr;
		}

		template<typename T>
		static constexpr T* get() noexcept
		{
			return (T*) m_data[type_id<T>];
		}

	private:
		static inline int m_type_id_seq = 0;
		template<typename T> static const int type_id = m_type_id_seq++;
		static inline void* m_data[16] = {};
	};
}
