#version 410 core

in vec4 position;
uniform mat4 mvMatrix;

void main() {
    gl_Position = mvMatrix * position;
}