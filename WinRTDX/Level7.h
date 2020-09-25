#pragma once
#include "pch.h"
#include "winrt/Windows.Foundation.Numerics.h"

#include <random>

#include "Drawable.h"
#include "CubeColored.h"
#include "CubeTextured.h"
#include "ILevel.h"
#include "IO.h"
#include "Camera.h"

using namespace winrt::Windows::Foundation::Numerics;
using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level7 : public ILevel
	{
	public:
		using ILevel::ILevel;
		concurrency::task<void> Load()
		{
			return concurrency::create_task([this]
				{
					Dx::Attachables::ResourceManager::ClearCache();

					m_vertexShaderTextured = VertexShader::Load(2, false, m_graphics, L"VertexShader7.cso");
					m_vertexShaderTextured_b = VertexShader::Load(3, false, m_graphics, L"VertexShader7b.cso");

					m_pixelShaderTextured = PixelShader::Load(2, false, m_graphics, L"PixelShader7.cso");
					m_pixelShaderTextured_b = PixelShader::Load(3, false, m_graphics, L"PixelShader7b.cso");

					m_texture = Texture::Load(1, false, m_graphics, L"Assets\\karin3.dds", 0);
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

			for (int i = 0; i <= 1000; i++)
			{
				m_drawables.push_back(std::make_unique<CubeTextured>(
					m_graphics, m_vertexShaderTextured, m_pixelShaderTextured, m_texture,

					location(generator), location(generator), location(generator),
					movementSpeed(generator), movementSpeed(generator), movementSpeed(generator),
					startAngle(generator), startAngle(generator), startAngle(generator),
					rotationSpeed(generator), rotationSpeed(generator), rotationSpeed(generator),
					scale(generator), scale(generator), scale(generator)
					)
				);
			}

			for (int i = 0; i <= 500; i++)
			{
				m_drawables.push_back(std::make_unique<CubeColored>(
					m_graphics, m_vertexShaderTextured_b, m_pixelShaderTextured_b, m_texture,

					location(generator), location(generator), location(generator),
					movementSpeed(generator), movementSpeed(generator), movementSpeed(generator),
					startAngle(generator), startAngle(generator), startAngle(generator),
					rotationSpeed(generator), rotationSpeed(generator), rotationSpeed(generator),
					scale(generator), scale(generator), scale(generator)
					)
				);
			}

			for (auto d : m_drawables) {
				d->RegisterResources();
			}

			m_worldViewTransformConstantBuffer = VSConstantBuffer<DirectX::XMMATRIX>::Create(1, false, m_graphics, m_worldViewTransform, 0, false);
			m_worldViewTransformConstantBuffer->Attach(false);
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

			m_camera.Rotate(
				-m_mouseInput->RelativeDeltaY() * m_mouseSensitivity, 
				m_mouseInput->RelativeDeltaX() * m_mouseSensitivity
			);
		}

		void Update(float delta)
		{
			ProcessInput();

			m_worldViewTransform = 
				m_camera.GetMatrix()
				* DirectX::XMMatrixPerspectiveFovLH(1.2f, m_graphics->Width() / m_graphics->Height(), .1f, 1000.0f);
			m_worldViewTransformConstantBuffer->Update(m_worldViewTransform);

			for (auto d : m_drawables)
				d->Update(delta);
		}

		void Render()
		{
			float color[4]{ 0.0f, .0f, .02f, .0f };
			m_graphics->StartFrame(color);

			for (auto d : m_drawables)
				d->Draw();
		}

	private:
		std::vector<std::shared_ptr<Drawable>>								m_drawables;
		std::shared_ptr<VertexShader>											m_vertexShaderSimple;
		std::shared_ptr<PixelShader>											m_pixelShaderSimple;
		std::shared_ptr<VertexShader>											m_vertexShaderTextured;
		std::shared_ptr<VertexShader>											m_vertexShaderTextured_b;
		std::shared_ptr<PixelShader>											m_pixelShaderTextured;
		std::shared_ptr<PixelShader>											m_pixelShaderTextured_b;
		std::shared_ptr<Texture>												m_texture;

		DirectX::XMMATRIX															m_worldViewTransform{};
		std::shared_ptr<VSConstantBuffer<DirectX::XMMATRIX>>			m_worldViewTransformConstantBuffer;
		Camera																		m_camera;

		float																			m_cameraMovementSpeed = 1;
		float																			m_mouseSensitivity = .0006f;
	};
}



