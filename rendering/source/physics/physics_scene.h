#pragma once

#include "klib.h"
#include "btBulletDynamicsCommon.h"


class physics_scene
{
	kl::ref<btDefaultCollisionConfiguration>    configuration_ = {};
	kl::ref<btCollisionDispatcher>                 dispatcher_ = {};
	kl::ref<btBroadphaseInterface>                 pair_cache_ = {};
	kl::ref<btSequentialImpulseConstraintSolver>       solver_ = {};
	kl::ref<btDiscreteDynamicsWorld>                    world_ = {};

public:
	physics_scene();
	~physics_scene();
};
