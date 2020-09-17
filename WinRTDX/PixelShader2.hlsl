struct VS_OUTPUT
{
	float4 Position  : SV_POSITION;
	float4 Color     : COLOR0;
};


float4 main(VS_OUTPUT Input) : SV_TARGET
{
	return Input.Color;
}