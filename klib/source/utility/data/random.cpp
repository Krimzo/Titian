#include "klib.h"


// Init
namespace kl_ignore {
    static const int dont_care = []
    {
        srand((uint32_t) time(nullptr));
        return 0;
    }();
}

// Byte
bool kl::random::gen_bool()
{
    return rand() % 2;
}

byte kl::random::gen_byte()
{
    return (byte) (rand() % 256);
}

kl::color kl::random::gen_color(const bool gray)
{
    if (gray) {
        const byte rand_gray = gen_byte();
        return { rand_gray, rand_gray, rand_gray };
    }
    return { gen_byte(), gen_byte(), gen_byte() };
}

// Int
int kl::random::gen_int(const int start_inclusive, const int end_exclusive)
{
    return rand() % (end_exclusive - start_inclusive) + start_inclusive;
}

int kl::random::gen_int(const int end_exclusive)
{
    return gen_int(0, end_exclusive);
}

kl::int2 kl::random::gen_int2(const int start_inclusive, const int end_exclusive)
{
    return { gen_int(start_inclusive, end_exclusive), gen_int(start_inclusive, end_exclusive) };
}

kl::int2 kl::random::gen_int2(const int end_exclusive)
{
    return { gen_int(end_exclusive), gen_int(end_exclusive) };
}

// Float
float kl::random::gen_float(const float start_inclusive, const float end_inclusive)
{
    return gen_float(end_inclusive - start_inclusive) + start_inclusive;
}

float kl::random::gen_float(const float end_inclusive)
{
    return gen_float() * end_inclusive;
}

float kl::random::gen_float()
{
    return ((float) rand()) / RAND_MAX;
}

kl::float2 kl::random::gen_float2(const float start_inclusive, const float end_inclusive)
{
    return { gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive) };
}

kl::float2 kl::random::gen_float2(const float end_inclusive)
{
    return { gen_float(end_inclusive), gen_float(end_inclusive) };
}

kl::float2 kl::random::gen_float2()
{
    return { gen_float(), gen_float() };
}

kl::float3 kl::random::gen_float3(const float start_inclusive, const float end_inclusive)
{
    return { gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive) };
}

kl::float3 kl::random::gen_float3(const float end_inclusive)
{
    return { gen_float(end_inclusive), gen_float(end_inclusive), gen_float(end_inclusive) };
}

kl::float3 kl::random::gen_float3()
{
    return { gen_float(), gen_float(), gen_float() };
}

kl::float4 kl::random::gen_float4(const float start_inclusive, const float end_inclusive)
{
    return { gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive) };
}

kl::float4 kl::random::gen_float4(const float end_inclusive)
{
    return { gen_float(end_inclusive), gen_float(end_inclusive), gen_float(end_inclusive), gen_float(end_inclusive) };
}

kl::float4 kl::random::gen_float4()
{
    return { gen_float(), gen_float(), gen_float(), gen_float() };
}

kl::float3x3 kl::random::gen_float3x3(const float start_inclusive, const float end_inclusive)
{
    float3x3 result = {};
    for (auto& value : result.data) {
        value = gen_float(start_inclusive, end_inclusive);
    }
    return result;
}

kl::float3x3 kl::random::gen_float3x3(const float end_inclusive)
{
    float3x3 result = {};
    for (auto& value : result.data) {
        value = gen_float(end_inclusive);
    }
    return result;
}

kl::float3x3 kl::random::gen_float3x3()
{
    float3x3 result = {};
    for (auto& value : result.data) {
        value = gen_float();
    }
    return result;
}

kl::float4x4 kl::random::gen_float4x4(const float start_inclusive, const float end_inclusive)
{
    float4x4 result = {};
    for (auto& value : result.data) {
        value = gen_float(start_inclusive, end_inclusive);
    }
    return result;
}

kl::float4x4 kl::random::gen_float4x4(const float end_inclusive)
{
    float4x4 result = {};
    for (auto& value : result.data) {
        value = gen_float(end_inclusive);
    }
    return result;
}

kl::float4x4 kl::random::gen_float4x4()
{
    float4x4 result = {};
    for (auto& value : result.data) {
        value = gen_float();
    }
    return result;
}

// Char
char kl::random::gen_char(const bool upper)
{
    return upper ? char(gen_int('A', 'Z' + 1)) : char(gen_int('a', 'z' + 1));
}

std::string kl::random::gen_string(const int length)
{
    std::stringstream stream = {};
    for (int i = 0; i < length; i++) {
        stream << gen_char(gen_bool());
    }
    return stream.str();
}
