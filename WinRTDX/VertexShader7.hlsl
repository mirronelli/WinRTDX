struct Input
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 textureCoordinates : TEXCOORD;
};

struct VsOutput
{
    float2 textureCoordinates : TEXCOORD;
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

cbuffer modelData : register(b1)
{
    row_major matrix worldTransform;
};

cbuffer levelData : register(b0)
{
    row_major matrix viewPerspectiveTransform;
};

VsOutput main(Input input)
{
    //float4x4 finalTransform = mul(worldTransform, viewPerspectiveTransform);
	
    VsOutput output;
    
    output.worldPosition = (float3) mul(float4(input.position, 1.0f), worldTransform);
    output.position = mul(float4(output.worldPosition, 1.0f), viewPerspectiveTransform);
    output.normal = mul(input.normal, (float3x3) worldTransform);
    output.textureCoordinates = input.textureCoordinates;
	
    return output;
}