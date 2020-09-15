cbuffer constantData {
	row_major matrix transform;
	float4 colors[6];
};

float4 main(uint id : SV_PRIMITIVEID) : SV_TARGET
{
	return colors[id / 2];
}