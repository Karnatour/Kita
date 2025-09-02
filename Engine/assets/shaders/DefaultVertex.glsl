#version 460 core

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec4 position;
    vec4 front;
} camera;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor;
out vec2 textureCoord;
out vec3 normalVec;
out vec3 fragPos;

uniform mat4 model;

void main(){
    fragPos = vec3(model * vec4(aPos, 1.0));
    vertexColor = aColor;
    textureCoord = aTex;
    normalVec = normalize(mat3(transpose(inverse(model))) * aNormal);

    gl_Position = camera.projection * camera.view * vec4(fragPos, 1.0);
}