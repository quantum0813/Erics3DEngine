#version 150 core

in vec2 initialZ;

out vec4 color;

uniform sampler1D texGradient;
uniform vec2 C;

void main() {
    vec2 Z = initialZ;
    int iterations = 0;
    const float thresholdSquared = 16.0;
    const int maxIterations = 256;
    while (iterations < maxIterations && dot(Z, Z) < thresholdSquared) {
        vec2 zSquared;
        zSquared.x = Z.x * Z.x - Z.y * Z.y;
        zSquared.y = 2.0 * Z.x * Z.y;
        Z = zSquared + C;
        iterations++;
    }
    if (iterations == maxIterations)
        color = vec4(0.0, 0.0, 0.0, 1.0);
    else
        color = texture(texGradient, float(iterations) / float(maxIterations));
}