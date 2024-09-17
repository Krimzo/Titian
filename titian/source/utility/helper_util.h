#pragma once

#include "standard.h"


namespace titian {
	Float3& px_cast(px::PxVec3& vec);
	const Float3& px_cast(const px::PxVec3& vec);

	Float4& px_cast(px::PxQuat& quat);
	const Float4& px_cast(const px::PxQuat& quat);

	px::PxVec3& px_cast(Float3& vec);
	const px::PxVec3& px_cast(const Float3& vec);

	px::PxQuat& px_cast(Float4& quat);
	const px::PxQuat& px_cast(const Float4& quat);
}
