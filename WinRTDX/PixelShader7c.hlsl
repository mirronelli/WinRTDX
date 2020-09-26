Texture2D inTexture;
SamplerState inSampler;

struct VsOutput
{
    float3 color : COLOR;
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

float4 main(VsOutput input, uint id : SV_PRIMITIVEID) : SV_TARGET
{
    return float4(input.color, 1.0f);
}