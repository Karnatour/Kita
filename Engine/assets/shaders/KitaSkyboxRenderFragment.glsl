#version 460 core

const uint ALBEDO             = 1u << 0;
const uint METALLIC_ROUGHNESS = 1u << 1;
const uint CUBEMAP            = 1u << 2;
const uint COLOR              = 1u << 3;
const uint DEPTH              = 1u << 4;
const uint STENCIL            = 1u << 5;
const uint SKYBOX             = 1u << 6;
const uint NORMAL             = 1u << 7;
const uint DEPTH_ARRAY        = 1u << 8;

out vec4 FragColor;

in vec3 texCoord;

uniform uint textureState;

uniform samplerCube cubemapTex;

void main()
{
    if ((textureState & CUBEMAP) != 0u){
        FragColor = texture(cubemapTex, texCoord);
    }
    else {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
}