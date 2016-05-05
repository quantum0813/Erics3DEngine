#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 vertexPosModelspace;
layout (location = 1) in vec3 vertexNormalModelspace;
layout (location = 2) in vec2 vertexUV;

// Output data; will be interpolated for each fragment.
out vec2 uv;
out vec3 posWorldspace;
out vec3 normalCameraspace;
out vec3 eyeDirCameraspace;
out vec3 lightDirCameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 v;
uniform mat4 m;
uniform vec3 lightPosWorldspace;

void main() {
    // Output position of the vertex, in clip space: mvp * position
    gl_Position = mvp * vec4(vertexPosModelspace, 1);

    // Position of the vertex, in worldspace: m * position
    posWorldspace = (m * vec4(vertexPosModelspace, 1)).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0, 0, 0).
    vec3 vertexPosCameraspace = (v * m * vec4(vertexPosModelspace, 1)).xyz;
    lightDirCameraspace = vec3(0, 0, 0) - vertexPosCameraspace;

    // Normal of the vertex, in camera space
    normalCameraspace = (v * transpose(inverse(m)) * vec4(vertexNormalModelspace, 0)).xyz;  // Only correct if ModelMatrix does not scale the model! Use its inverse transpose if not.

    // UV of the vertex. No special case for this one.
    uv = vertexUV;
}