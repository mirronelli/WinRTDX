#include "pch.h"
#include "Level3.h"
#include "IO.h"
#include <DirectXMath.h>

using namespace Dx::Attachables;

concurrency::task<void> Dx::Levels::Level3::Load()
{
	return concurrency::create_task([this]
		{
			m_vertexShader = VertexShader::Load(1, true, m_graphics, L"VertexShader3.cso");
			m_pixelShader = PixelShader::Load(1, true, m_graphics, L"PixelShader3.cso");
		}
	);
}

void Dx::Levels::Level3::CreateVertices()
{
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,										D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	com_ptr<ID3D11InputLayout> inputLayout;
	m_device->CreateInputLayout(
		ieds,
		ARRAYSIZE(ieds),
		m_vertexShader->Data(),
		m_vertexShader->Length(),
		inputLayout.put()
	);
	m_context->IASetInputLayout(inputLayout.get());

	// define vertices and its buffer
	Vertex vertices[] = {
		{ -1.0f,	 1.0f,	-1.0f,	 1.0f },
		{ 1.0f,	 1.0f,	-1.0f,	 1.0f },
		{ 1.0f,	-1.0f,	-1.0f,	 1.0f },
		{ -1.0f,	-1.0f,	-1.0f,	 1.0f },
		{ -1.0f,	 1.0f,	 1.0f,	 1.0f },
		{ 1.0f,	 1.0f,	 1.0f,	 1.0f },
		{ 1.0f,	-1.0f,	 1.0f,	 1.0f },
		{ -1.0f,	-1.0f,	 1.0f,	 1.0f },
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srdVertices = { vertices, 0, 0 };

	m_device->CreateBuffer(&vertexBufferDesc, &srdVertices, m_vertexBuffer.put());
}

void Dx::Levels::Level3::CreateIndices()
{
	const unsigned short indices[]{
		0, 1, 2,  // front
		0, 2, 3,
		1, 5, 2,  // right
		5, 6, 2,
		0, 7, 4,  // left
		0, 3, 7,
		0,	5, 1,  // top
		0, 4, 5,
		3, 2, 7,  // bottom
		2, 6, 7,
		4, 6, 5,  // back
		4, 7, 6
	};

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA srdIndices = { indices, 0, 0 };

	m_device->CreateBuffer(&indexBufferDesc, &srdIndices, m_indexBuffer.put());
	m_vertexShader->Attach(true);
	m_pixelShader->Attach(true);
}

void Dx::Levels::Level3::CreateConstantData()
{
	D3D11_BUFFER_DESC	transformDesc{ 0 };
	transformDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	transformDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	transformDesc.CPUAccessFlags = 0;
	transformDesc.ByteWidth = sizeof(m_constantData);

	D3D11_SUBRESOURCE_DATA transformSrd{ 0 };
	transformSrd.pSysMem = &m_constantData;

	m_device->CreateBuffer(&transformDesc, &transformSrd, m_constantBuffer.put());
}

void Dx::Levels::Level3::RegisterBuffers()
{
	UINT strideVertices = sizeof(Vertex);
	UINT offsetVertices = 0;
	ID3D11Buffer* vertexBuffers[1] = { m_vertexBuffer.get() };
	m_context->IASetVertexBuffers(0, 1, vertexBuffers, &strideVertices, &offsetVertices);

	m_context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);

	ID3D11Buffer* constantBuffers[1] = { m_constantBuffer.get() };
	m_context->VSSetConstantBuffers(0, 1, constantBuffers);
	m_context->PSSetConstantBuffers(0, 1, constantBuffers);
}

void Dx::Levels::Level3::SetupModel()
{
	CreateVertices();
	CreateIndices();
	CreateConstantData();
	RegisterBuffers();
}

void Dx::Levels::Level3::Update(float delta)
{
	m_elapsedTime += delta;
	while (m_elapsedTime > m_effectDuration)
		m_elapsedTime -= m_effectDuration;

	m_progress = m_elapsedTime / m_effectDuration * DirectX::XM_2PI;
}

void Dx::Levels::Level3::DrawCube(float angle, float x, float y, float z)
{
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_constantData.matrix =
		DirectX::XMMatrixRotationX(angle) *
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixRotationY(angle) *
		DirectX::XMMatrixTranslation(x, y, z) *
		DirectX::XMMatrixPerspectiveFovLH(1.2f, m_graphics->Width() / m_graphics->Height(), 0.1f, 100.0f);
	m_context->UpdateSubresource(m_constantBuffer.get(), 0, 0, &m_constantData, 0, 0);
	m_context->DrawIndexed(36, 0, 0);
}

void Dx::Levels::Level3::Render()
{
	float color[4]{ .2f, .2f, .2f, .2f};
	m_graphics->StartFrame(color);

	DrawCube(m_progress, m_graphics->MouseX() -.5f, m_graphics->MouseY(), 8.0f);
	DrawCube(-m_progress, m_graphics->MouseX() +.5f, 0, m_graphics->MouseY() + 8.0f);
}