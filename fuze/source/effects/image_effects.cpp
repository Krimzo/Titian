#include "fuze.h"


/* ImageEffectScaleRotPos */
titian::ImageEffectScaleRotPos::ImageEffectScaleRotPos()
{
    this->init();
    scale() = Float2(1.0f);
}

bool titian::ImageEffectScaleRotPos::needs_copy() const
{
    return true;
}

titian::String titian::ImageEffectScaleRotPos::get_source() const
{
    return R"(
float2 rotate(float2 vec, float angle)
{
    const float sin_a = sin(radians(angle));
    const float cos_a = cos(radians(angle));
    return float2(cos_a * vec.x - sin_a * vec.y, sin_a * vec.x + cos_a * vec.y);
}

void func(const float2 target_coords)
{
    float2 source_coords = target_coords - frame_size * 0.5f;
    source_coords /= float2(custom_data[0][0], custom_data[1][0]);
    source_coords = rotate(source_coords, -custom_data[0][1]);
    source_coords -= float2(custom_data[0][2], -custom_data[1][2]);
    source_coords += frame_size * 0.5f;
    TARGET[convert_coords(target_coords)] = SOURCE[convert_coords(source_coords)];
}
)";
}

titian::String titian::ImageEffectScaleRotPos::get_name() const
{
    return "Scale, Rotation, Position";
}

void titian::ImageEffectScaleRotPos::display_gui()
{
    im::DragFloat2("Scale", scale(), 0.05f);
    im::DragFloat("Rotation", &rotation());
    im::DragFloat2("Position", position());
}

titian::Float2& titian::ImageEffectScaleRotPos::scale()
{
    return (Float2&) custom_data(0, 0);
}

float& titian::ImageEffectScaleRotPos::rotation()
{
    return (float&) custom_data(0, 1);
}

titian::Float2& titian::ImageEffectScaleRotPos::position()
{
    return (Float2&) custom_data(0, 2);
}
