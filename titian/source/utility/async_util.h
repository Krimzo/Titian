#pragma once

#include "standard.h"


namespace titian {
	class WorkQueue
	{
	public:
		inline WorkQueue()
		{}

		inline void add_task(auto task)
		{
			m_queue.push_back(std::async(std::launch::async, task));
		}

		inline void finalize()
		{
			m_queue.clear();
		}

	private:
		Vector<std::future<void>> m_queue;
	};
}
