#include "update/update_frame.h"


void custom_update(state_machine* state)
{
	if (!state->scene) { return; }

	const float rotation_speed = -36.0f;
	const float elapsed_t = state->timer.get_elapsed();

	for (auto& entity : *state->scene) {
		if (!entity->user_data) { continue; }

		kl::int3& user_data = *(kl::int3*)entity->user_data;
		if (user_data.z == -1) {
			const float z_rotation = rotation_speed * elapsed_t;
			const kl::float3 new_position = kl::float3(user_data.x, user_data.y, entity->position.z);

			entity->position = new_position.rotate(z_rotation, { 0, 0, 1 });
			entity->rotation.z = z_rotation;
		}
	}
}
