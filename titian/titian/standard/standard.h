#pragma once

#include "klibrary.h"
#include "physx.h"
#include "chaiscript.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include "assimp/quaternion.h"
#include "assimp/scene.h"


namespace fs = std::filesystem;
namespace im = imgui;
namespace px = physx;
namespace cs = chaiscript;
namespace as = Assimp;

namespace titian {
	using String = std::string;
	using StringStream = std::stringstream;

	using Exception = std::exception;
	using Error = std::error_code;

	template<typename T>
	using Ref = kl::Object<T>;

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

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "zlibstaticd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#pragma comment(lib, "zlibstatic.lib")
#endif
