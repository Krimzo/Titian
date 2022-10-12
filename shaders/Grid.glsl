// vertex shader
#version 330

layout (location = 0) in vec3 inWorld;

out vec3 WORLD;

uniform mat4 W;
uniform mat4 VP;

void main() {
    vec4 world = W * vec4(inWorld, 1);
    WORLD = world.xyz;
    gl_Position = VP * world;
}

// fragment shader
#version 330

in vec3 WORLD;

out vec4 outPixel;

void main() {
    vec3 color = vec3(0.9f);

    if (WORLD.z == 0) {
        color = vec3(1, 0, 0);
    }
    else if (WORLD.x == 0) {
        color = vec3(0, 0, 1);
    }

    outPixel = vec4(color, 0.25f);
}
