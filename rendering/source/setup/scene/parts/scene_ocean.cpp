#include "setup/scene/setup_scene.h"


static constexpr float size_in_meters = 100.0f;
static constexpr float points_per_meter = 15.0f;

void setup_ocean(state_machine* state)
{
	const int number_of_points = (int) (size_in_meters * points_per_meter);
	state->scene->ocean_mesh = state->gpu->generate_plane_mesh(size_in_meters, number_of_points);
}
