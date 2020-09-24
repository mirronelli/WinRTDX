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
			m_position = XMVectorSet(0, 0, 0, 0);

			m_lookingAt = XMVectorSet(0, 0, 1, 0);
			m_headDirection = XMVectorSet(0, 1, 0, 0);
			m_lookingAtNormal = XMVectorSet(1, 0, 0, 0);
		}

		Camera(XMVECTOR position, XMVECTOR lookingAt, XMVECTOR headDirection) : 
			m_lookingAt(lookingAt), 
			m_position(position),
			m_headDirection(headDirection),
			m_lookingAtNormal(XMVector3Normalize(XMVector3Cross(m_lookingAt, m_headDirection)))
		{};


		void Rotate(float pitch, float yaw) 
		{
			if (yaw != 0)
				m_lookingAt = XMVector3Normalize( XMVector3Transform( m_lookingAt, XMMatrixRotationNormal(m_headDirection, yaw)));
			if (pitch != 0)
			{
				m_lookingAt = XMVector3Normalize( XMVector3Transform( m_lookingAt, XMMatrixRotationNormal(m_lookingAtNormal, pitch)));
				m_headDirection = XMVector3Normalize(XMVector3Transform(m_headDirection, XMMatrixRotationNormal(m_lookingAtNormal, pitch)));
			}

			m_lookingAtNormal = XMVector3Normalize(XMVector3Cross(m_lookingAt, m_headDirection));
		}

		void MoveForward(float length)
		{
			m_position = m_position + (m_lookingAt * length);
		}

		void Strafe(float length)
		{
			m_position = m_position + ( -m_lookingAtNormal * length);
		}

		DirectX::FXMMATRIX GetMatrix() {
			return DirectX::XMMatrixLookToLH(
				m_position,
				m_lookingAt,
				m_headDirection
			);
		}
	private:
		XMVECTOR		m_lookingAt;
		XMVECTOR		m_lookingAtNormal;
		XMVECTOR		m_headDirection;

		XMVECTOR		m_position;
	};

}
