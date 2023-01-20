#include "rendering.h"

#include "callbacks/callbacks.h"


int rendering_main()
{
	// Setup
	state_machine state = {};
	state.window = new kl::window({ 1600, 900 }, "Fun");
	state.gpu = new kl::gpu(state.window->get_window());
	setup_callback(&state);

	// Loop
	state.timer.reset();
	while (state.window->process(false)) {
		state.timer.update_interval();
		update_callback(&state);
	}
	
	// End
	return 0;
}
