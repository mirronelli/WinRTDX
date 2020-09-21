Texture2D inTexture;
SamplerState inSampler;

struct VSOutput
{
	
	float4 position : SV_Position;
	float3 normal : NORMAL;
};


float4 main(float2 textureCoordinates : TEXCOORD) : SV_TARGET
{
	return inTexture.Sample ( inSampler, textureCoordinates);
}