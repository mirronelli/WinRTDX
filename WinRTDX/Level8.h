#pragma once
#include "pch.h"

#include <random>

#include "ILevel.h"
#include "Drawable.h"
#include "CubeColored.h"
#include "SphereColored.h"
#include "CubeTextured.h"
#include "IO.h"
#include "Camera.h"

using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level8 : public ILevel
	{
	public:
		using ILevel::ILevel;
		concurrency::task<void> Load()
		{
			return concurrency::create_task(
				[this]()
				{
					m_vertexShaderWithNormal = VertexShader::Load(1, false, L"8_VertexWithNormal.cso");
					m_vertexShaderWithColor = VertexShader::Load(2, false, L"8_VertexWithColor.cso");
					m_pixelShaderWithNormal = PixelShader::Load(3, false, L"8_PixelWithNormal.cso");
					m_pixelShaderWithColor = PixelShader::Load(4, false, L"8_PixelWithColor.cso");
				}
			);
		}

		void SetupModel()
		{
			GenerateDrawables();

			m_pixelPerLevelConstants.lightPosition				= { 0, 0, 0, 0 };
			m_pixelPerLevelConstants.lightColor					= { 1.0f, 1.0f, 1.0, 0.0f };
			m_pixelPerLevelConstants.ambientLight				= { 0.1f, 0.01f, 0.01f, 0.2f };
			m_pixelPerLevelConstants.diffuseIntensity			= 1.0f;
			m_pixelPerLevelConstants.attenuationQuadratic	= 0.00001f;
			m_pixelPerLevelConstants.attenuationLinear		= 0.01f;
			m_pixelPerLevelConstants.attenuationConstant		= 0.f;

			m_pixelPerLevelConstantsBuffer	= PSConstantBuffer<PixelPerLevelConstants>::	Create(1u, false, m_pixelPerLevelConstants, 0, false);
			m_pixelPerFrameConstantsBuffer	= PSConstantBuffer<PixelPerFrameConstants>::	Create(2u, false, m_pixelPerFrameConstants, 1u, true);
			m_vertexPerFrameConstantsBuffer	= VSConstantBuffer<VertexPerFrameConstants>::Create(3u, false, m_vertexPerFrameConstants, 1u, true);

			m_pixelPerLevelConstantsBuffer->Attach(false);
			m_pixelPerFrameConstantsBuffer->Attach(false);
			m_vertexPerFrameConstantsBuffer->Attach(false);
			m_mouseInput->RelativeTrackingEnter();
		}

		void GenerateDrawables()
		{	
			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 generator(rd());
			std::uniform_real_distribution<float> location(-240.0f, 240.0f);
			std::uniform_real_distribution<float> movementSpeed(.0f, .0f);
			std::uniform_real_distribution<float> startAngle(-DirectX::XM_PI, DirectX::XM_PI);
			std::uniform_real_distribution<float> rotationSpeed(-DirectX::XM_PI / 50.f, DirectX::XM_PI / 50.f);
			std::uniform_real_distribution<float> scale(1.f, 6.0f);
			std::uniform_real_distribution<float> color(0.f, 1.0f);
			std::uniform_real_distribution<float> refelctiveness(0.f, 1.0f);
			std::uniform_real_distribution<float> specularPower(0.f, 64.0f);

			//for (int i = 0; i < 100; i++)
			//{
			//	std::unique_ptr<Meshh> mesh = std::make_unique<Meshh>(
			//		m_vertexShaderWithNormal,
			//		m_pixelShaderWithNormal,
			//		5u
			//	);

			//	mesh->X(location(generator));
			//	mesh->Y(location(generator));
			//	mesh->Z(location(generator));

			//	mesh->SpeedX(movementSpeed(generator));
			//	mesh->SpeedY(movementSpeed(generator));
			//	mesh->SpeedZ(movementSpeed(generator));

			//	mesh->RotationX(startAngle(generator));
			//	mesh->RotationY(startAngle(generator));
			//	mesh->RotationZ(startAngle(generator));

			//	mesh->RotationSpeedX(rotationSpeed(generator));
			//	mesh->RotationSpeedY(rotationSpeed(generator));
			//	mesh->RotationSpeedZ(rotationSpeed(generator));

			//	mesh->Color({ color(generator), color(generator), color(generator), 1 });
			//	mesh->Specular({ refelctiveness(generator), specularPower(generator) });
			//	mesh->FileName("Assets\\suzanne.obj");
			//	mesh->Scale(20);
			//	mesh->Init();

			//	m_drawables.push_back(std::move(mesh));

			//}

			auto theSun = std::make_unique<SphereColoredWithNormal>(
				m_vertexShaderWithColor,
				m_pixelShaderWithColor,
				200u,
				40
				);

			theSun->Scale(10);
			theSun->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun->Init();

			m_drawables.push_back(std::move(theSun));

			for (auto d : m_drawables) {
				d->RegisterResources();
			}
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

			m_vertexPerFrameConstants.worldViewTransform =
				m_camera.GetMatrix()
				* DirectX::XMMatrixPerspectiveFovLH(1.2f, Graphics::Instance->Width() / Graphics::Instance->Height(), .1f, 1000.0f);

			m_pixelPerFrameConstants.cameraPosition = m_camera.Position();

			m_vertexPerFrameConstantsBuffer->Update(m_vertexPerFrameConstants);
			m_pixelPerFrameConstantsBuffer->Update(m_pixelPerFrameConstants);

			for (auto d : m_drawables)
				d->Update(delta, XMMatrixIdentity());
		}

		void Render()
		{
			float color[4]{ 0.0f, .0f, .02f, .0f };
			Graphics::Instance->StartFrame(color);

			for (auto d : m_drawables)
				d->Draw();
		}

	private:
		struct VertexPerFrameConstants
		{
			DirectX::XMMATRIX worldViewTransform;
		};

		struct PixelPerFrameConstants
		{
			DirectX::XMVECTOR	cameraPosition;
		};

		struct PixelPerLevelConstants {
			XMFLOAT4	lightPosition;
			XMFLOAT4	lightColor;
			XMFLOAT4	ambientLight;

			float		diffuseIntensity;
			float		attenuationQuadratic;
			float		attenuationLinear;
			float		attenuationConstant;
		};

		std::vector<std::shared_ptr<Dx::Drawables::Drawable>>			m_drawables;

		std::shared_ptr<VertexShader>											m_vertexShaderWithColor;
		std::shared_ptr<VertexShader>											m_vertexShaderWithNormal;
		std::shared_ptr<PixelShader>											m_pixelShaderWithColor;
		std::shared_ptr<PixelShader>											m_pixelShaderWithNormal;

		PixelPerFrameConstants													m_pixelPerFrameConstants;
		std::shared_ptr<PSConstantBuffer<PixelPerFrameConstants>>	m_pixelPerFrameConstantsBuffer;

		VertexPerFrameConstants													m_vertexPerFrameConstants;
		std::shared_ptr<VSConstantBuffer<VertexPerFrameConstants>>	m_vertexPerFrameConstantsBuffer;

		PixelPerLevelConstants													m_pixelPerLevelConstants;
		std::shared_ptr<PSConstantBuffer<PixelPerLevelConstants>>	m_pixelPerLevelConstantsBuffer;

		Camera																		m_camera = Camera(DirectX::XMVectorSet(0, 0, -120, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0));
		float																			m_cameraMovementSpeed = 1;
		float																			m_mouseSensitivity = .0006f;
	};
}



