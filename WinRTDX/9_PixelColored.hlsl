#include "9_Functions.hlsli"
#include "9_Structures.hlsli"

float4 main(PixelColored input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}