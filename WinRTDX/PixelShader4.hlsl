cbuffer constantData {
	row_major matrix transform;
	float4 colors[6];
};

float4 main(uint id : SV_PRIMITIVEID) : SV_TARGET
{
	//float4 color = (1.0, 1.0, 0.0, 0,0);
	//return color;
	return (colors[id/2]);
	//return float4(1 - (id/2) * 32 / 256.0, (id/2) * 32 / 256.0, 0, 0);
}