#include "main.h"


titian::DefaultMaterials::DefaultMaterials(kl::GPU* gpu)
{
	white = new Material(gpu);
}
