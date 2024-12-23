#pragma once

#include "serial/serial.h"


namespace titian
{
inline std::set<String> HLSL_KEYWORDS;
inline std::set<String> HLSL_TYPES;
inline std::set<String> HLSL_MEMBERS;
inline std::set<String> HLSL_FUNCTIONS;
}

namespace titian
{
enum ShaderType : int32_t
{
    MATERIAL = 0,
    CAMERA = 1,
};
}

namespace titian
{
struct Shader : Serializable
{
    ShaderType shader_type;
    String source;
    kl::Shaders shaders;

    Shader( ShaderType type = ShaderType::MATERIAL );

    void serialize( Serializer& serializer ) const override;
    void deserialize( Serializer const& serializer ) override;

    void reload();

private:
    void reload_for_material();
    void reload_for_camera();
};
}
