#version 460 core
out vec4 FragColor;

struct LightProperties {
    vec4 position;
    vec4 direction;
    vec4 attenuation;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 cutOff;
    mat4 lightSpaceMatrix;
    int lightType;
};

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
    vec4 position;
    vec4 front;
} camera;

layout(std140, binding = 1) uniform PhongProperties {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} phongProperties;

layout(std430, binding = 2) buffer Lights {
    int lightCount;
    LightProperties lights[];
} lights;

// In futurure replace bools with bit field
uniform sampler2D diffuseTex;
uniform bool hasDiffuseTex;

uniform sampler2D specularTex;
uniform bool hasSpecularTex;

uniform sampler2D depthTex;

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normalVec;
in vec3 fragPos;
in vec4 fragPosLightSpace;

vec3 calculateDirectionalLight(LightProperties light);
vec3 calculatePointLight(LightProperties light);
vec3 calculateSpotLight(LightProperties light);
vec3 calculatePhongVec(LightProperties light, float diff, float spec, float attenuation, bool applyAttenuation,float shadow);
vec2 calculateShading(vec3 lightDir);
float calculateShadow(vec4 fragPosLightSpace);

void main()
{
    vec3 combined = vec3(0.0);
    if (lights.lightCount > 0 && lights.lights[0].lightType == 0) {
        combined = calculateDirectionalLight(lights.lights[0]);
    }
    FragColor = vec4(combined, 1.0);
}

vec3 calculateDirectionalLight(LightProperties light){
    vec3 lightDir = normalize(-light.direction.xyz);

    vec2 shading = calculateShading(lightDir);

    float shadow = 0.0;

    shadow = calculateShadow(fragPosLightSpace);

    return calculatePhongVec(light, shading.x, shading.y, 0.0f, false, shadow);
}

vec3 calculatePointLight(LightProperties light){
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    vec2 shading = calculateShading(lightDir);

    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

    return calculatePhongVec(light, shading.x, shading.y, attenuation, true, 0.0);
}

vec3 calculateSpotLight(LightProperties light){
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    vec2 shading = calculateShading(lightDir);

    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutOff.x - light.cutOff.y;
    float intensity = clamp((theta - light.cutOff.y) / epsilon, 0.0, 1.0);

    return calculatePhongVec(light, shading.x, shading.y, attenuation * intensity, true, 0.0);
}

vec3 calculatePhongVec(LightProperties light, float diff, float spec, float attenuation, bool applyAttenuation, float shadow){
    vec3 ambient, diffuse, specular;
    if (hasDiffuseTex) {
        ambient = light.ambient.xyz * vec3(texture(diffuseTex, texCoord));
        diffuse = light.diffuse.xyz * diff * vec3(texture(diffuseTex, texCoord));
    }
    else {
        ambient = light.ambient.xyz;
        diffuse = light.diffuse.xyz * diff;
    }

    if (hasSpecularTex) {
        specular = light.specular.xyz * spec * vec3(texture(specularTex, texCoord));
    }
    else {
        specular = light.specular.xyz * spec;
    }

    if (applyAttenuation){
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    diffuse *= (1.0 - shadow);
    specular *= (1.0 - shadow);

    return ambient + diffuse + specular;
}

vec2 calculateShading(vec3 lightDir){
    float diff = max(dot(normalVec, lightDir), 0.0);

    vec3 viewDir = normalize(camera.position.xyz - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normalVec, halfwayDir), 0.0), phongProperties.shininess);
    return vec2(diff, spec);
}

float calculateShadow(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthTex, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}