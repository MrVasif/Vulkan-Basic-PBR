/*

    Lightning Models Referenced From : http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html

*/

#version 450

#define PI 3.1415926535897932384626433832795

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D metallicMap;
layout(binding = 4) uniform sampler2D roughnessMap;
layout(binding = 5) uniform sampler2D aoMap;
layout(binding = 6) uniform samplerCube irradianceCube;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normalCoords;

layout(location = 0) out vec4 outColor;

float GGX_NormalDistribution(vec3 N,vec3 H,float a)
{
    float alpha = a * a;
    float _dotNH = max(0.0,dot(N,H));
    float dotNH = _dotNH * _dotNH;

    float denominator = (dotNH * (alpha - 1) + 1);
    denominator = PI * denominator * denominator;

    return alpha / denominator;
}

float GeometrySchlickGGX(float dotNV, float k)
{
    float nominator = dotNV;
    float denominator = nominator * (1.0 - k) + k;

    return nominator / denominator;
}

vec3 FresnelSchlick(float cosTheta,vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta,5.0);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L,float k)
{
    float dotNV = max(0.0,dot(N,V));
    float dotNL = max(0.0,dot(N,L));
    float eq1 = GeometrySchlickGGX(dotNV,k);
    float eq2 = GeometrySchlickGGX(dotNL,k);

    return eq1 * eq2;
}

void main() {

    vec3 albedo = pow(texture(texSampler,fragTexCoord).rgb,vec3(2.2));
    float metallic = texture(metallicMap,fragTexCoord).r;
    float roughness = texture(roughnessMap,fragTexCoord).r;
    float ambientOcclusion = texture(aoMap,fragTexCoord).r;

    vec3 lightColor = vec3(1.0f); 
    vec3 lightPosition[6];
    lightPosition[0] = vec3(1.0f,4.0f,1.0f);
    lightPosition[1] = vec3(-2.0f,1.0f,-3.5f);
    lightPosition[2] = vec3(4.0,-3.0f,2.0f);
    lightPosition[3] = vec3(-2.0f,-1.0f,0.0f);
    lightPosition[4] = vec3(-1.0f,2.5f,3.0f);
    lightPosition[5] = vec3(-4.0f,3.0f,-2.0f);
    
    vec3 N = normalize(normalCoords);
    vec3 V = normalize(vec3(-16.0f,6.0f,2.0) - vec3(0.0f,0.0f,0.0f));

    vec3 F0 = mix(vec3(0.04),albedo,metallic);

    vec3 totalLightning = vec3(0.0);

    for(int i = 0; i < 6; i++)
    {
        vec3 L = normalize(lightPosition[i] - vec3(0.0f,0.0f,0.0f));
        vec3 H = normalize(V + L);

        float distance = length(lightPosition[i] - vec3(0.0f,0.0f,0.0f));
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColor * attenuation;

        float normalDistribution = GGX_NormalDistribution(N,H,roughness);
        float geometryFunction = GeometrySmith(N,V,L,roughness);
        vec3 F = FresnelSchlick(max(0.0,dot(H,V)),F0);

        vec3 numerator = normalDistribution * geometryFunction * F;
        float denominator = 4.0 * max(0.0,dot(N,V)) * max(0.0,dot(N,L));
        vec3 specular = numerator / max(0.001,denominator);

        vec3 kS = F;
        vec3 kD = vec3(1.0f) - kS;
        kD *= 1.0 - metallic;

        float dotNL = max(0.0,dot(N,L));
        totalLightning += (kD * albedo / PI + specular) * radiance * dotNL;
    }

    // Multiplying with texture(irradianceCube,-N) is not good,but it replace IBL for now.    
    vec3 ambient = albedo * ambientOcclusion *  (texture(irradianceCube,-N).rgb / 4.0f);
    vec3 finalColor = ambient + totalLightning;

    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor,vec3(1.0/2.2));

    outColor = vec4(finalColor,1.0);

}