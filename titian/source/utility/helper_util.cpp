#include "titian.h"


titian::Float3& titian::px_cast(px::PxVec3& vec)
{
	return reinterpret_cast<Float3&>(vec);
}

const titian::Float3& titian::px_cast(const px::PxVec3& vec)
{
	return reinterpret_cast<const Float3&>(vec);
}

titian::Float4& titian::px_cast(px::PxQuat& quat)
{
	return reinterpret_cast<Float4&>(quat);
}

const titian::Float4& titian::px_cast(const px::PxQuat& quat)
{
	return reinterpret_cast<const Float4&>(quat);
}

px::PxVec3& titian::px_cast(Float3& vec)
{
	return reinterpret_cast<px::PxVec3&>(vec);
}

const px::PxVec3& titian::px_cast(const Float3& vec)
{
	return reinterpret_cast<const px::PxVec3&>(vec);
}

px::PxQuat& titian::px_cast(Float4& quat)
{
	return reinterpret_cast<px::PxQuat&>(quat);
}

const px::PxQuat& titian::px_cast(const Float4& quat)
{
	return reinterpret_cast<const px::PxQuat&>(quat);
}
