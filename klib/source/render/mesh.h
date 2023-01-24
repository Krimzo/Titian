#pragma once

#include "graphics/dx_types.h"

#include <vector>


namespace kl {
	class mesh : public std::vector<dx::buffer>
	{
	public:
		mesh();
		mesh(dx::buffer mesh);
		mesh(const std::vector<dx::buffer>& parts);
	};
}
