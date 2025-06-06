#version 460 core

layout(std140, binding = 0) uniform CameraBlock {
    mat4 view;
    mat4 projection;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
}