#include "fuze.h"


titian::ImageEffectSclRotPos::ImageEffectSclRotPos()
    : ImageEffect( get_source() )
{
    scale() = Float2( 1.0f );
}

titian::String titian::ImageEffectSclRotPos::name() const
{
    return "Scale, Rotation, Position";
}

bool titian::ImageEffectSclRotPos::needs_copy() const
{
    return true;
}

titian::Ref<titian::ImageEffect> titian::ImageEffectSclRotPos::make_copy() const
{
    Ref effect = new ImageEffectSclRotPos();
    effect->custom_data = this->custom_data;
    return effect;
}

void titian::ImageEffectSclRotPos::display_gui()
{
    im::DragFloat2( "Scale", &scale().x, 0.05f );
    im::DragFloat( "Rotation", &rotation() );
    im::DragFloat2( "Position", &position().x );
}

titian::String titian::ImageEffectSclRotPos::get_source() const
{
    return R"(
float2 rotate(float2 vec, float angle)
{
    float sin_a = sin(radians(angle));
    float cos_a = cos(radians(angle));
    return float2(cos_a * vec.x - sin_a * vec.y, sin_a * vec.x + cos_a * vec.y);
}

void func(float2 target_coords)
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

titian::Float2& titian::ImageEffectSclRotPos::scale()
{
    return (Float2&) custom_data( 0, 0 );
}

float& titian::ImageEffectSclRotPos::rotation()
{
    return (float&) custom_data( 0, 1 );
}

titian::Float2& titian::ImageEffectSclRotPos::position()
{
    return (Float2&) custom_data( 0, 2 );
}
