#version 460 core
#extension GL_ARB_shader_viewport_layer_array : require

layout (location = 0) in vec3 aPos;

layout (std140, binding = 1) uniform DirectionalShadowUBO {
    mat4 lightSpaceMatrices[MAX_CASCADES];
    vec4 cascadeSplitDistances[MAX_CASCADES];// distance | radius | depthPerUnit | padding
    ivec4 params;// cascadeCount | unused | unused | unused
} directionalShadow;

uniform mat4 model;
uniform int cascadeIndex;

void main() {
    gl_Position = directionalShadow.lightSpaceMatrices[cascadeIndex] * model * vec4(aPos, 1.0);
    gl_Layer = cascadeIndex;
}