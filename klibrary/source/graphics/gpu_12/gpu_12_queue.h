#pragma once

#include "apis/apis.h"


namespace kl
{
struct GPU12Queue
{
    dx12::CommandQueue queue{};

    GPU12Queue();
    GPU12Queue( dx12::CommandQueue const& queue );

    void execute( dx12::CommandList const& list ) const;
};
}
