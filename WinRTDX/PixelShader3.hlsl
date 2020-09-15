float4 main(uint id : SV_PRIMITIVEID) : SV_TARGET
{
	return float4(id/2 * 0.15f, 1-(id/2)*0.15f, 1.0f - id/2 * 0.15, 1.0f);
}