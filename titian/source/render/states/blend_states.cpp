#include "titian.h"


void titian::BlendStates::init(kl::GPU& gpu)
{
	disabled = gpu.create_blend_state(false);
	enabled = gpu.create_blend_state(true);

	kl::assert(disabled, "Failed to inti DISABLED blend state.");
	kl::assert(enabled, "Failed to init ENABLED blend state.");
}
