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

PixelTexturedWithNormalTangent main(VertexTexturedWithNormalTangent input)
{
    PixelTexturedWithNormalTangent output;
    
    output.worldPosition = (float3) mul(float4(input.position, 1.0f), instance.worldTransform);
    output.position = mul(float4(output.worldPosition, 1.0f), frame.viewPerspectiveTransform);
    output.textureCoordinates = input.textureCoordinates;
    
    output.normal  =    (float3) mul(float4(input.normal, 0),    instance.worldTransform);
    output.tangent =    (float3) mul(float4(input.tangent, 0),   instance.worldTransform);
    output.bitangent =  (float3) mul(float4(input.bitangent, 0), instance.worldTransform);
    return output;
}