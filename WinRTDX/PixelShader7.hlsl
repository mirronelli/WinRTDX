Texture2D inTexture;
SamplerState inSampler;

struct VsOutput
{
    float2 textureCoordinates : TEXCOORD;
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

cbuffer levelData : register(b0)
{
    float4 lightPosition;
    float4 lightColor;
    float4 ambientLight;
    float diffueseIntensity;
    float attenuationQuad;
};

float4 main(VsOutput input) : SV_TARGET
{
    const float3 vectorToLight = (float3) lightPosition - input.worldPosition;
    const float distanceToLight = length(vectorToLight);
    const float3 normalizedNormal = normalize(input.normal);
    const float3 normalizedvectorToLight = normalize(vectorToLight);
    
    const float angleCoefficient = max(0.0f, dot(normalizedvectorToLight, normalizedNormal));
    
    const float attenuation = 1.0f / (attenuationQuad * distanceToLight * distanceToLight);
    const float3 diffusedColor = (float3)lightColor * diffueseIntensity * attenuation * angleCoefficient;
    
    const float3 materialColor = (float3)inTexture.Sample(inSampler, input.textureCoordinates);
    return float4(materialColor * saturate(diffusedColor + (float3)ambientLight), 1.0f);
}