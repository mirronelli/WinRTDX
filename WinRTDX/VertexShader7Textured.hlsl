#include "Functions.hlsl"

cbuffer modelData : register(b1)
{
    row_major matrix worldTransform;
};

cbuffer levelData : register(b0)
{
    row_major matrix viewPerspectiveTransform;
};

VsTextureOutput main(VsTextureInput input)
{
    VsTextureOutput output;
    
    output.worldPosition = (float3) mul(float4(input.position, 1.0f), worldTransform);
    output.position = mul(float4(output.worldPosition, 1.0f), viewPerspectiveTransform);
    output.normal = mul(input.normal, (float3x3) worldTransform);
    output.textureCoordinates = input.textureCoordinates;
	
    return output;
}