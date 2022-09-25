// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;

uniform mat4 WVP;

void main() {
    gl_Position = WVP * vec4(inWorld, 1);
}

// fragment shader
#version 330

out vec4 outIndex;

uniform int index;

void main() {
    outIndex = vec4(index);
}
