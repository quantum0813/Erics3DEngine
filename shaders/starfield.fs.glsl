#version 410 core

layout (location = 0) out vec4 color;

uniform sampler2D texStar;
flat in vec4 starColor;

void main() {
    color = starColor * texture(texStar, gl_PointCoord);
}