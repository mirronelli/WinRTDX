#include "8_Functions.hlsli"
#include "8_Structures.hlsli"

float4 main(PixelWithColor input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}