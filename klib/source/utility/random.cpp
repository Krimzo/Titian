#include "utility/random.h"

#include <sstream>


namespace kl_ignore {
    [[maybe_unused]] static const int dont_care = []
    {
        srand((uint32_t) time(nullptr));
        return 0;
    }();
}

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

int kl::random::get_int(const int start_inclusive, const int end_exclusive)
{
    return rand() % (end_exclusive - start_inclusive) + start_inclusive;
}

int kl::random::get_int(const int end_exclusive)
{
    return get_int(0, end_exclusive);
}

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
    return (float) (rand() / RAND_MAX);
}

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
