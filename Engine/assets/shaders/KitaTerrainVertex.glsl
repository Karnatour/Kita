#version 460 core

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec4 position;
    vec4 front;
    vec4 params;// zNear | zFar | unused | unused
} camera;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aSlope;

out vec2 texCoord;
out vec3 normalVec;
out vec3 fragPosWorld;
out mat3 TBNMat;

void main(){
    fragPosWorld = aPos;
    texCoord = vec2(aPos.x, aPos.z);

    vec3 N = normalize(vec3(-aSlope.x, 1.0, -aSlope.y));
    vec3 T = normalize(vec3(1.0, aSlope.x, 0.0));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBNMat = mat3(T, B, N);
    normalVec = N;

    gl_Position = camera.projection * camera.view * vec4(aPos, 1.0);
}