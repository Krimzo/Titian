#include "titian.h"


titian::DepthStates::DepthStates()
{
    kl::GPU& gpu = AppLayer::get().gpu;

    disabled = gpu.create_depth_state( false );
    enabled = gpu.create_depth_state( true );

    dx::DepthStateDescriptor descriptor{};
    descriptor.DepthEnable = true;
    descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    descriptor.DepthFunc = D3D11_COMPARISON_LESS;
    only_compare = gpu.create_depth_state( &descriptor );

    ti_assert( disabled, "Failed to inti DISABLED depth state." );
    ti_assert( enabled, "Failed to init ENABLED depth state." );
    ti_assert( only_compare, "Failed to init ONLY_COMPARE depth state." );
}
