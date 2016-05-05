#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;
in vec3 posWorldspace;
in vec3 normalCameraspace;
in vec3 eyeDirCameraspace;
in vec3 lightDirCameraspace;

// Output data
//out vec4 color;
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D mTextureSampler;
uniform mat4 mv;
uniform vec3 lightPosWorldspace;

void main() {
    // Light emission properties
    // You probably want to put them as uniforms
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 50.0f;

    // Material properties
    vec3 materialDiffuseColor = texture(mTextureSampler, uv).rgb;
    vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
    vec3 materialSpecularColor = vec3(0.3, 0.3, 0.3);

    // Distance to the light
    float distance = length(lightPosWorldspace - posWorldspace);

    // Normal of the computed fragment, in camera space
    vec3 n = normalize(normalCameraspace);
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize(lightDirCameraspace);
    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp(dot(n, l), 0, 1);

    // Eye vector (towards the camera)
    vec3 e = normalize(eyeDirCameraspace);
    // Direction in which the triangle reflects the light
    vec3 r = reflect(-l, n);
    // Cosine of the angle between the eye vector and the reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp(dot(e, r), 0, 1);

    color =
        // Ambient: simulates indirect lighting
        materialAmbientColor +
        // Diffuse: "Color" of the object
        materialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
        // Specular: reflective highlight, like a mirror
        materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);
    // Output color = color of the texture at the specified uv
    // NOTE: Must specify alpha channel or it will default to 0 and
    // object will be invisible!
    //color = vec4(texture(mTextureSampler, uv).rgb, 1.0);
}