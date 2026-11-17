#version 460 core

const uint ALBEDO = 1u << 0;
const uint METALLIC_ROUGHNESS = 1u << 1;
const uint CUBEMAP = 1u << 2;
const uint COLOR = 1u << 3;
const uint DEPTH = 1u << 4;
const uint STENCIL = 1u << 5;
const uint SKYBOX = 1u << 6;
const uint NORMAL = 1u << 7;
const uint DEPTH_ARRAY = 1u << 8;
const uint DISPLACEMENT = 1u << 9;
const uint AMBIENT_OCCLUSION = 1u << 10;

const float NORMAL_FADE_START = 60.0;
const float NORMAL_FADE_END = 100.0;
const float POM_FADE_START = 20.0;
const float POM_FADE_END = 50.0;

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
    vec4 cascadeSplitDistances[MAX_CASCADES];// distance | radius | depthPerUnit | padding
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
    float ao;
};

out vec4 FragColor;

uniform uint textureState;

uniform sampler2DArray albedoTex;
uniform sampler2DArray metallicRoughnessTex;
uniform sampler2DArray normalTex;
uniform sampler2DArray displacementTex;
uniform sampler2DArray ambientOcclusionTex;

uniform samplerCube cubemapTex;
uniform sampler2DArray depthTexArray; //directionalShadowMap textureArray

uniform float iblIntensity;

in vec2 texCoord;
in vec3 normalVec;
in vec3 fragPosWorld;
in mat3 TBNMat;

vec3 getNormal(vec2 newTexCoord, float dist);
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
bool hasTexture(uint flag);
PBRProperties getPBRProperties(vec2 newTexCoord);
float random(vec2 p);
vec3 randomColor(vec2 p);
vec2 parallaxOcclusionMapping(vec2 texCoord, vec3 viewDirTangentSpace, vec2 dx, vec2 dy);
float detailFactor(float dist, float fadeStart, float fadeEnd);

const float PI = 3.14159265359;

void main()
{
    vec3 viewDirWorld = normalize(camera.position.xyz - fragPosWorld);

    float dist = length(camera.position.xyz - fragPosWorld);
    vec2 newTexCoord = texCoord;

    if (hasTexture(DISPLACEMENT)) {
        float pomWeight = detailFactor(dist, POM_FADE_START, POM_FADE_END);
        if (pomWeight > 0.0) {
            vec3 viewDirTangentSpace = normalize(transpose(TBNMat) * viewDirWorld);
            vec2 pomUV = parallaxOcclusionMapping(texCoord, viewDirTangentSpace, dFdx(texCoord), dFdy(texCoord));
            newTexCoord = mix(texCoord, pomUV, pomWeight);
        }
    }

    vec3 normal = getNormal(newTexCoord, dist);
    PBRProperties PBRData = getPBRProperties(newTexCoord);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, PBRData.albedo, PBRData.metallic);

    vec3 combined = vec3(0.0);
    for (int i = 0; i < lights.m_params.x; i++) {
        int lightType = lights.m_lights[i].params.x;

        if (lightType == 0) {
            combined += calculateDirectionalLight(normal, viewDirWorld, lights.m_lights[i], F0, PBRData);
        }
        else if (lightType == 1) {
            combined += calculatePointLight(normal, viewDirWorld, lights.m_lights[i], F0, PBRData);
        }
        else if (lightType == 2) {
            combined += calculateSpotLight(normal, viewDirWorld, lights.m_lights[i], F0, PBRData);
        }
    }

    vec3 kS = fresnelSchlickRoughness(max(dot(normal, viewDirWorld), 0.0), F0, PBRData.roughness);
    vec3 kD = 1.0 - kS;
    vec3 irradiance = texture(cubemapTex, normal).rgb;
    vec3 diffuse = irradiance * PBRData.albedo;
    vec3 ambient = kD * diffuse * iblIntensity * PBRData.ao;

    vec3 finalColor = ambient + combined;
    FragColor = vec4(finalColor, 1.0);
}

vec3 getNormal(vec2 newTexCoord, float dist) {
    vec3 geomN = normalize(normalVec);
    if (!hasTexture(NORMAL)) {
        return geomN;
    }

    float weight = detailFactor(dist, NORMAL_FADE_START, NORMAL_FADE_END);
    if (weight <= 0.0) {
        return geomN;
    }

    vec3 n = texture(normalTex, vec3(newTexCoord,0)).rgb * 2.0 - 1.0;
    vec3 mapped = normalize(TBNMat * n);
    return normalize(mix(geomN, mapped, weight));
}

vec3 calculateDirectionalLight(vec3 normal, vec3 viewDir, Light light, vec3 F0, PBRProperties PBRData){
    vec3 lightDir = normalize(-light.direction.xyz);

    float shadow = 0.0;
    shadow = calculateShadow(normalize(normalVec), lightDir);

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
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
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
    float k = (r * r) / 8.0;

    float numerator = normalDot;
    float denominator = normalDot * (1.0 - k) + k;

    return numerator / denominator;
}

