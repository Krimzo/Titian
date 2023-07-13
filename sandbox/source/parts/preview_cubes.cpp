#include "sandbox.h"


static constexpr float x_offset = 16.0f;

void setup_preview_cubes(editor_state* state, const int size)
{
    const int half_size = size / 2;

    int cube_counter = 0;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            auto cube = state->scene->make_entity(false);

            cube->render_scale = kl::float3(0.45f);

            cube->set_position({
                (float) (x - half_size) + x_offset,
                (float) (y - half_size),
                5.0f,
            });

            cube->mesh = state->default_meshes["cube"];

            cube->material = new kl::material();
            cube->material->color = kl::colors::orange;
            cube->material->reflection_factor = (float) cube_counter / (size * size);

            state->scene->add(kl::format("Cube", cube_counter), cube);

            cube_counter += 1;
        }
    }
}
