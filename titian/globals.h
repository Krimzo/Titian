#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <PxPhysicsAPI.h>
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
namespace px = physx;
namespace sl = sol;
namespace as = Assimp;

namespace titian
{
using byte = uint8_t;
using str = const char*;

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

namespace titian
{
inline constexpr str _CONF_FILE = "titian.json";
inline constexpr str CONF_EXE_TYPE = "exe_type";
inline constexpr str CONF_ENTRY_SCENE = "entry_scene";
inline constexpr str CONF_SPECIAL_COLOR = "special_color";
inline constexpr str CONF_ALTERNATE_COLOR = "alternate_color";
} // namespace titian

namespace titian
{
inline js::Object _conf_data{kl::read_file_string(_CONF_FILE)};
}
