#pragma once

#include "serial/serial.h"

namespace titian
{
struct Material : Serializable
{
    float texture_blend = 0.0f;
    float reflectivity_factor = 0.0f;
    float refraction_index = 1.0f;

    float4 color = kl::colors::WHITE;
    float4x4 custom_data;

    String color_texture_name = "/";
    String normal_texture_name = "/";
    String roughness_texture_name = "/";
    String shader_name = "/";

    Material();

    void serialize(Serializer& serializer) const override;
    void deserialize(Serializer const& serializer) override;

    bool is_transparent() const;

    Ref<Material> clone() const;
};
} // namespace titian
