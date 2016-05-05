#version 430 core

layout (location = 0) out vec4 color;

in VS_OUT {
    flat vec3 color;
} fsIn;

void main() {
    color = vec4(fsIn.color.rgb, 1.0);
}