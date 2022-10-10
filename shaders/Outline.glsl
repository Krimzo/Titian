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

out vec4 outPixel;

vec2 toUV(vec2 pos) {
    return pos / frameSize;
}

bool isValidSelection() {
    return selectedIndex > 0;
}

bool isValidPixel() {
    return texture(indexMap, toUV(gl_FragCoord.xy)).x != selectedIndex;
}

bool isOutlinePixel(vec2 coords) {
    for (vec2 pos = vec2(-outlineThickness); pos.y <= outlineThickness; pos.y++) {
        for (pos.x = -outlineThickness; pos.x <= outlineThickness; pos.x++) {
            if (texture(indexMap, toUV(coords + pos)).x == selectedIndex) {
                return true;
            }
        }
    }
    return false;
}

void main() {
    if (isValidSelection() && isValidPixel() && isOutlinePixel(gl_FragCoord.xy)) {
        outPixel = outlineColor;
    }
    else {
        discard;
    }
}
