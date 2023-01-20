#pragma once

#include "render/plane.h"
#include "render/sphere.h"
#include "render/triangle.h"
#include "render/camera.h"


namespace kl {
    struct ray
    {
        float3 origin = {};
        float3 direction = {};

        ray();
        ray(const float3& origin, const float3& direction);
        ray(const float3& origin, const mat4& inverse_matrix, const float2& ndc);
        ray(const camera& camera, const float2& ndc);

        bool intersect_plane(const plane& plane, float3* out_intersection = nullptr) const;

        bool intersect_triangle(const triangle& triangle, float3* out_intersection = nullptr) const;

        bool intersect_sphere(const sphere& sphere, float3& out_intersection, float* out_distance = nullptr) const;
        [[nodiscard]] bool intersect_sphere(const sphere& sphere) const;
    };

    std::ostream& operator<<(std::ostream& os, const ray& obj);
}
