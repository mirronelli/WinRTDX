Texture2D inTexture;
SamplerState inSampler;

struct VsOutput
{
    float3 color : COLOR;
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

static const float3 lightPosition = { 0.0f, 0.0f, 0.0f };
static const float3 lightColor = { 1.0f, 0.5f, 1.0f };
static const float3 ambientLight = { 0.08f, 0.08f, 0.08f };

static const float diffueseIntensity = 1.0f;

static const float attenuationConst = 0.0f;
static const float attenuationLinearity = 0.0;
static const float attenuationQuad = 0.00005;

float4 main(VsOutput input) : SV_TARGET
{
    const float3 vectorToLight = lightPosition - input.worldPosition;
    const float distanceToLight = length(vectorToLight);
    const float3 normalizedNormal = normalize(input.normal);
    const float3 normalizedvectorToLight = normalize(vectorToLight);
    
    const float angleCoefficient = max(0.0f, dot(normalizedvectorToLight, normalizedNormal));
    
    const float attenuation = 1.0f / (attenuationConst + attenuationLinearity * distanceToLight + attenuationQuad * distanceToLight * distanceToLight);
    const float3 diffusedColor = lightColor * diffueseIntensity * attenuation * angleCoefficient;
    
    const float3 materialColor = input.color;
    return float4(materialColor * saturate(diffusedColor + ambientLight), 1.0f);
}