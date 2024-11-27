#pragma once

#include "scene/components/texture.h"
#include "scene/camera.h"


namespace titian
{
struct DirectionalLight : Entity
{
    static constexpr int CASCADE_COUNT = 4;

    struct Defaults
    {
        static constexpr int RESOLUTION = 2560;
        static constexpr float CASCADES[CASCADE_COUNT + 1] = { 0.0f, 0.015f, 0.05f, 0.15f, 0.5f };
    };

    Float3 color{ 1.0f };
    float point_size = 0.2f;
    float cascade_splits[std::size( Defaults::CASCADES )] = {};

    DirectionalLight();

    void serialize( Serializer& serializer ) const override;
    void deserialize( Serializer const& serializer ) override;

    void set_resolution( int resolution );
    int resolution() const;

    void set_direction( Float3 const& direction );
    Float3 direction() const;

    dx::DepthView depth_view( int cascade_index ) const;
    dx::ShaderView shader_view( int cascade_index ) const;

    Float4x4 light_matrix( Camera* camera, int cascade_index ) const;

private:
    int m_resolution = 0;
    Float3 m_direction = { 0.0f, -1.0f, 0.0f };
    Texture m_cascades[CASCADE_COUNT] = {};
};
}
