#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include "physx.h"
#include "assimp.h"
#include "sol.hpp"
#include "klibrary.h"

namespace fs = std::filesystem;
namespace dx = kl::dx;
namespace js = kl::json;
namespace im = imgui;
namespace px = physx;
namespace sl = sol;
namespace as = Assimp;

namespace titian
{
using byte = uint8_t;
using str = const char*;

using Int2 = kl::Int2;
using Int3 = kl::Int3;
using Int4 = kl::Int4;

using Float2 = kl::Float2;
using Float3 = kl::Float3;
using Float4 = kl::Float4;

using Float2x2 = kl::Float2x2;
using Float3x3 = kl::Float3x3;
using Float4x4 = kl::Float4x4;

using Complex = kl::Complex;
using Quaternion = kl::Quaternion;

using RGB = kl::RGB;
using YUV = kl::YUV;

using String = std::string;
using StringRef = std::string_view;
using StringStream = std::stringstream;

using Except = std::exception;
using Error = std::error_code;

using Any = std::any;

template<typename T>
using Ref = kl::Ref<T>;

template<typename T>
using Opt = std::optional<T>;

template<typename T>
using Func = std::function<T>;

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
