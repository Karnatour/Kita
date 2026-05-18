#version 460 core

layout(triangles, invocations = NUM_CASCADES) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 1) uniform DirectionalShadowUBO {
    mat4 lightSpaceMatrices[MAX_CASCADES];
    vec4 cascadeSplitDistances[MAX_CASCADES];// distance | padding | padding | padding
    ivec4 params;// cascadeCount | unused | unused | unused
} directionalShadow;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = directionalShadow.lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}