// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;

uniform mat4 W;
uniform mat4 VP;

void main() {
    gl_Position = VP * W * vec4(inWorld, 1);
}

// fragment shader
#version 330

uniform int index;

out vec4 outIndex;

void main() {
    outIndex = vec4(index);
}
