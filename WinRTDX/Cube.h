#pragma once
#include "Drawable.h"

using namespace Dx::Attachables;
namespace Dx {

	class Cube : public Drawable
	{
		struct Vertex {
			float	x, y, z, w;
		};

		typedef struct {
			DirectX::XMMATRIX matrix;
			DirectX::XMFLOAT4 colors[6]; 
		} Constants;

	public:
		Cube(std::shared_ptr<Dx::Graphics> graphics, IBuffer vertexShaderByteCode, IBuffer pixelShaderByteCode,
			float x = 0, float y = 0, float z = 0,
			float speedX = 0, float speedY = 1, float speedZ = 0,
			float rotationX = 0, float rotationY = 0, float rotationZ = 0,
			float rotationSpeedX = 1, float rotationSpeedY = 1, float rotationSpeedZ = 1
		) :
			Drawable(
				graphics, vertexShaderByteCode, pixelShaderByteCode,
				x, y, z,
				speedX, speedY, speedZ,
				rotationX, rotationY, rotationZ,
				rotationSpeedX, rotationSpeedY, rotationSpeedZ
			)
		{};
		Cube(Cube&) = delete;
		Cube() = delete;
		~Cube() {};

		void RegisterResources();
		void AttachResources();
		void Update(float delta);
		void Draw();
		void DrawAt(float x, float y, float z);
	private:
		std::shared_ptr<InputLayout>									m_inputLayout;
		std::shared_ptr<VertexBuffer<Vertex>>						m_vertexBuffer;
		std::shared_ptr<IndexBuffer>									m_indexBuffer;
		std::shared_ptr<ConstantBuffer<Constants>>				m_constantBuffer;

		Constants															m_constants = {};

	};
}
