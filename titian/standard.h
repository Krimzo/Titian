#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <sol/sol.hpp>
#include <klibrary.h>
#include <imgui_memory_editor.h>
#include <imgui_text_editor.h>
#include <ImNodeFlow.h>
#include <ImGuizmo.h>

namespace fs = std::filesystem;
namespace dx = kl::dx;
namespace js = kl::json;
namespace im = imgui;
namespace jl = JPH;
namespace sl = sol;
namespace as = Assimp;

namespace titian
{
using byte = uint8_t;
using str = const char*;

using Int2 = int2;
using Int3 = int3;
using Int4 = int4;

using Float2 = float2;
using Float3 = float3;
using Float4 = float4;

using Float2x2 = float2x2;
using Float3x3 = float3x3;
using Float4x4 = float4x4;

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

template <typename T> using Ref = kl::Ref<T>;

template <typename T> using Opt = std::optional<T>;

template <typename T> using Func = std::function<T>;

template <typename F, typename S> using Pair = std::pair<F, S>;

template <typename T> using Vector = std::vector<T>;

template <typename T> using List = std::list<T>;

template <typename K>
    requires(not std::is_same_v<K, String>)
using Set = std::unordered_set<K>;

using StringSet = std::set<String, std::less<>>;

template <typename K, typename V>
    requires(not std::is_same_v<K, String>)
using Map = std::unordered_map<K, V>;

template <typename V> using StringMap = std::map<String, V, std::less<>>;
} // namespace titian
