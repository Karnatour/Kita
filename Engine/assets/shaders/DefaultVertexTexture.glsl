#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec2 texturePos;

void main(){
    gl_Position = vec4(aPos,1.0);
    texturePos = aTex;
}