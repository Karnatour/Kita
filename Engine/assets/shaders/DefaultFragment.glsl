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


uniform sampler2D diffuseTex;
uniform bool hasDiffuseTex;

uniform sampler2D specularTex;
uniform bool hasSpecularTex;

in vec4 vertexColor;
in vec2 textureCoord;
in vec3 normalVec;
in vec3 fragPos;


vec3 calculateDirectionalLight(LightProperties light);
vec3 calculatePointLight(LightProperties light);
vec3 calculateSpotLight(LightProperties light);
vec3 calculatePhongVec(LightProperties light, float diff, float spec, float attenuation, bool applyAttenuation);
vec2 calculateShading(vec3 lightDir);

void main()
{
    vec3 combined = vec3(0.0);
    for (int i = 0; i < lights.lightCount; i++){
        switch (lights.lights[i].lightType){
            case 0:
            combined += calculateDirectionalLight(lights.lights[i]);
            break;
            case 1:
            combined += calculatePointLight(lights.lights[i]);
            break;
            case 2:
            combined += calculateSpotLight(lights.lights[i]);
            break;
            default :
            break;
        }
    }
    FragColor = vec4(combined, 1.0);
}

vec3 calculateDirectionalLight(LightProperties light){
    vec3 lightDir = normalize(-light.direction.xyz);

    vec2 shading = calculateShading(lightDir);

    return calculatePhongVec(light, shading.x, shading.y, 0.0f, false);
}

vec3 calculatePointLight(LightProperties light){
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    vec2 shading = calculateShading(lightDir);

    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

    return calculatePhongVec(light, shading.x, shading.y, attenuation, true);
}

vec3 calculateSpotLight(LightProperties light){
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    vec2 shading = calculateShading(lightDir);

    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = light.cutOff.x - light.cutOff.y;
    float intensity = clamp((theta - light.cutOff.y) / epsilon, 0.0, 1.0);

    return calculatePhongVec(light, shading.x, shading.y, attenuation * intensity, true);
}

vec3 calculatePhongVec(LightProperties light, float diff, float spec, float attenuation, bool applyAttenuation){
    vec3 ambient, diffuse, specular;
    if (hasDiffuseTex) {
        ambient = light.ambient.xyz * vec3(texture(diffuseTex, textureCoord));
        diffuse = light.diffuse.xyz * diff * vec3(texture(diffuseTex, textureCoord));
    }
    else {
        ambient = light.ambient.xyz;
        diffuse = light.diffuse.xyz * diff;
    }

    if (hasSpecularTex) {
        specular = light.specular.xyz * spec * vec3(texture(specularTex, textureCoord));
    }
    else {
        specular = light.specular.xyz * spec;
    }

    if (applyAttenuation){
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    return ambient + diffuse + specular;
}

vec2 calculateShading(vec3 lightDir){
    float diff = max(dot(normalVec, lightDir), 0.0);

    vec3 viewDir = normalize(camera.position.xyz + camera.front.xyz - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normalVec, halfwayDir), 0.0), phongProperties.shininess);
    return vec2(diff, spec);
}