#version 460 core
out vec4 FragColor;

uniform sampler2D texture1;

in vec2 texturePos;

void main(){
    FragColor = texture(texture1, texturePos);
}