#version 460 core

struct LightProperties {
    vec4 position;
    vec4 direction;
    vec4 attenuation;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 cutOff;
    mat4 lightSpaceMatrix;
    int lightType;
};

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec4 position;
    vec4 front;
} camera;

layout(std430, binding = 2) buffer Lights {
    int lightCount;
    LightProperties lights[];
} lights;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor;
out vec2 texCoord;
out vec3 normalVec;
out vec3 fragPos;
out vec4 fragPosLightSpace;

uniform mat4 model;

void main(){
    fragPos = vec3(model * vec4(aPos, 1.0));
    vertexColor = aColor;
    texCoord = aTex;
    normalVec = normalize(mat3(transpose(inverse(model))) * aNormal);

    for (int i = 0; i < lights.lightCount; i++) {
        if (lights.lights[i].lightType == 0) {
            fragPosLightSpace = lights.lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
            break;
        }
    }

    gl_Position = camera.projection * camera.view * vec4(fragPos, 1.0);
}