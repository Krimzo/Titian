// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;
layout (location = 1) in vec2 inTexture;
layout (location = 2) in vec3 inNormal;

out vec3 outWorld;
out vec2 outTexture;
out vec3 outNormal;

uniform mat4 W;
uniform mat4 VP;

void main() {
    outWorld = (W * vec4(inWorld, 1)).xyz;
    gl_Position = VP * vec4(outWorld, 1);
    outTexture = inTexture;
    outNormal = (W * vec4(inNormal, 0)).xyz;
}

// fragment shader
#version 330

in vec3 outWorld;
in vec2 outTexture;
in vec3 outNormal;

out vec4 outPixel;

uniform sampler2D colorMap;

void main() {
    vec4 textureColor = texture(colorMap, outTexture);
    vec3 pixelNormal = normalize(outNormal);

    vec3 lightDirection = normalize(vec3(1, -1, 1));
    float diffuseFactor = dot(pixelNormal, -lightDirection);

    outPixel = textureColor * diffuseFactor;
}
