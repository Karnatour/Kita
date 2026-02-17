#version 460 core
out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube cubemapTex;
uniform bool hasCubemapTex;
void main()
{
    if (hasCubemapTex){
        FragColor = texture(cubemapTex, texCoord);
    }
    else {
        FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
    }
}