#pragma once
#include <DirectXMath.h>

using namespace DirectX;
namespace Dx
{

	class Camera
	{
	public:
		Camera()
		{
			m_lookingAt = XMVectorSet(0, 0, 1, 0);
			m_position = XMVectorSet(0, 0, 0, 0);
		}

		Camera(XMVECTOR position, XMVECTOR lookingAt) : m_lookingAt(m_lookingAt), m_position(position) {};


		void Rotate(float roll, float pitch, float yaw) 
		{
			m_lookingAt = XMVector3Transform( m_lookingAt, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		}

		void MoveForward(float length)
		{
			m_position = m_position + (m_lookingAt * length);
		}

		DirectX::FXMMATRIX GetMatrix() {
			return DirectX::XMMatrixLookAtLH(
				m_position,
				m_position + m_lookingAt,
				DirectX::XMVectorSet(0., 1., 0., 0.)
			);
		}
	private:
		XMVECTOR		m_lookingAt;
		XMVECTOR		m_position;
	};

}
