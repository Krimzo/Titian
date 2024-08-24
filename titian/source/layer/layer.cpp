#include "titian.h"


titian::Layer::Layer(const StringView& name)
	: BenchmarkInfo(name)
{}

void titian::Layer::panic() const
{
	kl::assert(false, "Layer 0x", std::hex, (size_t) this, " panicked");
}
