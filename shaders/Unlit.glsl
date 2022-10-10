// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;
layout (location = 1) in vec2 inTexture;

out vec3 WORLD;
out vec2 TEXTURE;

uniform mat4 W;
uniform mat4 VP;

void main() {
    WORLD = (W * vec4(inWorld, 1)).xyz;
    TEXTURE = inTexture;

    gl_Position = VP * vec4(WORLD, 1);
}

// fragment shader
#version 330

in vec3 WORLD;
in vec2 TEXTURE;

out vec4 outPixel;

uniform sampler2D colorMap;

void main() {
    vec3 baseColor = texture(colorMap, TEXTURE).xyz;
    outPixel = vec4(baseColor, 1);
}
