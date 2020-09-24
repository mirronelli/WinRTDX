Texture2D inTexture;
SamplerState inSampler;

struct VSOutput
{
	
    float4 position : SV_Position;
    float3 normal : NORMAL;
};

static const float3 lightPosition = { 0.0f, 200.0f, 0.0f };
static const float3 lightColor = { 1.0f, 1.0f, 1.0f };
static const float3 ambientLight = { 0.1f, 0.1f, 0.1f };

static const float diffueseIntensity = 1.0f;

static const float attenuationConst = 1.0f;
static const float attenuationLinearity = 1.0f;
static const float attenuationQuad = 1.0f;

float4 main(float2 textureCoordinates : TEXCOORD, float4 pixelPosition : SV_Position, float3 normal : NORMAL) : SV_TARGET
{
    const float3 vectorToLight = lightPosition - pixelPosition;
    const float distanceToLight = length(vectorToLight);
    const float3 vectorToLightNormalized = vectorToLight / distanceToLight;
    
    //const float3 materialColor = inTexture.Sample(inSampler, textureCoordinates);
    
    const float attenuation = attenuationConst + attenuationLinearity * distanceToLight + attenuationQuad * distanceToLight * distanceToLight;
    const float3 diffusedColor = lightColor * diffueseIntensity * max(0.0f, dot(vectorToLightNormalized, normal)) / attenuation;
    
    //const float3 diffusedColor = (float3) materialColor * (max(0.0f, dot(vectorToLightNormalized, normal)) + ambientLight);
    
    //return inTexture.Sample(inSampler, textureCoordinates);
    return float4(saturate(diffusedColor + ambientLight), 1.0f);
    //return float4(materialColor * saturate(ambientLight + lightColor * max(0, dot(vectorToLightNormalized, normal))), 1.0f);
}