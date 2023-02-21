#include "callbacks/main_callbacks.h"

#include "setup/setup_state.h"


void setup_callback(state_machine* state)
{
    state->window->maximize();

    // Render states
    setup_depth_states(state);
    setup_sampler_states(state);
    setup_raster_states(state);

    // Render buffers
    setup_cbuffers(state);
    setup_shaders(state);
    setup_textures(state);
    setup_skyboxes(state);

    // Render components
    setup_meshes(state);
    setup_materials(state);

    // Scene
    setup_scene(state);

    // GUI
    setup_gui(state);
}
