struct VS_INPUT
{
	float4 Position  : POSITION0;
	float4 Color     : COLOR0;
};

struct VS_OUTPUT
{
	float4 Position  : SV_POSITION;
	float4 Color     : COLOR0;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = Input.Position;
	Output.Color = Input.Color;

	return Output;
}