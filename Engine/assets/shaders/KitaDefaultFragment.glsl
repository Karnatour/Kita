#version 460 core

struct Light {
    vec4 position;
    vec4 direction;
    vec4 attenuation;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
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

layout (std140, binding = 2) uniform PhongPropertiesUBO {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 params;// shininess | unused | unused | unused
} phongProperties;

layout (std430, binding = 0) buffer LightsSSBO {
    ivec4 m_params;//lightCount | unused | unused | unused
    Light m_lights[];
} lights;

out vec4 FragColor;

uniform sampler2D diffuseTex;
uniform bool hasDiffuseTex;

uniform sampler2D specularTex;
uniform bool hasSpecularTex;

uniform sampler2D normalTex;
uniform bool hasNormalTex;

uniform sampler2DArray depthTexArray;//directionalShadowMap textureArray
uniform bool hasDepthTexArray;

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normalVec;
in vec3 fragPosWorld;
in mat3 TBNMat;

vec3 calculateDirectionalLight(vec3 normal, Light light);
vec3 calculatePointLight(vec3 normal, Light light);
vec3 calculateSpotLight(vec3 normal, Light light);
vec3 calculatePhongVec(Light light, float diff, float spec, float attenuation, bool applyAttenuation, float shadow);
vec2 calculateShading(vec3 normal, vec3 lightDir);
float calculateShadow(vec3 normal, vec3 lightDir);

void main()
{
    vec3 combined = vec3(0.0);
    for (int i = 0; i < lights.m_params.x; i++) {
        int lightType = lights.m_lights[i].params.x;
        vec3 normal = normalVec;
        if(hasNormalTex){
            normal = texture(normalTex, texCoord).rgb;
            normal = normal * 2.0f - 1.0f;
            normal = normalize(TBNMat * normal);
        }

        if (lightType == 0) {
            combined += calculateDirectionalLight(normal, lights.m_lights[i]);
        }
        else if (lightType == 1) {
        }
        else if (lightType == 2) {

        }
    }
    FragColor = vec4(combined, 1.0);
}

vec3 calculateDirectionalLight(vec3 normal, Light light){
    vec3 lightDir = normalize(-light.direction.xyz);

    vec2 shading = calculateShading(normal, lightDir);

    float shadow = 0.0;

    shadow = calculateShadow(normal, lightDir);

    return calculatePhongVec(light, shading.x, shading.y, 0.0f, false, shadow);
}

vec3 calculatePointLight(vec3 normal, Light light){
    vec3 lightDir = normalize(light.position.xyz - fragPosWorld);

    vec2 shading = calculateShading(normal, lightDir);

    float distance = length(light.position.xyz - fragPosWorld);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

    return calculatePhongVec(light, shading.x, shading.y, attenuation, true, 0.0);
}

vec3 calculateSpotLight(vec3 normal, Light light){
    vec3 lightDir = normalize(light.position.xyz - fragPosWorld);

    vec2 shading = calculateShading(normal, lightDir);

    float distance = length(light.position.xyz - fragPosWorld);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutOff.x - light.cutOff.y;
    float intensity = clamp((theta - light.cutOff.y) / epsilon, 0.0, 1.0);

    return calculatePhongVec(light, shading.x, shading.y, attenuation * intensity, true, 0.0);
}

vec3 calculatePhongVec(Light light, float diff, float spec, float attenuation, bool applyAttenuation, float shadow){
    vec3 ambient, diffuse, specular;
    if (hasDiffuseTex) {
        ambient = light.ambient.xyz * phongProperties.ambient.xyz * vec3(texture(diffuseTex, texCoord));
        diffuse = light.diffuse.xyz * phongProperties.diffuse.xyz * diff * vec3(texture(diffuseTex, texCoord));
    } else {
        ambient = light.ambient.xyz * phongProperties.ambient.xyz;
        diffuse = light.diffuse.xyz * phongProperties.diffuse.xyz * diff;
    }

    if (hasSpecularTex) {
        specular = light.specular.xyz * phongProperties.specular.xyz * spec * vec3(texture(specularTex, texCoord));
    } else {
        specular = light.specular.xyz * phongProperties.specular.xyz * spec;
    }

    if (applyAttenuation) {
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    diffuse *= (1.0 - shadow);
    specular *= (1.0 - shadow);

    return ambient + diffuse + specular;
}

vec2 calculateShading(vec3 normal, vec3 lightDir){
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 viewDir = normalize(camera.position.xyz - fragPosWorld);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), phongProperties.params.x);
    return vec2(diff, spec);
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