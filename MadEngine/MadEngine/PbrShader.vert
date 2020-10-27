#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform PushConstants
{
    float speed;
} pushConsts;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normalCoords;
layout(location = 3) out vec3 reflectedVector;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition,1.0);
    vec4 worldPosition = ubo.model * vec4(inPosition,1.0); 
    fragColor = inColor;
    normalCoords = mat3(ubo.proj * ubo.view * ubo.model) * inNormal;
    fragTexCoord = inTexCoord;
    
    vec3 viewVector = normalize(worldPosition.xyz - vec3(1.0f,2.0f,1.0f));
    reflectedVector = reflect(viewVector,normalize(normalCoords));
}