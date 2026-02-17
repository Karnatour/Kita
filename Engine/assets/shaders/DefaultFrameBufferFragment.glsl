#version 460
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D colorTex;
uniform bool hasColorTex;

void main()
{
    if (hasColorTex){
        FragColor = texture(colorTex, texCoord);
    }
    else {
        FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
    }
}