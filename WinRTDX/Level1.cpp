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
		{  -1.0f,  1.0f, 0.0f, 1.0f,		1.0f,  .0f,  .0f, 1.0f },
		{  -0.6f,  0.5f, 0.0f, 1.0f,		 .0f, 1.0f,  .0f, 1.0f },
		{  -0.8f,  0.2f, 0.0f, 1.0f,		 .0f,  .0f, 1.0f, 1.0f },
		{  -0.4f,  0.0f, 0.0f, 1.0f,		1.0f,  .0f,  .0f, 1.0f },
		{  -0.6f, -0.5f, 0.0f, 1.0f,		1.0f, 1.0f,  .0f, 1.0f },
		{   .45f, -0.8f, 0.0f, 1.0f,		0.0f,  .0f, 1.0f, 1.0f },
	};

	// Describe Input Layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		 {"POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,										D3D11_INPUT_PER_VERTEX_DATA, 0},
		 {"COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
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

	// Create Vertex Buffer over vertices
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA srdVertices = { vertices, 0, 0 };
	m_graphics->Device()->CreateBuffer(&vertexBufferDesc, &srdVertices, m_vertexBuffer.put());
	
	// define indices
	const unsigned short indices[] = {
		0, 1, 2,
		3, 5, 4,
		5, 2, 1
	};

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA srdIndices = { indices, 0, 0 };
	m_graphics->Device()->CreateBuffer(&indexBufferDesc, &srdIndices, m_indexBuffer.put());
}

void Dx::Levels::Level1::Render()
{
	float color[4]{ m_red * c_maxColorIntensity, m_green * c_maxColorIntensity, m_blue * c_maxColorIntensity, .0 };
	m_graphics->SetColor(color);

	UINT strideVertices = sizeof(Vertex);
	UINT offsetVertices = 0;
	ID3D11Buffer* vertexBuffers[1] = { m_vertexBuffer.get() };
	m_graphics->Context()->IASetVertexBuffers(0, 1, vertexBuffers, &strideVertices, &offsetVertices);
	m_graphics->Context()->IASetIndexBuffer(m_indexBuffer.get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	m_graphics->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_graphics->Context()->DrawIndexed(9, 0, 0);
}

void Dx::Levels::Level1::Update(float delta)
{
	m_elapsedTime += delta;
	if (m_elapsedTime > m_effectDuration)
		m_elapsedTime -= m_effectDuration;

	float progress = m_elapsedTime / m_effectDuration;

	if (progress < c_oneSixth)														//phase 1	
	{
		m_red = progress * 6;															// red up	
		m_green = 0;																		// green zero
		m_blue = 1.0;																		// blue 1
	}
	else if (progress < c_twoSixths)												//phase 2
	{
		m_red = 1.0f;																		// red 1
		m_green = 0;																		// green zero
		m_blue = 1.0f - (progress-c_oneSixth) * 6;								// blue down
	}
	else if (progress < c_threeSixths)											//phase 3
	{
		m_red = 1.0;																		// red 1	
		m_green = (progress - c_twoSixths) * 6;									// green up
		m_blue = 0;																			// blue 0
	}
	else if (progress < c_fourSixths)											// phase 4
	{
		m_red = 1.0f - (progress - c_threeSixths) * 6;							// red down
		m_green = 1.0;																		// green 1
		m_blue = 0;																			// blue 0
	}
	else if (progress < c_fiveSixths)											// phase 5
	{
		m_red = 0;																			// red 0
		m_green = 1;																		// green 1
		m_blue = (progress - c_fourSixths) * 6;									// blue up
	}
	else																					// phase 6
	{
		m_red = 0;																			// red 0
		m_green = 1.0 -(progress - c_fiveSixths) * 6;							// green down
		m_blue = 1;																			// blue up
	}
}