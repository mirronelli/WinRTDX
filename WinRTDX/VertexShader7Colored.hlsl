#include "Functions.hlsl"

// once per level slot 0 reserved

// once per frame
cbuffer level : register(b1)
{
    LevelBuffer level;
};

// once per drawable
cbuffer drawable : register(b2)
{
    DrawableBuffer drawable;
};

VsColorOutput main(VsColorInput input)
{
    VsColorOutput output;
    
    output.worldPosition = (float3) mul(float4(input.position, 1.0f), drawable.worldTransform);
    output.position = mul(float4(output.worldPosition, 1.0f), level.viewPerspectiveTransform);
    output.normal = mul(input.normal, (float3x3) drawable.worldTransform);
    output.color = input.color;
	
    return output;
}