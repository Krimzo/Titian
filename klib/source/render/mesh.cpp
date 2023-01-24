#include "render/mesh.h"


kl::mesh::mesh()
{}

kl::mesh::mesh(dx::buffer mesh)
{
	push_back(mesh);
}

kl::mesh::mesh(const std::vector<dx::buffer>& parts)
{
	insert(end(), parts.begin(), parts.end());
}
