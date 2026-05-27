#version 460 core

struct Light {
    vec4 position;
    vec4 direction;
    vec4 diffuse;
    vec4 cutOff;
    ivec4 params;// lightType | castsShadows | pointCubemapArray | unused
};

layout (std140, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 projection;
    vec4 position;
    vec4 front;
    vec4 params;// zNear | zFar | unused | unused
} camera;

layout (std140, binding = 1) uniform DirectionalShadowUBO {
    mat4 lightSpaceMatrices[MAX_CASCADES];
    vec4 cascadeSplitDistances[MAX_CASCADES];// distance | padding | padding | padding
    ivec4 params;// cascadeCount | unused | unused | unused
} directionalShadow;

layout (std430, binding = 0) buffer LightsSSBO {
    ivec4 m_params;//lightCount | unused | unused | unused
    Light m_lights[];
} lights;

struct PBRProperties {
    vec3 albedo;
    float metallic;
    float roughness;
};

out vec4 FragColor;

uniform sampler2D albedoTex;
uniform bool hasAlbedoTex;

uniform sampler2D metallicRoughnessTex;
uniform bool hasMetallicRoughnessTex;

uniform sampler2D normalTex;
uniform bool hasNormalTex;

uniform samplerCube cubemapTex;
uniform bool hasCubemapTex;

uniform sampler2DArray depthTexArray;//directionalShadowMap textureArray
uniform bool hasDepthTexArray;

uniform float iblIntensity;

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normalVec;
in vec3 fragPosWorld;
in mat3 TBNMat;

vec3 getNormal();
vec3 calculateDirectionalLight(vec3 normal, vec3 viewDir, Light light, vec3 F0, PBRProperties PBRData);
vec3 calculatePointLight(vec3 normal, vec3 viewDir, Light light, vec3 F0, PBRProperties PBRData);
vec3 calculateSpotLight(vec3 normal, vec3 viewDir, Light light, vec3 F0, PBRProperties PBRData);
vec3 calculateOutgoingRadiance(vec3 normal, vec3 viewDir, vec3 lightDir, float attenuation, vec3 F0, PBRProperties PBRData, Light light);
vec3 fresnelSchlick(float halfwayDotViewDir, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float DistributionGGX(float normalDotHalfway, float roughness);
float GeometrySchlickGGX(float normalDot, float roughness);
float GeometrySmith(float normalDotViewDir, float normalDotLightDir, float roughness);
float calculateShadow(vec3 normal, vec3 lightDir);
PBRProperties getPBRProperties();

const float PI = 3.14159265359;

void main()
{
    vec3 viewDir = normalize(camera.position.xyz - fragPosWorld);
    vec3 normal = getNormal();
    PBRProperties PBRData = getPBRProperties();

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, PBRData.albedo, PBRData.metallic);

    vec3 combined = vec3(0.0);
    for (int i = 0; i < lights.m_params.x; i++) {
        int lightType = lights.m_lights[i].params.x;

        if (lightType == 0) {
            combined += calculateDirectionalLight(normal, viewDir, lights.m_lights[i], F0, PBRData);
        }
        else if (lightType == 1) {
            combined += calculatePointLight(normal, viewDir, lights.m_lights[i], F0, PBRData);
        }
        else if (lightType == 2) {
            combined += calculateSpotLight(normal, viewDir, lights.m_lights[i], F0, PBRData);
        }
    }

    vec3 kS = fresnelSchlickRoughness(max(dot(normal, viewDir), 0.0), F0, PBRData.roughness);
    vec3 kD = 1.0 - kS;
    vec3 irradiance = texture(cubemapTex, normal).rgb;
    vec3 diffuse = irradiance * PBRData.albedo;
    vec3 ambient = kD * diffuse * iblIntensity;// * ao

    vec3 finalColor = ambient + combined;

    FragColor = vec4(finalColor, 1.0);
}

vec3 getNormal() {
    vec3 normal;
    if (hasNormalTex){
        normal = texture(normalTex, texCoord).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(TBNMat * normal);
    }
    else {
        normal = normalVec;
    }
    return normal;
}

vec3 calculateDirectionalLight(vec3 normal, vec3 viewDir, Light light, vec3 F0, PBRProperties PBRData){
    vec3 lightDir = normalize(-light.direction.xyz);

    float shadow = 0.0;
    shadow = calculateShadow(normal, lightDir);

    float attenuation = 1.0;
    vec3 outgoingRadiance = calculateOutgoingRadiance(normal, viewDir, lightDir, attenuation, F0, PBRData, light);
    return outgoingRadiance * (1.0 - shadow);
}

