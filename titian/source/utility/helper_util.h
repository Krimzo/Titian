#pragma once

#include "standard.h"


namespace titian
{
Float3& px_cast( px::PxVec3& vec );
Float3 const& px_cast( px::PxVec3 const& vec );

Float4& px_cast( px::PxQuat& quat );
Float4 const& px_cast( px::PxQuat const& quat );

px::PxVec3& px_cast( Float3& vec );
px::PxVec3 const& px_cast( Float3 const& vec );

px::PxQuat& px_cast( Float4& quat );
px::PxQuat const& px_cast( Float4 const& quat );
}
