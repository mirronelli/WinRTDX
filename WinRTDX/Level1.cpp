#include "pch.h"
#include "Level1.h"
#include "IO.h"

concurrency::task<void> Dx::Levels::Level1::Load()
{
	return concurrency::create_task([this]
		{
			m_compiledVertexShader = IO::ReadFile(L"VertexShader.cso");
			m_graphics->SetVertexShader(m_graphics->CreateVertexShader(m_compiledVertexShader));

			m_compiledPixelShader = IO::ReadFile(L"PixelShader.cso");
			m_graphics->SetPixelShader(m_graphics->CreatePixelShader(m_compiledPixelShader));
		}
	);
}

void Dx::Levels::Level1::SetupModel()
{
	Vertex vertices[] = {
		{  -0.9f, 0.9f, 0.0f },
		{  -0.6f,  0.5f, 0.0f },
		{ -0.8f, 0.5f, 0.0f },
		{ 0.0f, 0.5f, 0.0f },
		{ 0.45f, -0.5f, 0.0f },
		{ -0.45f, -0.5f, 0.0f },
	};

	// Create Vertex Buffer over vertices
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };
	m_graphics->Device()->CreateBuffer(&bd, &srd, m_vertexBuffer.put());
	
	// Describe Input Layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		 {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	com_ptr<ID3D11InputLayout> inputLayout;
	m_graphics->Device()->CreateInputLayout(
		ieds,
		ARRAYSIZE(ieds),
		m_compiledVertexShader.data(),
		m_compiledVertexShader.Length(),
		inputLayout.put()
	);
	m_graphics->Context()->IASetInputLayout(inputLayout.get());
}

void Dx::Levels::Level1::Render()
{
	float color[4]{ 1, .5, .5, 0.5 };
	m_graphics->SetColor(color);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vertexBuffers[1] = { m_vertexBuffer.get() };
	m_graphics->Context()->IASetVertexBuffers(0, 1, vertexBuffers, &stride, &offset);

	m_graphics->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_graphics->Context()->Draw(6, 0);

}

void Dx::Levels::Level1::Update(UINT32 deltaTime)
{
}
