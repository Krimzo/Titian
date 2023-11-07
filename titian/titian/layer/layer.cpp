#include "main.h"


void titian::Layer::panic() const
{
	const std::string message = kl::format("Layer 0x", std::hex, (size_t)this, " panicked");
	kl::assert(false, message, false);
}
