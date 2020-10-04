#pragma once

namespace Dx::Drawables
{
	enum class VertexType
	{
		None,

		Simple,
		SimpleWithNormal,

		Colored,
		ColoredWithNormal,

		Textured,
		TextuedWithNormal
	};

	struct VertexSimple
	{
		DirectX::XMFLOAT3	position;
	};
	struct VertexSimpleWithNormal : VertexSimple
	{
		DirectX::XMFLOAT3 normal;
	};

	struct VertexColored : VertexSimple
	{
		DirectX::XMFLOAT3	color;
	};
	struct VertexColoredWithNormal : VertexSimpleWithNormal, VertexColored {};

	struct VertexTextured : VertexSimple
	{
		DirectX::XMFLOAT2 textureCoordinates;
	};
	struct VertexTexturedWithNormal : VertexSimpleWithNormal, VertexTextured  {};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> IedsSimple = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> IedsSimpleWithNormal = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> IedsColored = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> IedsColoredWithNormal = {
				{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> IedsTextured = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static std::vector<D3D11_INPUT_ELEMENT_DESC> IedsTexturedWithNormal = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		DirectX::XMFLOAT3 cameraPosition;
	};

	struct WorldTransform
	{
		DirectX::XMMATRIX worldTransform;
	};

	struct ColorSpecular
	{
		DirectX::XMFLOAT4 color;
		float	reflectiveness;
		float reflectionPower;
		float padding1;
		float padding2;
	};

	struct Specular
	{
		float	reflectiveness;
		float reflectionPower;
		float padding1;
		float padding2;
		DirectX::XMFLOAT4 padding3;
	};
}
