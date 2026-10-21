#version 460

const float FOG_LINEAR = 0.0f;
const float FOG_EXP = 1.0f;
const float FOG_EXP2 = 2.0f;

const uint DEPTH = 1u << 4;

out vec4 FragColor;

in vec2 texCoord;

uniform uint textureState;
uniform sampler2D depthTex;
uniform mat4 invViewProj;

layout (std140, binding = 3) uniform EnvironmentUBO {
    vec4 fogColor;    // rgb | unused
    vec4 fogParams;   // fogStart | fogEnd | fogDensity | fogMode
} environment;

// used for sky blending
const float HORIZON_START = 0.35;
const float HORIZON_END = 1.0;

float computeFogAmount(float dist)
{
    float fogStart = environment.fogParams.x;
    float fogEnd = environment.fogParams.y;
    float fogDensity = environment.fogParams.z;
    float fogMode = environment.fogParams.w;

    float x = max(dist - fogStart, 0.0) / (fogEnd - fogStart);
    float amount;

    if (fogMode == FOG_LINEAR) {
        amount = x;
    }
    else if (fogMode == FOG_EXP) {
        amount = 1.0 - exp(-x * fogDensity);
    }
    else { // FOG_EXP2
        float d = x * fogDensity;
        amount = 1.0 - exp(-d * d);
    }

    return clamp(amount, 0.0, 1.0);
}

void main()
{
    if ((textureState & DEPTH) == 0u) {
        discard;
    }

    float depth = texture(depthTex, texCoord).r;

    // CameraToFragment conversion
    vec4 ndc = vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 offset = invViewProj * ndc;
    vec3 toFragment = offset.xyz / offset.w;

    float amount;

    if (depth >= 0.99999) {
        // sky: no distance, use view direction instead
        vec3 dir = normalize(toFragment);
        amount = 1.0 - smoothstep(HORIZON_START, HORIZON_END, dir.y);
    }
    else {
        float dist = length(toFragment);
        amount = computeFogAmount(dist);
    }

    FragColor = vec4(environment.fogColor.rgb, amount);
}