vec3 calculatePointLight(vec3 normal, vec3 viewDir, Light light, vec3 F0, PBRProperties PBRData){
    vec3 lightDir = normalize(light.position.xyz - fragPosWorld);

    float distance = length(light.position.xyz - fragPosWorld);
    float attenuation = 1.0 / (distance * distance);

    vec3 outgoingRadiance = calculateOutgoingRadiance(normal, viewDir, lightDir, attenuation, F0, PBRData, light);
    return outgoingRadiance;// * (1.0 - shadow)
}

vec3 calculateSpotLight(vec3 normal, vec3 viewDir, Light light, vec3 F0, PBRProperties PBRData){
    vec3 lightDir = normalize(light.position.xyz - fragPosWorld);

    float distance = length(light.position.xyz - fragPosWorld);
    float attenuation = 1.0 / (distance * distance);

    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutOff.x - light.cutOff.y;
    float intensity = clamp((theta - light.cutOff.y) / epsilon, 0.0, 1.0);

    vec3 outgoingRadiance = calculateOutgoingRadiance(normal, viewDir, lightDir, attenuation, F0, PBRData, light);
    return outgoingRadiance * intensity;// * (1.0 - shadow)
}

vec3 calculateOutgoingRadiance(vec3 normal, vec3 viewDir, vec3 lightDir, float attenuation, vec3 F0, PBRProperties PBRData, Light light) {
    vec3 halfway = normalize(viewDir + lightDir);

    vec3 incomingRadiance = light.diffuse.xyz * attenuation;

    float normalDotViewDir = max(dot(normal, viewDir), 0.0);
    float normalDotLightDir = max(dot(normal, lightDir), 0.0);
    float normalDotHalfway = max(dot(normal, halfway), 0.0);
    float halfwayDotViewDir = max(dot(halfway, viewDir), 0.0);

    float NDF = DistributionGGX(normalDotHalfway, PBRData.roughness);
    float G = GeometrySmith(normalDotViewDir, normalDotLightDir, PBRData.roughness);
    vec3 F = fresnelSchlick(halfwayDotViewDir, F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0)  + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - PBRData.metallic;

    return (kD * PBRData.albedo / PI + specular) * incomingRadiance * normalDotLightDir;
}

vec3 fresnelSchlick(float halfwayDotViewDir, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - halfwayDotViewDir, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float normalDotViewDir, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - normalDotViewDir, 0.0, 1.0), 5.0);
}

float DistributionGGX(float normalDotHalfway, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;

    float normalDotHalfwaySq = normalDotHalfway * normalDotHalfway;

    float numerator = a2;

    float denominator = (normalDotHalfwaySq * (a2 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

float GeometrySchlickGGX(float normalDot, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float numerator = normalDot;
    float denominator = normalDot * (1.0 - k) + k;

    return numerator / denominator;
}

float GeometrySmith(float normalDotViewDir, float normalDotLightDir, float roughness) {
    float ggx2 = GeometrySchlickGGX(normalDotViewDir, roughness);
    float ggx1 = GeometrySchlickGGX(normalDotLightDir, roughness);

    return ggx1 * ggx2;
}

PBRProperties getPBRProperties() {
    PBRProperties PBRData;
    PBRData.albedo = texture(albedoTex, texCoord).rgb;
    vec4 metallicRoughness = texture(metallicRoughnessTex, texCoord);
    PBRData.roughness = metallicRoughness.g;
    PBRData.metallic  = metallicRoughness.b;
    return PBRData;
}

float calculateShadow(vec3 normal, vec3 lightDir){
    vec4 fragPosViewSpace = camera.view * vec4(fragPosWorld, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = directionalShadow.params.x - 1;
    for (int i = 0; i < directionalShadow.params.x; ++i) {
        if (depthValue < directionalShadow.cascadeSplitDistances[i].x) {
            layer = i;
            break;
        }
    }
    vec4 fragPosLightSpace = directionalShadow.lightSpaceMatrices[layer] * vec4(fragPosWorld, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    if (currentDepth > 1.0) {
        return 0.0;
    }

    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    bias = clamp(bias, 0.0005, 0.003);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(depthTexArray, 0));
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(depthTexArray, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}