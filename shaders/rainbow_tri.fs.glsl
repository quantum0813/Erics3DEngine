#version 150 core

uniform vec3 vertexColor;

in vec3 Color;
out vec4 outColor;

void main() {
    outColor = vec4(vertexColor + Color, 1.0f);
}