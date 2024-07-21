#pragma once

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include "assimp/quaternion.h"
#include "assimp/scene.h"

#ifdef _DEBUG

#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "zlibstaticd.lib")

#else

#pragma comment(lib, "assimp-vc143-mt.lib")
#pragma comment(lib, "zlibstatic.lib")

#endif