float GeometrySmith(float normalDotViewDir, float normalDotLightDir, float roughness) {
    float ggx2 = GeometrySchlickGGX(normalDotViewDir, roughness);
    float ggx1 = GeometrySchlickGGX(normalDotLightDir, roughness);

    return ggx1 * ggx2;
}

PBRProperties getPBRProperties(vec2 newTexCoord) {
    PBRProperties PBRData;
    if (hasTexture(ALBEDO)){
        PBRData.albedo = texture(albedoTex, vec3(newTexCoord,0)).rgb;
    }
    else {
        PBRData.albedo = vec3(0.0f);
    }
    if (hasTexture(METALLIC_ROUGHNESS)){
        vec4 metallicRoughness = texture(metallicRoughnessTex, vec3(newTexCoord,0));
        PBRData.roughness = metallicRoughness.g;
        PBRData.metallic = metallicRoughness.b;
    }
    else {
        PBRData.roughness = 1.0f;
        PBRData.metallic = 0.0f;
    }
    if (hasTexture(AMBIENT_OCCLUSION)) {
        PBRData.ao = texture(ambientOcclusionTex, vec3(newTexCoord,0)).r;
    } else {
        PBRData.ao = 1.0;
    }
    return PBRData;
}

float calculateShadow(vec3 geomNormal, vec3 lightDir){
    float normalDotLightDir = dot(geomNormal, lightDir);
    if (normalDotLightDir <= 0.0) {
        return 1.0;
    }

    vec4 fragPosViewSpace = camera.view * vec4(fragPosWorld, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = directionalShadow.params.x - 1;
    for (int i = 0; i < directionalShadow.params.x; ++i) {
        if (depthValue < directionalShadow.cascadeSplitDistances[i].x) {
            layer = i;
            break;
        }
    }

    mat4 lsm = directionalShadow.lightSpaceMatrices[layer];

    vec4 cascadeData = directionalShadow.cascadeSplitDistances[layer];

    float cascadeRadius = cascadeData.y;
    float depthPerUnit = cascadeData.z;

    vec2 mapSize = vec2(textureSize(depthTexArray, 0).xy);
    float texelWorld = 2.0 * cascadeRadius / mapSize.x;

    float sinTheta = sqrt(max(1.0 - normalDotLightDir * normalDotLightDir, 0.0));
    float tanTheta = min(sinTheta / max(normalDotLightDir, 0.001), 4.0);

    const float NORMAL_OFFSET_TEXELS = 1.5;
    vec3 offsetPos = fragPosWorld + geomNormal * texelWorld * NORMAL_OFFSET_TEXELS * sinTheta;

    vec4 fragPosLightSpace = lsm * vec4(offsetPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0) {
        return 0.0;
    }

    float biasWorld = texelWorld * (0.5 + 1.0 * tanTheta);
    float bias = biasWorld * depthPerUnit;

    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / mapSize;
    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            float pcfDepth = texture(depthTexArray, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 25.0;
}

bool hasTexture(uint flag){
    return (textureState & flag) != 0u;
}

float random(vec2 p){
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 randomColor(vec2 p) {
    return vec3(random(p), random(p + 1.0), random(p + 2.0));
}

vec2 parallaxOcclusionMapping(vec2 texCoord, vec3 viewDirTangentSpace, vec2 dx, vec2 dy) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    const float heightScale = 0.05;
    // calculate the correct number of layer based on dot product
    float layersCount = mix(maxLayers, minLayers, abs(viewDirTangentSpace.z));

    // calculate depth of each layer
    float layerDepth = 1.0f / layersCount;

    // calculate total shift
    vec2 P = viewDirTangentSpace.xy / max(viewDirTangentSpace.z, 0.1) * heightScale;
    // get shift for one layer
    vec2 deltaTexCoords = P / layersCount;

    // fetch original texture values
    vec2 currentTexCoord = texCoord;
    float currentDisplacement = 1.0 - textureGrad(displacementTex, vec3(currentTexCoord,0), dx, dy).r;

    float currentLayerDepth = 0.0f;
    for (int i = 0; i < maxLayers; ++i) {
        if (currentLayerDepth >= currentDisplacement){
            break;
        }
        // shift texture coordinates
        currentTexCoord -= deltaTexCoords;
        currentDisplacement = 1.0 - textureGrad(displacementTex, vec3(currentTexCoord,0), dx, dy).r;
        currentLayerDepth += layerDepth;
    }

    // interpolate between the last two steps for a smooth result
    vec2 prevTexCoord = currentTexCoord + deltaTexCoords;
    float afterDepth = currentDisplacement - currentLayerDepth;
    float beforeDepth = (1.0 - textureGrad(displacementTex, vec3(prevTexCoord,0), dx, dy).r) - (currentLayerDepth - layerDepth);
    float denominator = afterDepth - beforeDepth;
    float weight = abs(denominator) > 1e-5 ? afterDepth / denominator : 0.0;
    return mix(currentTexCoord, prevTexCoord, clamp(weight, 0.0, 1.0));
}

float detailFactor(float dist, float fadeStart, float fadeEnd) {
    return 1.0 - smoothstep(fadeStart, fadeEnd, dist);
}