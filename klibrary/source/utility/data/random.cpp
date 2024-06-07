#include "klibrary.h"


// Init
namespace kl_ignore {
    [[maybe_unused]] static const int DONT_CARE = []
    {
        std::srand(std::random_device{}());
        return 0;
    }();
}

namespace kl {
    static thread_local std::mt19937 ENGINE{ std::random_device{}() };
}

// Byte
bool kl::random::gen_bool()
{
    return static_cast<bool>(ENGINE() % 2);
}

byte kl::random::gen_byte()
{
    return static_cast<byte>(ENGINE() % 256);
}

kl::Color kl::random::gen_color(const bool gray)
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
    return start_inclusive + (ENGINE() % (end_exclusive - start_inclusive));
}

int kl::random::gen_int(const int end_exclusive)
{
    return gen_int(0, end_exclusive);
}

kl::Int2 kl::random::gen_int2(const int start_inclusive, const int end_exclusive)
{
    return { gen_int(start_inclusive, end_exclusive), gen_int(start_inclusive, end_exclusive) };
}

kl::Int2 kl::random::gen_int2(const int end_exclusive)
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
    return static_cast<float>(ENGINE()) / std::numeric_limits<uint32_t>::max();
}

kl::Float2 kl::random::gen_float2(const float start_inclusive, const float end_inclusive)
{
    return { gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive) };
}

kl::Float2 kl::random::gen_float2(const float end_inclusive)
{
    return { gen_float(end_inclusive), gen_float(end_inclusive) };
}

kl::Float2 kl::random::gen_float2()
{
    return { gen_float(), gen_float() };
}

kl::Float3 kl::random::gen_float3(const float start_inclusive, const float end_inclusive)
{
    return { gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive) };
}

kl::Float3 kl::random::gen_float3(const float end_inclusive)
{
    return { gen_float(end_inclusive), gen_float(end_inclusive), gen_float(end_inclusive) };
}

kl::Float3 kl::random::gen_float3()
{
    return { gen_float(), gen_float(), gen_float() };
}

kl::Float4 kl::random::gen_float4(const float start_inclusive, const float end_inclusive)
{
    return { gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive), gen_float(start_inclusive, end_inclusive) };
}

kl::Float4 kl::random::gen_float4(const float end_inclusive)
{
    return { gen_float(end_inclusive), gen_float(end_inclusive), gen_float(end_inclusive), gen_float(end_inclusive) };
}

kl::Float4 kl::random::gen_float4()
{
    return { gen_float(), gen_float(), gen_float(), gen_float() };
}

kl::Float3x3 kl::random::gen_float3x3(const float start_inclusive, const float end_inclusive)
{
    Float3x3 result;
    for (auto& value : result.data) {
        value = gen_float(start_inclusive, end_inclusive);
    }
    return result;
}

kl::Float3x3 kl::random::gen_float3x3(const float end_inclusive)
{
    Float3x3 result;
    for (auto& value : result.data) {
        value = gen_float(end_inclusive);
    }
    return result;
}

kl::Float3x3 kl::random::gen_float3x3()
{
    Float3x3 result;
    for (auto& value : result.data) {
        value = gen_float();
    }
    return result;
}

kl::Float4x4 kl::random::gen_float4x4(const float start_inclusive, const float end_inclusive)
{
    Float4x4 result;
    for (auto& value : result.data) {
        value = gen_float(start_inclusive, end_inclusive);
    }
    return result;
}

kl::Float4x4 kl::random::gen_float4x4(const float end_inclusive)
{
    Float4x4 result;
    for (auto& value : result.data) {
        value = gen_float(end_inclusive);
    }
    return result;
}

kl::Float4x4 kl::random::gen_float4x4()
{
    Float4x4 result;
    for (auto& value : result.data) {
        value = gen_float();
    }
    return result;
}

// Char
char kl::random::gen_char(const bool upper)
{
    if (upper) {
        return static_cast<char>(gen_int('A', 'Z' + 1));
    }
    return static_cast<char>(gen_int('a', 'z' + 1));
}

std::string kl::random::gen_string(const int length, const bool upper)
{
    std::string result;
    result.resize(length);
    for (auto& value : result) {
        value = gen_char(upper);
    }
    return result;
}
