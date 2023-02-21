#include "state/state_machine.h"


state_machine::state_machine()
{
    window = kl::make<kl::window>(kl::int2(1600, 900), "Fun");
    gpu = kl::make<kl::gpu>((HWND) *window);

    render_state = kl::make<::render_state>(gpu, window->size());
    scene = kl::make<kl::scene>();
}

state_machine::~state_machine()
{
    meshes.clear();
    materials.clear();
    scene = nullptr;
}
