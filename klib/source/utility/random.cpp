#include "utility/random.h"

#include <sstream>


namespace kl_ignore {
    [[maybe_unused]] static const int dont_care = []
    {
        srand((uint32_t) time(nullptr));
        return 0;
    }();
}

// Byte-s
bool kl::random::get_bool()
{
    return rand() % 2;
}

uint8_t kl::random::get_byte()
{
    return (uint8_t) (rand() % 256);
}

kl::color kl::random::get_color(const bool gray)
{
    if (gray) {
        const uint8_t rand_gray = get_byte();
        return { rand_gray, rand_gray, rand_gray };
    }
    return { get_byte(), get_byte(), get_byte() };
}

// Int-s
int kl::random::get_int(const int start_inclusive, const int end_exclusive)
{
    return rand() % (end_exclusive - start_inclusive) + start_inclusive;
}

int kl::random::get_int(const int end_exclusive)
{
    return get_int(0, end_exclusive);
}

kl::int2 kl::random::get_int2(const int start_inclusive, const int end_exclusive)
{
    return { get_int(start_inclusive, end_exclusive), get_int(start_inclusive, end_exclusive) };
}

kl::int2 kl::random::get_int2(const int end_exclusive)
{
    return { get_int(end_exclusive), get_int(end_exclusive) };
}

kl::int3 kl::random::get_int3(const int start_inclusive, const int end_exclusive)
{
    return { get_int(start_inclusive, end_exclusive), get_int(start_inclusive, end_exclusive), get_int(start_inclusive, end_exclusive) };
}

kl::int3 kl::random::get_int3(const int end_exclusive)
{
    return { get_int(end_exclusive), get_int(end_exclusive), get_int(end_exclusive) };
}

kl::int4 kl::random::get_int4(const int start_inclusive, const int end_exclusive)
{
    return { get_int(start_inclusive, end_exclusive), get_int(start_inclusive, end_exclusive), get_int(start_inclusive, end_exclusive), get_int(start_inclusive, end_exclusive) };
}

kl::int4 kl::random::get_int4(const int end_exclusive)
{
    return { get_int(end_exclusive), get_int(end_exclusive), get_int(end_exclusive), get_int(end_exclusive) };
}

// Float-s
float kl::random::get_float(const float start_inclusive, const float end_inclusive)
{
    return get_float(end_inclusive - start_inclusive) + start_inclusive;
}

float kl::random::get_float(const float end_inclusive)
{
    return get_float() * end_inclusive;
}

float kl::random::get_float()
{
    return ((float) rand()) / RAND_MAX;
}

kl::float2 kl::random::get_float2(const float start_inclusive, const float end_inclusive)
{
    return { get_float(start_inclusive, end_inclusive), get_float(start_inclusive, end_inclusive) };
}

kl::float2 kl::random::get_float2(const float end_inclusive)
{
    return { get_float(end_inclusive), get_float(end_inclusive) };
}

kl::float2 kl::random::get_float2()
{
    return { get_float(), get_float() };
}

kl::float3 kl::random::get_float3(const float start_inclusive, const float end_inclusive)
{
    return { get_float(start_inclusive, end_inclusive), get_float(start_inclusive, end_inclusive), get_float(start_inclusive, end_inclusive) };
}

kl::float3 kl::random::get_float3(const float end_inclusive)
{
    return { get_float(end_inclusive), get_float(end_inclusive), get_float(end_inclusive) };
}

kl::float3 kl::random::get_float3()
{
    return { get_float(), get_float(), get_float() };
}

kl::float4 kl::random::get_float4(const float start_inclusive, const float end_inclusive)
{
    return { get_float(start_inclusive, end_inclusive), get_float(start_inclusive, end_inclusive), get_float(start_inclusive, end_inclusive), get_float(start_inclusive, end_inclusive) };
}

kl::float4 kl::random::get_float4(const float end_inclusive)
{
    return { get_float(end_inclusive), get_float(end_inclusive), get_float(end_inclusive), get_float(end_inclusive) };
}

kl::float4 kl::random::get_float4()
{
    return { get_float(), get_float(), get_float(), get_float() };
}

// Mat-s
kl::mat3 kl::random::get_mat3(const float start_inclusive, const float end_inclusive)
{
    kl::mat3 result = {};
    for (auto& value : result) {
        value = get_float(start_inclusive, end_inclusive);
    }
    return result;
}

kl::mat3 kl::random::get_mat3(const float end_inclusive)
{
    kl::mat3 result = {};
    for (auto& value : result) {
        value = get_float(end_inclusive);
    }
    return result;
}

kl::mat3 kl::random::get_mat3()
{
    kl::mat3 result = {};
    for (auto& value : result) {
        value = get_float();
    }
    return result;
}

kl::mat4 kl::random::get_mat4(const float start_inclusive, const float end_inclusive)
{
    kl::mat4 result = {};
    for (auto& value : result) {
        value = get_float(start_inclusive, end_inclusive);
    }
    return result;
}

kl::mat4 kl::random::get_mat4(const float end_inclusive)
{
    kl::mat4 result = {};
    for (auto& value : result) {
        value = get_float(end_inclusive);
    }
    return result;
}

kl::mat4 kl::random::get_mat4()
{
    kl::mat4 result = {};
    for (auto& value : result) {
        value = get_float();
    }
    return result;
}

// Char-s
char kl::random::get_char(const bool upper)
{
    return upper ? char(get_int('A', 'Z' + 1)) : char(get_int('a', 'z' + 1));
}

std::string kl::random::get_string(const int length)
{
    std::stringstream stream;
    for (int i = 0; i < length; i++) {
        stream << get_char(get_bool());
    }
    return stream.str();
}
