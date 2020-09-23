#include "pch.h"
#include "Level2.h"
#include "IO.h"
#include <DirectXMath.h>

using namespace Dx::Attachables;

concurrency::task<void> Dx::Levels::Level2::Load()
{
	return concurrency::create_task([this]
		{
			m_vertexShader = VertexShader::Load(1, true, m_graphics, L"VertexShader2.cso");
			m_pixelShader = PixelShader::Load(1, true, m_graphics, L"PixelShader2.cso");
		}
	);
}

void Dx::Levels::Level2::CreateVertices()
{
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,										D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16,									D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		{ -1.0f,	 1.0f,	1.0f,	 1.0f,	1.f,	.0f,	0.f, 0.f},
		{ 1.0f,	 1.0f,	1.0f,	 1.0f,	0.f,	1.f,	0.f, 0.f},
		{ 1.0f,	-1.0f,	1.0f,	 1.0f,	0.f,	0.f,	1.f, 0.f},
		{ -1.0f,	-1.0f,	1.0f,	 1.0f,	1.f,	1.f,	1.f, 0.f},
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srdVertices = { vertices, 0, 0 };

	m_device->CreateBuffer(&vertexBufferDesc, &srdVertices, m_vertexBuffer.put());
	m_vertexShader->Attach(true);
	m_pixelShader->Attach(true);
}

void Dx::Levels::Level2::CreateIndices()
{
	const unsigned short indices[]{
		0, 1, 2,  // front
		0, 2, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA srdIndices = { indices, 0, 0 };

	m_device->CreateBuffer(&indexBufferDesc, &srdIndices, m_indexBuffer.put());
}

void Dx::Levels::Level2::CreateConstantData()
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

void Dx::Levels::Level2::RegisterBuffers()
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

void Dx::Levels::Level2::SetupModel()
{
	CreateVertices();
	CreateIndices();
	CreateConstantData();
	RegisterBuffers();
}

void Dx::Levels::Level2::Update(float delta)
{
	m_elapsedTime += delta;
	while (m_elapsedTime > m_effectDuration)
		m_elapsedTime -= m_effectDuration;

	m_progress = m_elapsedTime / m_effectDuration * DirectX::XM_2PI;
}

void Dx::Levels::Level2::Draw(float angle, float x, float y, float z)
{
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_constantData.matrix = DirectX::XMMatrixRotationZ(angle);
	m_constantData.matrix *= DirectX::XMMatrixTranslation(x, y, z);
	m_constantData.matrix *= DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_graphics->Width() / m_graphics->Height(), 1.f, 100.0f);
	m_context->UpdateSubresource(m_constantBuffer.get(), 0, 0, &m_constantData, 0, 0);
	
	m_context->DrawIndexed(6, 0, 0);
}

void Dx::Levels::Level2::Render()
{
	float color[4]{ .4f, .2f, .2f, .2f };
	m_graphics->StartFrame(color);

	Draw(m_progress, m_graphics->MouseX(), m_graphics->MouseY(), 2.0f);
}