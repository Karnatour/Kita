#version 460 core

const uint DISPLACEMENT = 1u << 9;

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec4 position;
    vec4 front;
    vec4 params;// zNear | zFar | unused | unused
} camera;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

out vec4 vertexColor;
out vec2 texCoord;
out vec3 normalVec;
out vec3 fragPosWorld;
out mat3 TBNMat;

uniform uint textureState;
uniform sampler2D displacementTex;

uniform mat4 model;

bool hasTexture(uint flag);

void main(){
    vec3 displacedPos = aPos;

    if (hasTexture(DISPLACEMENT)){
        float height = textureLod(displacementTex, aTex, 0.0).r;
        displacedPos += aNormal * (height - 1.0) * 0.20f;
    }

    fragPosWorld = vec3(model * vec4(displacedPos, 1.0));
    vertexColor = aColor;
    texCoord = aTex;

    mat3 normalMatrix = transpose(inverse(mat3(model)));

    normalVec = normalize(normalMatrix * aNormal);

    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBNMat = mat3(T, B, N);

    gl_Position = camera.projection * camera.view * vec4(fragPosWorld, 1.0);
}

bool hasTexture(uint flag){
    return (textureState & flag) != 0u;
}