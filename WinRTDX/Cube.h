#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace Dx::Attachables;
namespace Dx {

	class Cube : public Drawable
	{

	public:
		struct Vertex {
			float	x, y, z, w;
		};

		typedef struct {
			DirectX::XMFLOAT4 colors[6]; 
		} PSConstants;

		typedef struct {
			DirectX::XMMATRIX matrix;
		} VSConstants;

		Cube(std::shared_ptr<Dx::Graphics> graphics, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader,
			float x = 0, float y = 0, float z = 0,
			float speedX = 0, float speedY = 1, float speedZ = 0,
			float rotationX = 0, float rotationY = 0, float rotationZ = 0,
			float rotationSpeedX = 1., float rotationSpeedY = 1, float rotationSpeedZ = 1
		) :
			Drawable(
				graphics, vertexShader, pixelShader,
				x, y, z,
				speedX, speedY, speedZ,
				rotationX, rotationY, rotationZ,
				rotationSpeedX, rotationSpeedY, rotationSpeedZ
			)
		{};

		inline static std::vector<D3D11_INPUT_ELEMENT_DESC> Ieds {
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		inline static std::vector<Vertex> Vertices  {
			{ -1.0f,	 1.0f,	-1.0f,	 1.0f },
			{ 1.0f,	 1.0f,	-1.0f,	 1.0f },
			{ 1.0f,	-1.0f,	-1.0f,	 1.0f },
			{ -1.0f,	-1.0f,	-1.0f,	 1.0f },
			{ -1.0f,	 1.0f,	 1.0f,	 1.0f },
			{ 1.0f,	 1.0f,	 1.0f,	 1.0f },
			{ 1.0f,	-1.0f,	 1.0f,	 1.0f },
			{ -1.0f,	-1.0f,	 1.0f,	 1.0f },
		};

		inline static 	std::vector<UINT> Indices = {
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

		void Dx::Cube::RegisterResources() {
			m_psConstants = {
					DirectX::XMFLOAT4(1.f, 0.5f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 1.f, 0.5f, 1.f),
			};

			ApplyMatrix();

			m_vertexBuffer =		VertexBuffer<Vertex>::Create				(2, false, m_graphics, Vertices);
			m_indexBuffer =		IndexBuffer::Create							(3, false, m_graphics, Indices);
			m_psConstantBuffer =	PSConstantBuffer<PSConstants>::Create	(4, false, m_graphics, m_psConstants);
			m_vsConstantBuffer =	VSConstantBuffer<VSConstants>::Create	(4, false, m_graphics, m_vsConstants);
			m_inputLayout =		InputLayout::Create							(5, false, m_graphics, Ieds, m_vertexShader);
		}

		void AttachResources(bool force) {
			m_vertexBuffer->Attach(force);
			m_indexBuffer->Attach(force);
			m_inputLayout->Attach(force);
			m_psConstantBuffer->Attach(force);
			m_vsConstantBuffer->Attach(force);
		}

		void ApplyMatrix()
		{
			m_vsConstants.matrix =
				DirectX::XMMatrixRotationX(m_rotationX)
				* DirectX::XMMatrixRotationZ(m_rotationY)
				* DirectX::XMMatrixRotationY(m_rotationZ)
				* DirectX::XMMatrixTranslation(m_x, m_y, m_z)
				* DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_graphics->Width() / m_graphics->Height(), 1.f, 100.0f);
		}

		void Update(float delta) {
			m_rotationX = fmod(m_rotationX + delta * m_rotationSpeedX * DirectX::XM_2PI, DirectX::XM_2PI);
			m_rotationY = fmod(m_rotationY + delta * m_rotationSpeedY * DirectX::XM_2PI, DirectX::XM_2PI);
			m_rotationX = fmod(m_rotationZ + delta * m_rotationSpeedZ * DirectX::XM_2PI, DirectX::XM_2PI);

			m_x += m_speedX * delta;
			m_y += m_speedY * delta;
			m_z += m_speedZ * delta;

			ApplyMatrix();
		};	

		void Draw() {
			AttachResources(false);
			m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//m_psConstantBuffer->Update(m_psConstants);
			m_vsConstantBuffer->Update(m_vsConstants);
			m_context->DrawIndexed(36, 0, 0);
		};

		void DrawAt(float x, float y, float z) {
			m_x = x;
			m_y = y;
			m_z = z;

			Draw();
		}
	
	private:
		std::shared_ptr<InputLayout>									m_inputLayout;
		std::shared_ptr<VertexBuffer<Vertex>>						m_vertexBuffer;
		std::shared_ptr<IndexBuffer>									m_indexBuffer;
		std::shared_ptr<PSConstantBuffer<PSConstants>>			m_psConstantBuffer;
		std::shared_ptr<VSConstantBuffer<VSConstants>>			m_vsConstantBuffer;

		PSConstants															m_psConstants = {};
		VSConstants															m_vsConstants = {};
	};
}
