#version 460 core
out vec4 FragColor;

in vec3 vertexColor;

uniform sampler2D texture1;
uniform bool hasTexture;

in vec2 texturePos;

void main() {
    if (hasTexture) {
        vec4 texColor = texture(texture1, texturePos);
        FragColor = texColor * vec4(vertexColor, 1.0);
    }
    else {
        FragColor = vec4(vertexColor, 1.0);
    }

}