#pragma once
#include <pch.h>
#include "Graphics.h"
#include "InputLayout.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

namespace Dx {

	class Drawable
	{
	public:
		Drawable(
			std::shared_ptr<Dx::Graphics> graphics,
			IBuffer vertexShaderBytCode,
			IBuffer pixelShaderByteCode,

			float x = 0, float y = 0, float z = 0,
			float speedX = 0, float speedY = 1, float speedZ = 0,
			float rotationX = 0, float rotationY = 0, float rotationZ = 0,
			float rotationSpeedX = 1, float rotationSpeedY = 1, float rotationSpeedZ = 1
		) :
			m_graphics(graphics), 
			m_device(graphics->Device()), 
			m_context(graphics->Context()),
			m_vertexShaderByteCode(vertexShaderBytCode),
			m_pixelShaderByteCode(pixelShaderByteCode),

			m_x(x), m_y(y), m_z(z),
			m_speedX(speedX), m_speedY(speedY), m_speedZ(speedZ),
			m_rotationX(rotationX), m_rotationY(rotationY), m_rotationZ(rotationZ),
			m_rotationSpeedX(rotationSpeedX), m_rotationSpeedY(rotationSpeedY), m_rotationSpeedZ(rotationSpeedZ)
		{};
		Drawable(Drawable& a) = delete;
		Drawable() = delete;
		virtual void RegisterResources() = 0;
		virtual void AttachResources() = 0;
		virtual void Update(float delta) = 0;
		virtual void Draw() = 0;
		virtual void DrawAt(float x, float y, float z) = 0;
		virtual ~Drawable() {};

	protected:
		bool													m_registered = false;
		std::shared_ptr<Dx::Graphics>					m_graphics;
		com_ptr<ID3D11Device3>							m_device;
		com_ptr<ID3D11DeviceContext4>					m_context;
		IBuffer												m_pixelShaderByteCode;
		IBuffer												m_vertexShaderByteCode;

		// state
		float		m_x;
		float		m_y;
		float		m_z;

		float		m_speedX;
		float		m_speedY;
		float		m_speedZ;

		float		m_rotationSpeedX;
		float		m_rotationSpeedY;
		float		m_rotationSpeedZ;

		float		m_rotationX;
		float		m_rotationY;
		float		m_rotationZ;
	};
}