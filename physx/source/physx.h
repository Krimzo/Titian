#pragma once

#include "PxPhysicsAPI.h"

#ifdef _DEBUG

#pragma comment(lib, "PhysX_static_64d.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64d.lib")
#pragma comment(lib, "PhysXCommon_static_64d.lib")
#pragma comment(lib, "PhysXCooking_static_64d.lib")
#pragma comment(lib, "PhysXExtensions_static_64d.lib")
#pragma comment(lib, "PhysXFoundation_static_64d.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64d.lib")
#pragma comment(lib, "PhysXVehicle_static_64d.lib")

#else

#pragma comment(lib, "PhysX_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysXCommon_static_64.lib")
#pragma comment(lib, "PhysXCooking_static_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXFoundation_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXVehicle_static_64.lib")

#endif
