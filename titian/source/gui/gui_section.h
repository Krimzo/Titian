#pragma once

#include "utility/gui_util.h"
#include "utility/bench_util.h"


namespace titian
{
struct GUISection : kl::NoCopy, BenchmarkInfo
{
    GUISection( StringRef const& name );
    virtual ~GUISection() = default;

    virtual void render_gui() = 0;
};
}
