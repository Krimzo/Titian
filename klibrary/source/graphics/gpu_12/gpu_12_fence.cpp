#include "klibrary.h"


kl::GPU12Fence::GPU12Fence()
{}

kl::GPU12Fence::GPU12Fence(const dx12::Fence& fence)
	: fence(fence)
{}

void kl::GPU12Fence::signal_and_wait(const dx12::CommandQueue& queue)
{
	counter += 1;
	queue->Signal(fence.get(), counter) >> verify_result;
	fence->SetEventOnCompletion(counter, nullptr) >> verify_result;
}
