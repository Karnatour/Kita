#version 460 core
out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube cubemapTex;

void main()
{
    FragColor = texture(cubemapTex, texCoord);
}