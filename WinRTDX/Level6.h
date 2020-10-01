#pragma once
#include "pch.h"

#include <random>

#include "Drawable.h"
#include "CubeTextured.h"
#include "ILevel.h"
#include "IO.h"
#include "Camera.h"

using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level6 : public ILevel
	{
	public:
		using ILevel::ILevel;
		concurrency::task<void> Load()
		{
			return concurrency::create_task([this]
				{
					m_vertexShaderTextured = VertexShader::Load(2, false, L"VertexShader5_6.cso");
					m_pixelShaderTextured = PixelShader::Load(2, false, L"PixelShader5_6.cso");

					m_texture = Texture::Load(1, false, L"Assets\\karin3.dds", 0);
				}
			);
		}

		void SetupModel()
		{
			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 generator(rd());
			std::uniform_real_distribution<float> location(-240.0f, 240.0f);
			std::uniform_real_distribution<float> movementSpeed(.0f, .0f);
			std::uniform_real_distribution<float> startAngle(-DirectX::XM_PI, DirectX::XM_PI);
			std::uniform_real_distribution<float> rotationSpeed(-DirectX::XM_PI / 10.f, DirectX::XM_PI / 10.f);
			std::uniform_real_distribution<float> scale(3.f, 3.0f);

			for (int i = 0; i <= 5000; i++)
			{
				auto cube = std::make_unique<CubeTextured>(m_vertexShaderTextured, m_pixelShaderTextured, 1u);

				cube->X(location(generator));
				cube->Y(location(generator));
				cube->Z(location(generator));

				cube->SpeedX(movementSpeed(generator));
				cube->SpeedY(movementSpeed(generator));
				cube->SpeedZ(movementSpeed(generator));

				cube->RotationX(startAngle(generator));
				cube->RotationY(startAngle(generator));
				cube->RotationZ(startAngle(generator));

				cube->RotationSpeedX(rotationSpeed(generator));
				cube->RotationSpeedY(rotationSpeed(generator));
				cube->RotationSpeedZ(rotationSpeed(generator));

				cube->ScaleX(scale(generator));
				cube->ScaleY(scale(generator));
				cube->ScaleZ(scale(generator));

				cube->Texture(m_texture);
				cube->Init();

				m_drawables.push_back(std::move(cube));
			}

			for (auto d : m_drawables) {
				d->RegisterResources();
			}

			m_worldViewTransformConstantBuffer = VSConstantBuffer<DirectX::XMMATRIX>::Create(1, false, m_worldViewTransform, 0, false);
			m_worldViewTransformConstantBuffer->Attach(false);
			m_worldRotationSpeedY = 0.f;
		}

		void ProcessInput()
		{
			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Up) || m_keyboardInput->IsSet(Windows::System::VirtualKey::W))
				m_camera.MoveForward(m_cameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Down) || m_keyboardInput->IsSet(Windows::System::VirtualKey::S))
				m_camera.MoveForward(-m_cameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Left) || m_keyboardInput->IsSet(Windows::System::VirtualKey::A))
				m_camera.Strafe(-m_cameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Right) || m_keyboardInput->IsSet(Windows::System::VirtualKey::D))
				m_camera.Strafe(m_cameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::X, true))
				m_mouseInput->RelativeTrackingEnter();

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Escape, true))
				m_mouseInput->RelativeTrackingExit();

			//m_camera.Rotate(0, 0.01, 0);
			m_camera.Rotate(
				-m_mouseInput->RelativeDeltaY() * m_mouseSensitivity,
				m_mouseInput->RelativeDeltaX() * m_mouseSensitivity
			);
		}

		void Update(float delta)
		{
			ProcessInput();

			m_worldRotationX = fmod(m_worldRotationX + delta * m_worldRotationSpeedX * DirectX::XM_2PI, DirectX::XM_2PI);
			m_worldRotationY = fmod(m_worldRotationY + delta * m_worldRotationSpeedY * DirectX::XM_2PI, DirectX::XM_2PI);
			m_worldRotationZ = fmod(m_worldRotationZ + delta * m_worldRotationSpeedZ * DirectX::XM_2PI, DirectX::XM_2PI);

			m_worldViewTransform = DirectX::XMMatrixIdentity();
			if (m_worldRotationZ != 0)
				m_worldViewTransform *= DirectX::XMMatrixRotationZ(m_worldRotationZ);

			if (m_worldRotationX != 0)
				m_worldViewTransform *= DirectX::XMMatrixRotationX(m_worldRotationX);

			if (m_worldRotationY != 0)
				m_worldViewTransform *= DirectX::XMMatrixRotationY(m_worldRotationY);

			m_worldViewTransform *= m_camera.GetMatrix();
			m_worldViewTransform *= DirectX::XMMatrixPerspectiveFovLH(1.2f, Dx::Graphics::Instance->Width() / Dx::Graphics::Instance->Height(), .1f, 1000.0f);
			m_worldViewTransformConstantBuffer->Update(m_worldViewTransform);

			for (auto d : m_drawables)
				d->Update(delta, XMMatrixIdentity());
		}

		void Render()
		{
			float color[4]{ 0.3f, .0f, .1f, .2f };
			Graphics::Instance->StartFrame(color);

			for (auto d : m_drawables)
				d->Draw();
		}

	private:
		std::vector<std::shared_ptr<Drawable>>								m_drawables;
		std::shared_ptr<VertexShader>											m_vertexShaderSimple;
		std::shared_ptr<PixelShader>											m_pixelShaderSimple;
		std::shared_ptr<VertexShader>											m_vertexShaderTextured;
		std::shared_ptr<PixelShader>											m_pixelShaderTextured;
		std::shared_ptr<Texture>												m_texture;

		float																			m_worldRotationX{ 0 };
		float																			m_worldRotationY{ 0 };
		float																			m_worldRotationZ{ 0 };

		float																			m_worldRotationSpeedX{ 0 };
		float																			m_worldRotationSpeedY{ 0 };
		float																			m_worldRotationSpeedZ{ 0 };

		DirectX::XMMATRIX															m_worldViewTransform{};
		std::shared_ptr<VSConstantBuffer<DirectX::XMMATRIX>>			m_worldViewTransformConstantBuffer;
		Camera																		m_camera;

		float																			m_cameraMovementSpeed = 1;
		float																			m_cameraRotationSpeed = .03f;
		float																			m_mouseSensitivity = .0006f;
	};
}



