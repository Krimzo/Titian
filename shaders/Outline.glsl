// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;

void main() {
    gl_Position = vec4(inWorld, 1);
}

// fragment shader
#version 330

uniform sampler2D indexMap;
uniform vec4 outlineColor;
uniform vec2 frameSize;
uniform int outlineThickness;
uniform int selectedIndex;

out vec4 pixel;

vec2 toUV(vec2 pos) {
    return pos / frameSize;
}

float sqr(float value) {
    return value * value;
}

float squish(float value, float minValue, float maxValue) {
    return (value - minValue) / (maxValue - minValue);
}

void main() {
    if (selectedIndex > 0 && texture(indexMap, toUV(gl_FragCoord.xy)).x == 0) {
        //int counter = 0;
        for (int y = -outlineThickness; y <= outlineThickness; y++) {
            for (int x = -outlineThickness; x <= outlineThickness; x++) {
                if (texture(indexMap, toUV(gl_FragCoord.xy + vec2(x, y))).x == selectedIndex) {
                    //counter += 1;
                    pixel = outlineColor;
                    break;
                }
            }
        }
        //pixel = outlineColor * squish(counter, 0, sqr(outlineThickness * 2 + 1) - 1);
    }
    else {
        discard;
    }
}
