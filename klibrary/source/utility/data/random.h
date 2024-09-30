#pragma once

#include "math/math.h"


namespace kl::random {
    bool gen_bool();
    uint8_t gen_byte();
    RGB gen_rgb(bool gray = false);

    int gen_int(int start_inclusive, int end_exclusive);
    int gen_int(int end_exclusive);

    Int2 gen_int2(int start_inclusive, int end_exclusive);
    Int2 gen_int2(int end_exclusive);

    float gen_float(float start_inclusive, float end_inclusive);
    float gen_float(float end_inclusive);
    float gen_float();

    Float2 gen_float2(float start_inclusive, float end_inclusive);
    Float2 gen_float2(float end_inclusive);
    Float2 gen_float2();

    Float3 gen_float3(float start_inclusive, float end_inclusive);
    Float3 gen_float3(float end_inclusive);
    Float3 gen_float3();

    Float4 gen_float4(float start_inclusive, float end_inclusive);
    Float4 gen_float4(float end_inclusive);
    Float4 gen_float4();

    Float3x3 gen_float3x3(float start_inclusive, float end_inclusive);
    Float3x3 gen_float3x3(float end_inclusive);
    Float3x3 gen_float3x3();

    Float4x4 gen_float4x4(float start_inclusive, float end_inclusive);
    Float4x4 gen_float4x4(float end_inclusive);
    Float4x4 gen_float4x4();

    char gen_char(bool upper = false);
    std::string gen_string(int length, bool upper = false);
}
