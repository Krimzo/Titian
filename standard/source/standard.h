#pragma once

#include "klibrary.h"
#include "physx.h"
#include "chaiscript.hpp"
#include "assimp.h"

#undef min
#undef max
#undef assert
#undef VOID
#undef INFINITY

namespace fs = std::filesystem;
namespace dx = kl::dx;
namespace im = imgui;
namespace px = physx;
namespace cs = chaiscript;
namespace as = Assimp;

namespace titian {
	using Int2 = kl::Int2;
	using Int3 = kl::Int3;
	using Int4 = kl::Int4;

	using Float2 = kl::Float2;
	using Float3 = kl::Float3;
	using Float4 = kl::Float4;

	using Float2x2 = kl::Float2x2;
	using Float3x3 = kl::Float3x3;
	using Float4x4 = kl::Float4x4;

	using Complex = kl::Complex<float>;
	using Quaternion = kl::Quaternion<float>;

	using String = std::string;
	using StringStream = std::stringstream;

	using Exception = std::exception;
	using Error = std::error_code;

	using Any = std::any;

	template<typename T>
	using Ref = kl::Ref<T>;

	template<typename T>
	using Optional = std::optional<T>;

	template<typename T>
	using Function = std::function<T>;

	template<typename F, typename S>
	using Pair = std::pair<F, S>;

	template<typename T>
	using Vector = std::vector<T>;

	template<typename T>
	using List = std::list<T>;

	template<typename K>
	using Set = std::unordered_set<K>;
	
	template<typename K, typename V>
	using Map = std::unordered_map<K, V>;
}
