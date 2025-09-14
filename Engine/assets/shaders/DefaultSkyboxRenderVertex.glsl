#version 460 core

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec4 position;
    vec4 front;
} camera;

layout (location = 0) in vec3 aPos;

out vec3 texCoord;

void main()
{
    texCoord = aPos;

    mat4 viewNoTranslation = mat4(mat3(camera.view));

    vec4 pos = camera.projection * viewNoTranslation * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
