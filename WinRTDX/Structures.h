#pragma once
#include <DirectXMath.h>

namespace Dx::Drawables
{
	struct Vertex
	{
		DirectX::XMFLOAT3	position;
	};
	struct VertexWithNormal : Vertex
	{
		DirectX::XMFLOAT3 normal;
	};
	struct VertextWithColor : Vertex
	{
		DirectX::XMFLOAT3	color;
	};
	struct VertexWithTexture : Vertex
	{
		DirectX::XMFLOAT2 textureCoordinates;
	};
	struct VertexWithNormalColor : VertexWithNormal, VertextWithColor {};
	struct VertexWithNormalTexture : VertexWithNormal, VertexWithTexture {};

	std::vector<D3D11_INPUT_ELEMENT_DESC> IedsWithNormalColor{
				{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	struct LightBuffer
	{
		DirectX::XMFLOAT4 lightPosition;
		DirectX::XMFLOAT4 lightColor;
		DirectX::XMFLOAT4 ambientLight;
		float diffueseIntensity;
		float attenuationQuadratic;
		float attenuationLinear;
		float attenuationConstant;
	};

	struct FrameConstantsBuffer
	{
		DirectX::XMMATRIX viewPerspectiveTransform;
		float3 cameraPosition;
	};

	struct WorldTransformWithSpecular{
		DirectX::XMMATRIX worldTransform;
		float	reflectiveness;
		float reflectionPower;
		float2	padding;
	};
}