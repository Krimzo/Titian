#include "main.h"


titian::DepthStates::DepthStates(kl::GPU* gpu)
{
    disabled = gpu->create_depth_state(false);
    enabled = gpu->create_depth_state(true);

    kl::assert(disabled, "Failed to inti DISABLED depth state.");
    kl::assert(enabled, "Failed to init ENABLED depth state.");
}
