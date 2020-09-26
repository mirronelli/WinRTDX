cbuffer modelData : register(b2)
{
	row_major matrix modelToWorld;
};

cbuffer levelData : register(b0)
{
	row_major matrix worldView;
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
	float4x4 finalTransform = mul(modelToWorld, worldView);
	return mul(pos, finalTransform);
}