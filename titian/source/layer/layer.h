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
			requires(!std::is_pointer_v<T> and !std::is_reference_v<T>)
		static constexpr void bind(T& obj) noexcept
		{
			m_data[type_id<T>] = &obj;
		}

		template<typename T>
			requires(!std::is_pointer_v<T> and !std::is_reference_v<T>)
		static constexpr T& get() noexcept
		{
			return *(T*) m_data[type_id<T>];
		}

	private:
		static inline int m_type_id_seq = 0;
		template<typename T> static const int type_id = m_type_id_seq++;
		static inline void* m_data[16] = {};
	};
}
