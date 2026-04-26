#version 460
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D colorTex;
uniform bool hasColorTex;

vec3 ACESFilm(vec3 x) {
    float a = 2.51, b = 0.03, c = 2.43, d = 0.59, e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

void main()
{
    float gamma = 2.2;

    if (hasColorTex){
        vec4 fragmentFromTexture = texture(colorTex, texCoord);
        vec3 hdrColor = fragmentFromTexture.rgb;

        vec3 mapped = ACESFilm(hdrColor);

        FragColor.rgb = pow(mapped, vec3(1.0 / gamma));
        FragColor.a = fragmentFromTexture.a;
    }
    else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}