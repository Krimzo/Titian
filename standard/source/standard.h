#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include "klibrary.h"
#include "physx.h"
#include "sol.hpp"
#include "assimp.h"

#undef min
#undef max
#undef assert
#undef VOID
#undef INFINITY

namespace fs = std::filesystem;
namespace dx = kl::dx;
namespace js = kl::json;
namespace im = imgui;
namespace px = physx;
namespace sl = sol;
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

	using Color = kl::Color;

	using String = std::string;
	using StringView = std::string_view;
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
		requires (not std::is_same_v<K, String>)
	using Set = std::unordered_set<K>;

	using StringSet = std::unordered_set<String, kl::string_hash, std::equal_to<>>;

	template<typename K, typename V>
		requires (not std::is_same_v<K, String>)
	using Map = std::unordered_map<K, V>;

	template<typename V>
	using StringMap = std::unordered_map<String, V, kl::string_hash, std::equal_to<>>;
}
