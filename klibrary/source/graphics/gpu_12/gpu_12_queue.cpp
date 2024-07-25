#include "klibrary.h"


kl::GPU12Queue::GPU12Queue()
{}

kl::GPU12Queue::GPU12Queue(const dx12::CommandQueue& queue)
	: queue(queue)
{}

void kl::GPU12Queue::execute(const dx12::CommandList& list) const
{
	ID3D12CommandList* command_lists[1] = { list.get() };
	queue->ExecuteCommandLists(1, command_lists);
}
