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
    PixelPerInstanceColorSpecularBuffer instance;
};

float4 main(PixelSimpleWithNormal input) : SV_TARGET
{
    const float3 materialColor = (float3) instance.color;
    const float3 light = mLightIntensity(
        (float3) level_light.ambientLight,
        (float3) level_light.lightColor,
        (float3) level_light.lightPosition,
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
        
    return float4(materialColor * light, 1.0f);
}