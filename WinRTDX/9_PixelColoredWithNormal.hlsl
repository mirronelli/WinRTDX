#include "9_Functions.hlsli"
#include "9_Structures.hlsli"

cbuffer light : register(b0)
{
    PixelPerLevelLightBuffer level_light;
};

cbuffer level : register(b1)
{
    PixelPerFrameCameraBuffer frame;
};

// once per drawable
cbuffer drawable : register(b2)
{
    PixelPerInstanceAllColorsBuffer instance;
};

float4 main(PixelColoredWithNormal input) : SV_TARGET
{
    const float3 light = mLightIntensity(
        (float3) input.color,
        (float3) level_light.ambientLight,
        (float3) level_light.lightColor,
        (float3) level_light.lightPosition,
        (float3) instance.specularColor,
        input.worldPosition,
        input.normal,
        level_light.diffueseIntensity,
        level_light.attenuationQuadratic,
        level_light.attenuationLinear,
        level_light.attenuationConstant,
        instance.reflectiveness,
        instance.reflectivePower,
        frame.cameraPosition
    );
        
    return float4(light, 1.0f);
}