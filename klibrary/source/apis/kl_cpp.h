#pragma once

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <cstdint>
#include <ctime>
#include <execution>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <ranges>
#include <set>
#include <source_location>
#include <sstream>
#include <syncstream>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>


namespace kl {
	struct NoCopy
	{
	protected:
		NoCopy() = default;
	private:
		NoCopy(const NoCopy&) = delete;
		void operator=(const NoCopy&) = delete;
	};
}

namespace kl {
	struct NoMove
	{
	protected:
		NoMove() = default;
	private:
		NoMove(NoMove&&) = delete;
		void operator=(NoMove&&) = delete;
	};
}
