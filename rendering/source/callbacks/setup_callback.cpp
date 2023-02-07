#include "callbacks/main_callbacks.h"

#include "setup/setup_state.h"


void setup_callback(state_machine* state)
{
    state->window->maximize();

    // States
    setup_depth_states(state);
    setup_sampler_states(state);
    setup_raster_states(state);

    // Buffers
    setup_shaders(state);
    setup_meshes(state);
    setup_textures(state);
    setup_skyboxes(state);

    // Scene
    setup_scene(state);

    // GUI
    setup_gui(state);
}
