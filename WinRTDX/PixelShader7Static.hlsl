#include "Functions.hlsl"

float4 main(VsColorOutput input, uint id : SV_PRIMITIVEID) : SV_TARGET
{
    return float4(input.color, 1.0f);
}