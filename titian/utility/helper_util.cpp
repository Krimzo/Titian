#include "titian.h"

float3& titian::px_cast(px::PxVec3& vec)
{
    return reinterpret_cast<float3&>(vec);
}

float3 const& titian::px_cast(px::PxVec3 const& vec)
{
    return reinterpret_cast<float3 const&>(vec);
}

float4& titian::px_cast(px::PxQuat& quat)
{
    return reinterpret_cast<float4&>(quat);
}

float4 const& titian::px_cast(px::PxQuat const& quat)
{
    return reinterpret_cast<float4 const&>(quat);
}

px::PxVec3& titian::px_cast(float3& vec)
{
    return reinterpret_cast<px::PxVec3&>(vec);
}

px::PxVec3 const& titian::px_cast(float3 const& vec)
{
    return reinterpret_cast<px::PxVec3 const&>(vec);
}

px::PxQuat& titian::px_cast(float4& quat)
{
    return reinterpret_cast<px::PxQuat&>(quat);
}

px::PxQuat const& titian::px_cast(float4 const& quat)
{
    return reinterpret_cast<px::PxQuat const&>(quat);
}
