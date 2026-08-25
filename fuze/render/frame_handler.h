#pragma once

#include "storage/frame.h"


namespace titian
{
struct FrameHandler
{
    Frame out_frame;

    FrameHandler();

    void prepare_frame( Int2 size );
    void mix_frame( Frame const& frame );

private:
    dx::ComputeShader m_clear_shader;
    dx::ComputeShader m_mix_shader;
};
}
