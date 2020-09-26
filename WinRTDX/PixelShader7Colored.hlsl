#include "Functions.hlsl"

Texture2D inTexture;
SamplerState inSampler;

cbuffer light : register(b0)
{
    Light lightBuffer;
};

float4 main(VsColorOutput input) : SV_TARGET
{
    const float3 materialColor = input.color;
    const float3 light = mLightIntensity(
        (float) lightBuffer.ambientLight,
        (float3) lightBuffer.lightColor,
        (float3) lightBuffer.lightPosition,
        input.worldPosition,
        input.normal,
        lightBuffer.diffueseIntensity,
        lightBuffer.attenuationQuadratic,
        lightBuffer.attenuationLinear,
        lightBuffer.attenuationConstant
    );
        
    return float4(materialColor * light, 1.0f);
}