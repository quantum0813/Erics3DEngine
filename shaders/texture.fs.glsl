#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;

// Output data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D mTextureSampler;

void main() {
    // Output color = color of the texture at the specified uv
    // NOTE: Must specify alpha channel or it will default to 0 and
    // object will be invisible!
    color = vec4(texture(mTextureSampler, uv).rgb, 1.0);
}