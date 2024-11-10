#pragma once

#include "components/mesh.h"
#include "components/animation.h"
#include "components/material.h"
#include "utility/async_util.h"


namespace titian
{
struct DefaultMeshes
{
    Mesh cube;
    Mesh sphere;
    Mesh capsule;

    DefaultMeshes();
};
}

namespace titian
{
struct DefaultAnimations
{
    Animation cube;
    Animation sphere;
    Animation capsule;

    DefaultAnimations();
};
}

namespace titian
{
struct DefaultMaterials
{
    Material white;

    DefaultMaterials();
};
}
