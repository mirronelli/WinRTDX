#include "9_Functions.hlsli"
#include "9_Structures.hlsli"

Texture2D inTextureDiffuse : register(t0);
Texture2D inTextureSpecular : register(t1);
Texture2D inTextureNormal: register(t2);
SamplerState inSampler;

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

float4 main(PixelTexturedWithNormal input) : SV_TARGET
{
    const float3 materialColor = (float3) (instance.hasTextureMap ? inTextureDiffuse.Sample(inSampler, input.textureCoordinates) : instance.diffuseColor);
    const float3 specularColor = (float3) (instance.hasSpecularMap ? inTextureSpecular.Sample(inSampler, input.textureCoordinates) : instance.specularColor);
    
    const float3 light = mLightIntensity(
        materialColor,
        (float3) level_light.ambientLight,
        (float3) level_light.lightColor,
        (float3) level_light.lightPosition,
        input.worldPosition,
        input.normal,
        specularColor,
        level_light.diffueseIntensity,
        level_light.attenuationQuadratic,
        level_light.attenuationLinear,
        level_light.attenuationConstant,
        instance.specularPower,
        frame.cameraPosition
    );
        
    return float4(light, 1.0f);
}