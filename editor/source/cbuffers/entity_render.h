#pragma once

#include "klib.h"


struct entity_render_vs_cb
{
    kl::float4x4  w_matrix;
    kl::float4x4 vp_matrix;

    kl::float4x4 vp_light_matrices[kl::directional_light::CASCADE_COUNT];
};

struct entity_render_ps_cb
{
    kl::float4 object_color; // (color.r, color.g, color.b, none)
    kl::float4 object_index; // (index, index, index, index)

    kl::float4     object_material; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
    kl::float4 object_texture_info; // (has_normal_map, has_roughness_map, none, none)

    kl::float4 camera_info; // (camera.x, camera.y, camera.z, none)
    kl::float4x4  v_matrix; // View matrix

    kl::float4     ambient_light; // (color.r, color.g, color.b, intensity)
    kl::float4 directional_light; // (sun.x, sun.y, sun.z, sun_point_size)

    kl::float4   shadow_map_info; // (width, height, texel_width, texel_size)
    kl::float4 cascade_distances; // (cascade_0_far, cascade_1_far, cascade_2_far, cascade_3_far)
};
