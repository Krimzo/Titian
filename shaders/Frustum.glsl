// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;

uniform mat4 iVP;
uniform mat4 VP;

void main() {
    vec4 corner = iVP * vec4(inWorld, 1);
    corner /= corner.w;

    gl_Position = VP * corner;
}

// fragment shader
#version 330

uniform vec3 color;

out vec4 pixel;

void main() {
    pixel = vec4(color, 1);
}
