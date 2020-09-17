#include "pch.h"
#include "Cube.h"

void Dx::Cube::RegisterResources() {
	std::vector<D3D11_INPUT_ELEMENT_DESC> ieds = {
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,										D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	std::vector<Vertex> vertices = {
		{ -1.0f,	 1.0f,	-1.0f,	 1.0f },
		{ 1.0f,	 1.0f,	-1.0f,	 1.0f },
		{ 1.0f,	-1.0f,	-1.0f,	 1.0f },
		{ -1.0f,	-1.0f,	-1.0f,	 1.0f },
		{ -1.0f,	 1.0f,	 1.0f,	 1.0f },
		{ 1.0f,	 1.0f,	 1.0f,	 1.0f },
		{ 1.0f,	-1.0f,	 1.0f,	 1.0f },
		{ -1.0f,	-1.0f,	 1.0f,	 1.0f },
	};

	std::vector<UINT> indices = {
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

	m_constants = {
		DirectX::XMMatrixIdentity(),
		{
			DirectX::XMFLOAT4(1.f, 0.5f, 0.5f, 1.f),
			DirectX::XMFLOAT4(0.5f, 1.f, 0.5f, 1.f),
			DirectX::XMFLOAT4(0.5f, 0.5f, 1.f, 1.f),
			DirectX::XMFLOAT4(1.f, 0.5f, 1.f, 1.f),
			DirectX::XMFLOAT4(0.5f, 1.f, 1.f, 1.f),
			DirectX::XMFLOAT4(1.f, 1.f, 0.5f, 1.f),
		}
	};

	m_vertexBuffer = std::make_unique<VertexBuffer<Vertex>>(m_graphics, vertices);
	m_indexBuffer = std::make_unique<IndexBuffer>(m_graphics, indices);
	m_inputLayout = std::make_unique<InputLayout>(m_graphics, ieds, m_vertexShaderByteCode);
	m_constantBuffer = std::make_unique<ConstantBuffer<Constants>>(m_graphics, m_constants);
}

void Dx::Cube::AttachResources() {
	m_vertexBuffer->AttachPrivate();
	m_indexBuffer->AttachPrivate();
	m_inputLayout->AttachPrivate();
	m_constantBuffer->AttachPrivate();
}

void Dx::Cube::Draw() {
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_constants.matrix =  DirectX::XMMatrixRotationX(m_rotationX);
	m_constants.matrix *= DirectX::XMMatrixRotationZ(m_rotationY);
	m_constants.matrix *= DirectX::XMMatrixRotationY(m_rotationZ);
	m_constants.matrix *= DirectX::XMMatrixTranslation(m_x, m_y, m_z);
	m_constants.matrix *= DirectX::XMMatrixPerspectiveFovLH(1.2f, m_graphics->Width() / m_graphics->Height(), 1.f, 100.0f);
	m_constantBuffer->Update(m_constants);
	m_context->DrawIndexed(36, 0, 0);
};

void Dx::Cube::DrawAt(float x, float y, float z) {
	//m_x = x;
	//m_y = y;
	m_z = z;

	Draw();
}

void Dx::Cube::Update(float delta) {
	float step = delta / 1;

	m_rotationX = fmod (m_rotationX + step * m_rotationSpeedX, DirectX::XM_2PI);
	m_rotationY = fmod (m_rotationY + step * m_rotationSpeedY, DirectX::XM_2PI);
	m_rotationX = fmod (m_rotationZ + step * m_rotationSpeedZ, DirectX::XM_2PI);

	m_x += m_speedX * step;
	m_y += m_speedY * step;
	m_z += m_speedZ * step;
};
