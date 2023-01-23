#include "update/update_frame.h"


void update_light(state_machine* state)
{
	if (!state->scene) { return; }

	//const float elapsed_time = state->timer.get_elapsed();
	//state->scene->sun_direction.x = cos(elapsed_time);
	//state->scene->sun_direction.z = sin(elapsed_time);

	//state->scene->sun_direction = state->scene->camera.get_forward();
}
