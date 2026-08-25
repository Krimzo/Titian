#include "titian.h"


titian::Float3& titian::px_cast( px::PxVec3& vec )
{
    return reinterpret_cast<Float3&>(vec);
}

titian::Float3 const& titian::px_cast( px::PxVec3 const& vec )
{
    return reinterpret_cast<Float3 const&>(vec);
}

titian::Float4& titian::px_cast( px::PxQuat& quat )
{
    return reinterpret_cast<Float4&>(quat);
}

titian::Float4 const& titian::px_cast( px::PxQuat const& quat )
{
    return reinterpret_cast<Float4 const&>(quat);
}

px::PxVec3& titian::px_cast( Float3& vec )
{
    return reinterpret_cast<px::PxVec3&>(vec);
}

px::PxVec3 const& titian::px_cast( Float3 const& vec )
{
    return reinterpret_cast<px::PxVec3 const&>(vec);
}

px::PxQuat& titian::px_cast( Float4& quat )
{
    return reinterpret_cast<px::PxQuat&>(quat);
}

px::PxQuat const& titian::px_cast( Float4 const& quat )
{
    return reinterpret_cast<px::PxQuat const&>(quat);
}
