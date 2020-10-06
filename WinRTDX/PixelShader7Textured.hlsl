#include "Functions.hlsl"

Texture2D inTexture;
SamplerState inSampler;

// once per level
cbuffer light : register(b0)
{
    LightBuffer lightBuffer;
};

// once per frame
cbuffer levelData : register(b1)
{
    PixelPerFrameBuffer level;
};

// once per drawable
cbuffer drawable : register(b2)
{
    DrawablePixelBuffer drawable;
};

float4 main(VsTextureOutput input) : SV_TARGET
{
    const float3 materialColor = (float3)inTexture.Sample(inSampler, input.textureCoordinates);
    const float3 light = mLightIntensity(
        (float3) lightBuffer.ambientLight,
        (float3) lightBuffer.lightColor,
        (float3) lightBuffer.lightPosition,
        input.worldPosition,
        input.normal,
        lightBuffer.diffueseIntensity,
        lightBuffer.attenuationQuadratic,
        lightBuffer.attenuationLinear,
        lightBuffer.attenuationConstant,
        drawable.reflectiveness,
        drawable.reflectivePower,
        level.cameraPosition
    );
        
    return float4(materialColor * light, 1.0f);
}