#version 410 core

out vec4 color;

in TES_OUT {
    vec3 N;
} fsIn;

void main() {
    vec3 n = normalize(fsIn.N);

    vec4 c = vec4(1.0, -1.0, 0.0, 0.0) * n.z + vec4(0.0, 0.0, 0.0, 1.0);

    color = clamp(c, vec4(0.0), vec4(1.0));
}