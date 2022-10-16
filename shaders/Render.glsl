// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;

out vec3 WORLD;
out vec2 TEXTURE;
out vec3 NORMAL;

uniform mat4 W;
uniform mat4 VP;

void main() {
    WORLD = (W * vec4(inWorld, 1)).xyz;
    TEXTURE = inTexture;
    NORMAL = (W * vec4(inNormal, 0)).xyz;

    gl_Position = VP * vec4(WORLD, 1);
}

// fragment shader
#version 330

in vec3 WORLD;
in vec2 TEXTURE;
in vec3 NORMAL;

out vec4 outPixel;

uniform float colorBlend;
uniform vec3 color;
uniform sampler2D colorMap;

uniform vec3 ambientColor;
uniform vec3 sunDirection;
uniform vec3 sunColor;

void main() {
    vec3 baseColor = mix(color, texture(colorMap, TEXTURE).xyz, colorBlend);

    vec3 normal = normalize(NORMAL);
    float diffuseFactor = max(dot(normal, -sunDirection), 0);

    vec3 fullLight = diffuseFactor * sunColor + ambientColor;

    outPixel = vec4(baseColor * fullLight, 1);
}
