// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;

uniform mat4 VP;
uniform mat4 iVP;

void main() {
    vec4 corner = iVP * vec4(inWorld, 1);
    corner /= corner.w;

    gl_Position = VP * corner;
}

// fragment shader
#version 330

uniform vec3 color;

out vec4 outPixel;

void main() {
    outPixel = vec4(color, 1);
}
