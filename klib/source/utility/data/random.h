#pragma once

#include "math/math.h"


namespace kl::random {
    // Byte
    bool get_bool();
    uint8_t get_byte();
    color get_color(bool gray = false);

    // Int
    int get_int(int start_inclusive, int end_exclusive);
    int get_int(int end_exclusive);

    int2 get_int2(int start_inclusive, int end_exclusive);
    int2 get_int2(int end_exclusive);

    // Float
    float get_float(float start_inclusive, float end_inclusive);
    float get_float(float end_inclusive);
    float get_float();

    float2 get_float2(float start_inclusive, float end_inclusive);
    float2 get_float2(float end_inclusive);
    float2 get_float2();

    float3 get_float3(float start_inclusive, float end_inclusive);
    float3 get_float3(float end_inclusive);
    float3 get_float3();

    float4 get_float4(float start_inclusive, float end_inclusive);
    float4 get_float4(float end_inclusive);
    float4 get_float4();

    float3x3 get_float3x3(float start_inclusive, float end_inclusive);
    float3x3 get_float3x3(float end_inclusive);
    float3x3 get_float3x3();

    float4x4 get_float4x4(float start_inclusive, float end_inclusive);
    float4x4 get_float4x4(float end_inclusive);
    float4x4 get_float4x4();

    // Char
    char get_char(bool upper = false);
    std::string get_string(int length);
}
