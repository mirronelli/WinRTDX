struct VS_OUTPUT
{
	float4 Position  : SV_POSITION;
	float4 Color     : COLOR0;
};

float4 main(VS_OUTPUT data) : SV_TARGET
{
	return data.Color;
}