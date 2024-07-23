#pragma once

#include "utility/time_util.h"


namespace titian {
	class Layer : public BenchmarkInfo
	{
	public:
		Layer(const String& name);
		virtual ~Layer() = default;

		Layer(const Layer&) = delete;
		Layer(Layer&&) = delete;

		void operator=(const Layer&) = delete;
		void operator=(Layer&&) = delete;

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
		static inline constexpr void bind(T* ptr) noexcept
		{
			m_data[type_id<T>] = ptr;
		}

		template<typename T>
		static inline constexpr T* get() noexcept
		{
			return static_cast<T*>(m_data[type_id<T>]);
		}

	private:
		static inline void* m_data[10] = {};
		static inline int m_type_id_seq = 0;
		template<typename T> static inline const int type_id = m_type_id_seq++;
	};
}
