#version 420 core

// Incoming per vertex position
in vec4 vVertex;

// Output varyings
out vec4 color;

uniform mat4 mvpMatrix;

layout (binding = 0) uniform sampler1D grassPaletteTexture;
layout (binding = 1) uniform sampler2D lengthTexture;
layout (binding = 2) uniform sampler2D orientationTexture;
layout (binding = 3) uniform sampler2D grassColorTexture;
layout (binding = 4) uniform sampler2D bendTexture;

int random(int seed, int iterations) {
    int value = seed;
    int n;

    for (n = 0; n < iterations; n++)
        value = ((value >> 7) ^ (value << 9)) * 15485863;

    return value;
}

vec4 randomVector(int seed) {
    int r = random(gl_InstanceID, 4);
    int g = random(r, 2);
    int b = random(g, 2);
    int a = random(b, 2);

    return vec4(
        float(r & 0x3FF) / 1024.0,
        float(g & 0x3FF) / 1024.0,
        float(b & 0x3FF) / 1024.0,
        float(a & 0x3FF) / 1024.0
    );
}

mat4 constructRotationMatrix(float angle) {
    float st = sin(angle);
    float ct = cos(angle);

    return mat4(
        vec4(ct, 0.0, st, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(-st, 0.0, ct, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

void main() {
    vec4 offset = vec4(float(gl_InstanceID >> 10) - 512.0, 0.0f, float(gl_InstanceID & 0x3ff) - 512.0, 0.0f);
    int number1 = random(gl_InstanceID, 3);
    int number2 = random(number1, 2);
    offset += vec4(
        float(number1 & 0xFF) / 256.0,
        0.0f,
        float(number2 & 0xFF) / 256.0,
        0.0f
    );

    vec2 texCoord = offset.xz / 1024.0 + vec2(0.5);

    float bendFactor = texture(bendTexture, texCoord).r * 2.0;
    float bendAmount = cos(vVertex.y);

    float angle = texture(orientationTexture, texCoord).r * 2.0 * 3.141592;
    mat4 rot = constructRotationMatrix(angle);
    vec4 position = (rot * (vVertex + vec4(0.0, 0.0, bendAmount * bendFactor, 0.0))) + offset;

    position *= vec4(1.0, texture(lengthTexture, texCoord).r * 0.9 + 0.3, 1.0, 1.0);

    gl_Position = mvpMatrix * position;
    color = texture(grassPaletteTexture, texture(grassColorTexture, texCoord).r) + vec4(randomVector(gl_InstanceID).xyz * vec3(0.1, 0.5, 0.1), 1.0);
}