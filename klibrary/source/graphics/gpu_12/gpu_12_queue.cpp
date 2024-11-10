#include "klibrary.h"


kl::GPU12Queue::GPU12Queue()
{}

kl::GPU12Queue::GPU12Queue( dx12::CommandQueue const& queue )
    : queue( queue )
{}

void kl::GPU12Queue::execute( dx12::CommandList const& list ) const
{
    ID3D12CommandList* command_lists[1] = { list.get() };
    queue->ExecuteCommandLists( 1, command_lists );
}
