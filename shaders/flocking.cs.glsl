#version 430 core

layout (local_size_x = 256) in;

uniform float closestAllowedDist = 50.0;
uniform float rule1Weight = 0.18;
uniform float rule2Weight = 0.05;
uniform float rule3Weight = 0.17;
uniform float rule4Weight = 0.02;
uniform vec3 goal = vec3(0.0);
uniform float timeStep = 0.4;

struct Boid {
    vec3 position;
    vec3 velocity;
};

layout (std430, binding = 0) readonly buffer membersIn {
    Boid member[];
} inputData;

layout (std430, binding = 1) buffer membersOut {
    Boid member[];
} outputData;

shared Boid sharedMember[gl_WorkGroupSize.x];

vec3 rule1(vec3 myPos, vec3 myVel, vec3 theirPos, vec3 theirVel) {
    vec3 d = myPos - theirPos;
    if (dot(d, d) < closestAllowedDist)
        return d;
    return vec3(0.0);
}

vec3 rule2(vec3 myPos, vec3 myVel, vec3 theirPos, vec3 theirVel) {
    vec3 d = theirPos - myPos;
    vec3 dv = theirVel - myVel;
    return dv / (dot(d, d) + 10.0);
}

void main() {
    uint i, j;
    int globalId = int(gl_GlobalInvocationID.x);
    int localId = int(gl_LocalInvocationID.x);

    Boid me = inputData.member[globalId];
    Boid newMe;
    vec3 acceleration = vec3(0.0);
    vec3 flockCenter = vec3(0.0);

    for (i = 0; i < gl_NumWorkGroups.x; i++) {
        Boid them = inputData.member[i * gl_WorkGroupSize.x + localId];
        sharedMember[localId] = them;
        memoryBarrierShared();
        barrier();
        for (j = 0; j < gl_WorkGroupSize.x; j++) {
            them = sharedMember[j];
            flockCenter += them.position;
            if (i * gl_WorkGroupSize.x + j != globalId) {
                acceleration += rule1(me.position, me.velocity, them.position, them.velocity) * rule1Weight;
                acceleration += rule2(me.position, me.velocity, them.position, them.velocity) * rule2Weight;
            }
        }
        barrier();
    }

    flockCenter /= float(gl_NumWorkGroups.x * gl_WorkGroupSize.x);
    newMe.position = me.position + me.velocity * timeStep;
    acceleration += normalize(goal - me.position) * rule3Weight;
    acceleration += normalize(flockCenter - me.position) * rule4Weight;
    newMe.velocity = me.velocity + acceleration * timeStep;
    if (length(newMe.velocity) > 10.0)
        newMe.velocity = normalize(newMe.velocity) * 10.0;
    newMe.velocity = mix(me.velocity, newMe.velocity, 0.4);
    outputData.member[globalId] = newMe;
}