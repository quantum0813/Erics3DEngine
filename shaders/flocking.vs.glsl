#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 2) in vec3 boidPosition;
layout (location = 3) in vec3 boidVelocity;

out VS_OUT {
    flat vec3 color;
} vsOut;

uniform mat4 mvp;

mat4 makeLookAt(vec3 forward, vec3 up) {
    vec3 side = cross(forward, up);
    vec3 uFrame = cross(side, forward);

    return mat4(
        vec4(side, 0.0),
        vec4(uFrame, 0.0),
        vec4(forward, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

vec3 chooseColor(float f) {
    float r = sin(f * 6.2831853);
    float g = sin((f + 0.3333) * 6.2831853);
    float b = sin((f + 0.6666) * 6.2831853);

    return vec3(r, g, b) * 0.25 + vec3(0.75);
}

void main() {
    mat4 lookAt = makeLookAt(normalize(boidVelocity), vec3(0.0, 1.0, 0.0));
    vec4 objCoord = lookAt * vec4(position.xyz, 1.0);
    gl_Position = mvp * (objCoord + vec4(boidPosition, 0.0));

    vec3 n = mat3(lookAt) * normal;
    vec3 c = chooseColor(fract(float(gl_InstanceID / float(1237.0))));

    vsOut.color = mix(c * 0.2, c, smoothstep(0.0, 0.8, abs(n).z));
}