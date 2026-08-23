#version 460

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

in vec2 texCoord;

uniform uint textureState;

uniform sampler2D colorTex;

uniform float exposure;

vec3 ACESFilm(vec3 x) {
    float a = 2.51, b = 0.03, c = 2.43, d = 0.59, e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

void main()
{
    float gamma = 2.2;

    if ((textureState & COLOR) != 0u){
        vec4 fragmentFromTexture = texture(colorTex, texCoord);
        vec3 hdrColor = fragmentFromTexture.rgb;

        hdrColor *= exposure;

        vec3 mapped = ACESFilm(hdrColor);

        FragColor.rgb = pow(mapped, vec3(1.0 / gamma));
        FragColor.a = fragmentFromTexture.a;
    }
    else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}