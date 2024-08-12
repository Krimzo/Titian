#pragma once

#include "apis/apis.h"


namespace kl {
	template<typename T, size_t S = sizeof(T)>
		requires (S >= sizeof(T))
	struct Wrap
	{
		friend struct Wrap;

		// create
		Wrap()
		{}

		template<typename... Args>
		static Wrap make(const Args&... args)
		{
			Wrap wrap;
			wrap.m_valid = true;
			new (wrap.m_buffer) T(args...);
			return wrap;
		}

		// destroy
		~Wrap() noexcept
		{
			self.destroy();
		}

		// copy
		Wrap(const Wrap&) = delete;
		Wrap& operator=(const Wrap&) = delete;

		// move
		template<typename D, size_t DS>
			requires std::is_base_of_v<T, D> and (S >= DS)
		Wrap(Wrap<D, DS>&& other) noexcept
		{
			m_valid = other.m_valid;
			memcpy(m_buffer, other.m_buffer, DS);
			other.clear();
		}

		template<typename D, size_t DS>
			requires std::is_base_of_v<T, D> and (S >= DS)
		Wrap& operator=(Wrap<D, DS>&& other)
		{
			self.destroy();
			self.clear();
			m_valid = other.m_valid;
			memcpy(m_buffer, other.m_buffer, DS);
			other.clear();
			return self;
		}

		// cast
		template<typename D>
			requires std::is_base_of_v<T, D>
		D* as()
		{
			if (m_valid) {
				return dynamic_cast<D*>(&self);
			}
			return nullptr;
		}

		template<typename D>
			requires std::is_base_of_v<T, D>
		const D* as() const
		{
			if (m_valid) {
				return dynamic_cast<const D*>(&self);
			}
			return nullptr;
		}

		// access
		T* operator&()
		{
			return reinterpret_cast<T*>(m_buffer);
		}

		const T* operator&() const
		{
			return reinterpret_cast<const T*>(m_buffer);
		}

		T& operator*()
		{
			return *(&self);
		}

		const T& operator*() const
		{
			return *(&self);
		}

		T* operator->()
		{
			return &self;
		}

		const T* operator->() const
		{
			return &self;
		}

		// info
		operator bool() const
		{
			return m_valid;
		}

	private:
		bool m_valid = false;
		uint8_t m_buffer[S] = {};

		inline void destroy()
		{
			if (m_valid) {
				(*self).~T();
			}
		}

		inline void clear()
		{
			m_valid = false;
			memset(m_buffer, 0, S);
		}
	};
}
