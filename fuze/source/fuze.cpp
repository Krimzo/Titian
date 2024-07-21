#include "fuze.h"


int fuze::fuze_entry(int argc, const char** argv)
{
	Ref layer_stack = new FuzeEditor();
	while (layer_stack->process_layers());
	return 0;
}
