#pragma once

#include "tracing.h"

namespace titian
{
struct CPUTracingTask
{
    std::future<void> task;
    Pair<int2, int2> section;
};
} // namespace titian

namespace titian
{
struct CPUTracer
{
    kl::Window& window;
    TracingScene const& scene;

    CPUTracer(kl::Window& window, TracingScene const& scene, int2 resolution);

    void render();

  private:
    Vector<CPUTracingTask> m_tasks;
    kl::Image m_target;

    void handle_input();
    void make_frame(kl::Image& frame);

    void render_scene();
    void render_section(int2 top_left, int2 size);
    RGB render_pixel(float2 ndc);

    float3 trace_ray(kl::Ray const& ray, int depth, kl::Triangle const* blacklist);

    float3 align_to_normal(float3 const& sample, float3 const& normal);
    float3 rand_float3(float3 const& normal);
};
} // namespace titian
