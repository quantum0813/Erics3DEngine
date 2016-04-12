#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 vertexPosModelspace;
layout (location = 1) in vec3 vertexNormals;
layout (location = 2) in vec2 vertexUV;

// Output data; will be interpolated for each fragment.
out vec2 uv;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;

void main() {
    // Output position of the vertex, in clip space: mvp * position
    gl_Position = mvp * vec4(vertexPosModelspace, 1);

    // UV of the vertex. No special case for this one.
    uv = vertexUV;
}