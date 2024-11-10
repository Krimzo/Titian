#include "klibrary.h"


kl::GPU12Fence::GPU12Fence()
{}

kl::GPU12Fence::GPU12Fence( dx12::Fence const& fence )
    : fence( fence )
{}

void kl::GPU12Fence::signal_and_wait( dx12::CommandQueue const& queue )
{
    counter += 1;
    queue->Signal( fence.get(), counter ) >> verify_result;
    fence->SetEventOnCompletion( counter, nullptr ) >> verify_result;
}
