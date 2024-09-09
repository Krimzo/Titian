#pragma once

#include "standard.h"


namespace titian {
	struct WorkQueue
	{
		WorkQueue() = default;

		void add_task(auto&& task)
		{
			m_queue.push_back(std::async(task));
		}

		void finalize();

	private:
		Vector<std::future<void>> m_queue;
	};
}
