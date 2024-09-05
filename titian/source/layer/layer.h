#pragma once

#include "utility/bench_util.h"


namespace titian {
	class Layer : kl::NoCopy, public BenchmarkInfo
	{
	public:
		Layer(const StringView& name);
		virtual ~Layer() = default;

		virtual bool update() = 0;

	protected:
		void panic() const;
	};
}

namespace titian {
	class Layers
	{
	public:
		template<typename T>
		static constexpr void bind(T* ptr) noexcept
		{
			m_data[type_id<T>] = ptr;
		}

		template<typename T>
		static constexpr T* get() noexcept
		{
			return static_cast<T*>(m_data[type_id<T>]);
		}

	private:
		static inline int m_type_id_seq = 0;
		template<typename T> static const int type_id = m_type_id_seq++;
		static inline void* m_data[16] = {};
	};
}
