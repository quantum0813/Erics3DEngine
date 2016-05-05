#version 440 core

layout (originUpperLeft) in vec4 gl_FragCoord;
layout (location = 0) out vec4 oColor;
layout (binding = 0) uniform isampler2D textBuffer;
layout (binding = 1) uniform isampler2DArray fontTexture;

void main() {
    ivec2 fragCoord = ivec2(gl_FragCoord.xy);
    ivec2 charSize = textureSize(fontTexture, 0).xy;
    ivec2 charLocation = fragCoord / charSize;
    ivec2 texelCoord = fragCoord % charSize;
    int character = texelFetch(fontTexture, ivec3(texelCoord, character), 0).x;

    if (val == 0.0)
        discard;
    oColor = vec4(1.0);
}