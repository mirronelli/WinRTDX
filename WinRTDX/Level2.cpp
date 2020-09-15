#include "pch.h"
#include "Level2.h"
#include "IO.h"
#include <DirectXMath.h>


concurrency::task<void> Dx::Levels::Level2::Load()
{
	return concurrency::create_task([this]
		{
			m_compiledVertexShader = IO::ReadFile(L"VertexShader2.cso");
			m_graphics->SetVertexShader(m_graphics->CreateVertexShader(m_compiledVertexShader));

			m_compiledPixelShader = IO::ReadFile(L"PixelShader2.cso");
			m_graphics->SetPixelShader(m_graphics->CreatePixelShader(m_compiledPixelShader));
		}
	);
}

void Dx::Levels::Level2::SetupModel()
{
#pragma region vertices

	// Describe Input Layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		 {"POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,										D3D11_INPUT_PER_VERTEX_DATA, 0},
		 {"COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	com_ptr<ID3D11InputLayout> inputLayout;
	m_device->CreateInputLayout(
		ieds,
		ARRAYSIZE(ieds),
		m_compiledVertexShader.data(),
		m_compiledVertexShader.Length(),
		inputLayout.put()
	);
	m_context->IASetInputLayout(inputLayout.get());

	// define vertices and its buffer
	Vertex vertices[] = {
		{  -0.5f,  0.5f, 1.0f, .50f,		1.0f,  .0f,  .0f, 1.0f },
		{   0.5f, -0.5f, 1.0f, .50f,		 .0f, 1.0f,  .0f, 1.0f },
		{  -0.5f, -0.5f, 1.0f, .50f,		 .0f,  .0f, 1.0f, 1.0f },
		{   0.5f,  0.5f, 1.0f, .50f,		1.0f, 1.0f, 1.0f, 1.0f },
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srdVertices = { vertices, 0, 0 };

	m_device->CreateBuffer(&vertexBufferDesc, &srdVertices, m_vertexBuffer.put());

#pragma endregion
	
#pragma region indices

	const unsigned short indices[] = {
		0, 1, 2,
		0, 3, 1,
	};

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	
	D3D11_SUBRESOURCE_DATA srdIndices = { indices, 0, 0 };
	
	m_device->CreateBuffer(&indexBufferDesc, &srdIndices, m_indexBuffer.put());

#pragma endregion

#pragma region matrices

	D3D11_BUFFER_DESC	transformDesc{ 0 };
	transformDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	transformDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	transformDesc.CPUAccessFlags = 0;
	transformDesc.ByteWidth = sizeof(m_matrices);
	
	D3D11_SUBRESOURCE_DATA transformSrd{ 0 };
	transformSrd.pSysMem = &m_matrices;
	
	m_device->CreateBuffer(&transformDesc, &transformSrd, m_constantBuffer.put());

#pragma endregion


	UINT strideVertices = sizeof(Vertex);
	UINT offsetVertices = 0;
	ID3D11Buffer* vertexBuffers[1] = { m_vertexBuffer.get() };
	m_context->IASetVertexBuffers(0, 1, vertexBuffers, &strideVertices, &offsetVertices);
	
	m_context->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);

	ID3D11Buffer* constantBuffers[1] = { m_constantBuffer.get() };
	m_context->VSSetConstantBuffers(0, 1, constantBuffers);
	
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Dx::Levels::Level2::Update(float delta)
{
	m_elapsedTime += delta;
	while (m_elapsedTime > m_effectDuration)
		m_elapsedTime -= m_effectDuration;

	float progress = m_elapsedTime / m_effectDuration * DirectX::XM_2PI;
	m_matrices = DirectX::XMMatrixRotationZ(progress);
}

void Dx::Levels::Level2::Render()
{
	float color[4]{ .4f, .2f, .2f, .2f};
	m_graphics->StartFrame(color);
	m_context->UpdateSubresource(m_constantBuffer.get(), 0, 0, &m_matrices, 0, 0);
	m_context->DrawIndexed(6, 0, 0);
}