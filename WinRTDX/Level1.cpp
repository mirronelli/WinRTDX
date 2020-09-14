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

	// Create Vertex Buffer over vertices
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };
	m_graphics->Device()->CreateBuffer(&bd, &srd, m_vertexBuffer.put());
	
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
}

void Dx::Levels::Level1::Render()
{
	float color[4]{ m_red * c_maxColorIntensity, m_green * c_maxColorIntensity, m_blue * c_maxColorIntensity, .0 };
	m_graphics->SetColor(color);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vertexBuffers[1] = { m_vertexBuffer.get() };
	m_graphics->Context()->IASetVertexBuffers(0, 1, vertexBuffers, &stride, &offset);

	m_graphics->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	m_graphics->Context()->Draw(6, 0);

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
