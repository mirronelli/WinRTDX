#include "9_Functions.hlsli"
#include "9_Structures.hlsli"

// once per level slot 0 reserved

cbuffer frame : register(b1)
{
    VertexPerFrameTransformBuffer frame;
};

cbuffer instance : register(b2)
{
    VertexPerInstanceTransformBuffer instance;
};

PixelSimpleWithNormal main(VertexSimpleWithNormal input)
{
    PixelSimpleWithNormal output;
    
    output.worldPosition = (float3) mul(float4(input.position, 1.0f), instance.worldTransform);
    output.position = mul(float4(output.worldPosition, 1.0f), frame.viewPerspectiveTransform);
    output.normal = mul(input.normal, (float3x3) instance.worldTransform);
	
    return output;
}


