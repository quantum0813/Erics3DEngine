#version 120

uniform sampler2D rawTexture;
uniform sampler2D distortionTexture;

void main() {
    vec2 distortionIndex = texture2D(distortionTexture, gl_TexCoord[0].st).xy;
    float hIndex = distortionIndex.r;
    float vIndex = distortionIndex.g;

    if (vIndex > 0.0 && vIndex < 1.0 && hIndex > 0.0 && hIndex < 1.0)
        gl_FragColor = vec4(texture2D(rawTexture, distortionIndex).rrr, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}