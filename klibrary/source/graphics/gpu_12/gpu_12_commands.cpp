#include "klibrary.h"


kl::GPU12Commands::GPU12Commands()
{}

kl::GPU12Commands::GPU12Commands(const dx12::CommandAllocator& allocator)
	: allocator(allocator)
{}

kl::GPU12Commands::GPU12Commands(const dx12::CommandAllocator& allocator, const dx12::CommandList& list)
	: allocator(allocator), list(list)
{}

void kl::GPU12Commands::reset() const
{
	allocator->Reset() >> verify_result;
	list->Reset(allocator.get(), nullptr) >> verify_result;
}

void kl::GPU12Commands::close() const
{
	list->Close() >> verify_result;
}

void kl::GPU12Commands::transition_resource(const dx12::Resource& resource, const D3D12_RESOURCE_STATES from, const D3D12_RESOURCE_STATES to) const
{
	const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.get(), from, to);
	list->ResourceBarrier(1, &barrier);
}

void kl::GPU12Commands::clear_target_view(const dx12::DescriptorHandle& handle, const Float4& color) const
{
	list->ClearRenderTargetView(handle, &color.x, 0, nullptr);
}

void kl::GPU12Commands::copy(dx12::Resource& destination, const dx12::Resource& source) const
{
	list->CopyResource(destination.get(), source.get());
}

void kl::GPU12Commands::set_root_signature(const dx12::RootSignature& root_signature) const
{
	list->SetGraphicsRootSignature(root_signature.get());
}

void kl::GPU12Commands::set_pipeline_state(const dx12::PipelineState& pipeline_state) const
{
	list->SetPipelineState(pipeline_state.get());
}

void kl::GPU12Commands::set_primitive_topology(const D3D_PRIMITIVE_TOPOLOGY topology) const
{
	list->IASetPrimitiveTopology(topology);
}

void kl::GPU12Commands::set_vertex_buffer(const dx12::VertexBuffer& vertex_buffer) const
{
	list->IASetVertexBuffers(0, 1, &vertex_buffer);
}

void kl::GPU12Commands::set_scissors(const dx12::Scissors& scissors) const
{
	list->RSSetScissorRects(1, &scissors);
}

void kl::GPU12Commands::set_viewport(const dx12::Viewport& viewport) const
{
	list->RSSetViewports(1, &viewport);
}

void kl::GPU12Commands::set_render_target(const dx12::DescriptorHandle* render_target, const dx12::DescriptorHandle* depth_target) const
{
	list->OMSetRenderTargets(1, render_target, true, depth_target);
}

void kl::GPU12Commands::draw(const UINT vertex_count, const UINT instance_count) const
{
	list->DrawInstanced(vertex_count, instance_count, 0, 0);
}
