#pragma once
#include "pch.h"
#include "winrt/Windows.Foundation.Numerics.h"

#include <random>

#include "Drawable.h"
#include "CubeColored.h"
#include "SphereColored.h"
#include "CubeTextured.h"
#include "ILevel.h"
#include "IO.h"
#include "Camera.h"
#include <DirectXMath.h>

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
					m_vertexShaderTextured = VertexShader::Load(2, false, m_graphics, L"VertexShader7Textured.cso");
					m_vertexShaderColored = VertexShader::Load(3, false, m_graphics, L"VertexShader7Colored.cso");

					m_pixelShaderTextured	= PixelShader::Load(2, false, m_graphics, L"PixelShader7Textured.cso");
					m_pixelShaderColored = PixelShader::Load(3, false, m_graphics, L"PixelShader7Colored.cso");
					m_pixelShaderStatic = PixelShader::Load(4, false, m_graphics, L"PixelShader7Static.cso");

					m_texture = Texture::Load(1, false, m_graphics, L"Assets\\karin3.dds", 0);
				}
			);
		}

		void SetupModel()
		{
			GenerateDrawables();

			m_sharedConstantVSBuffer = VSConstantBuffer<SharedConstants>::Create(4u, false, m_graphics, m_sharedConstants, 1, false);
			m_sharedConstantVSBuffer->Attach(false);

			m_sharedConstantPSBuffer = PSConstantBuffer<SharedConstants>::Create(4u, false, m_graphics, m_sharedConstants, 1, false);
			m_sharedConstantPSBuffer->Attach(false);

			m_light.lightPosition				= { 0, 0, 0, 0 };
			m_light.lightColor					= { 1.0f, 0.5f, 0.9, 0.0f };
			m_light.ambientLight					= { 0.05f, 0.0f, 0.05f, 0.0f };
			m_light.diffuseIntensity			= 1.0f;
			m_light.attenuationQuadratic		= 0.0001f;
			m_light.attenuationLinear			= 0.0f;
			m_light.attenuationConstant		= 0.f;

			m_lightConstantBuffer = PSConstantBuffer<PSConstants>::Create(5u, false, m_graphics, m_light, 0, false);
			m_lightConstantBuffer->Attach(false);

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

			for (int i = 0; i <= 100; i++)
			{
				float radius = scale(generator);
				auto cube = std::make_unique<CubeTextured>(m_graphics, m_vertexShaderTextured, m_pixelShaderTextured, 1u);

				cube->WorldX(location(generator));
				cube->WorldY(location(generator));
				cube->WorldZ(location(generator));

				cube->SpeedX(movementSpeed(generator));
				cube->SpeedY(movementSpeed(generator));
				cube->SpeedZ(movementSpeed(generator));

				cube->RotationX(startAngle(generator));
				cube->RotationY(startAngle(generator));
				cube->RotationZ(startAngle(generator));

				cube->RotationSpeedX(rotationSpeed(generator));
				cube->RotationSpeedY(rotationSpeed(generator));
				cube->RotationSpeedZ(rotationSpeed(generator));

				cube->ScaleX(radius);
				cube->ScaleY(radius);
				cube->ScaleZ(radius);

				cube->Texture(m_texture);

				m_drawables.push_back(std::move(cube));
			}

			for (int i = 0; i <= 100; i++)
			{
				float radius = scale(generator);
				auto cube = std::make_unique<CubeColored>(m_graphics, m_vertexShaderColored, m_pixelShaderColored, 2u);

				cube->WorldX(location(generator));
				cube->WorldY(location(generator));
				cube->WorldZ(location(generator));

				cube->SpeedX(movementSpeed(generator));
				cube->SpeedY(movementSpeed(generator));
				cube->SpeedZ(movementSpeed(generator));

				cube->RotationX(startAngle(generator));
				cube->RotationY(startAngle(generator));
				cube->RotationZ(startAngle(generator));

				cube->RotationSpeedX(rotationSpeed(generator));
				cube->RotationSpeedY(rotationSpeed(generator));
				cube->RotationSpeedZ(rotationSpeed(generator));

				cube->ScaleX(radius);
				cube->ScaleY(radius);
				cube->ScaleZ(radius);

				m_drawables.push_back(std::move(cube));
			}

			for (int i = 0; i <= 100; i++)
			{
				float radius = scale(generator);
				auto sphere = std::make_unique<SphereColored>(
					m_graphics,
					m_vertexShaderColored,
					m_pixelShaderColored,
					i + 1000u,
					24
					);

				sphere->WorldX(location(generator));
				sphere->WorldY(location(generator));
				sphere->WorldZ(location(generator));

				sphere->SpeedX(movementSpeed(generator));
				sphere->SpeedY(movementSpeed(generator));
				sphere->SpeedZ(movementSpeed(generator));

				sphere->RotationX(startAngle(generator));
				sphere->RotationY(startAngle(generator));
				sphere->RotationZ(startAngle(generator));

				sphere->RotationSpeedX(rotationSpeed(generator));
				sphere->RotationSpeedY(rotationSpeed(generator));
				sphere->RotationSpeedZ(rotationSpeed(generator));

				sphere->ScaleX(radius);
				sphere->ScaleY(radius);
				sphere->ScaleZ(radius);

				sphere->Color(XMFLOAT3(color(generator), color(generator), color(generator)));

				sphere->Prepare();

				m_drawables.push_back(std::move(sphere));
			}

			auto theSun = std::make_unique<SphereColored>(
				m_graphics,
				m_vertexShaderColored,
				m_pixelShaderStatic,
				3u,
				40
				);

			theSun->ScaleX(10);
			theSun->ScaleY(10);
			theSun->ScaleZ(10);
			theSun->WorldZ(0);
			theSun->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun->Prepare();

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

			m_sharedConstants.worldViewTransform = 
				m_camera.GetMatrix()
				* DirectX::XMMatrixPerspectiveFovLH(1.2f, m_graphics->Width() / m_graphics->Height(), .1f, 1000.0f);
			
			m_sharedConstants.cameraPosition = m_camera.Position();

			m_sharedConstantVSBuffer->Update(m_sharedConstants);
			m_sharedConstantPSBuffer->Update(m_sharedConstants);

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
		struct SharedConstants
		{
			DirectX::XMMATRIX worldViewTransform;
			DirectX::XMVECTOR	cameraPosition;
		};

		struct PSConstants {
			float4	lightPosition;
			float4	lightColor;
			float4	ambientLight;
			
			float		diffuseIntensity;
			float		attenuationQuadratic;
			float		attenuationLinear;
			float		attenuationConstant;
		} ;

		std::vector<std::shared_ptr<Drawable>>								m_drawables;

		std::shared_ptr<VertexShader>											m_vertexShaderTextured;
		std::shared_ptr<VertexShader>											m_vertexShaderColored;
		std::shared_ptr<PixelShader>											m_pixelShaderTextured;
		std::shared_ptr<PixelShader>											m_pixelShaderColored;
		std::shared_ptr<PixelShader>											m_pixelShaderStatic;

		std::shared_ptr<Texture>												m_texture;

		SharedConstants															m_sharedConstants;
		std::shared_ptr<PSConstantBuffer<SharedConstants>>				m_sharedConstantPSBuffer;
		std::shared_ptr<VSConstantBuffer<SharedConstants>>				m_sharedConstantVSBuffer;

		PSConstants																	m_light;
		std::shared_ptr<PSConstantBuffer<PSConstants>>					m_lightConstantBuffer;

		Camera																		m_camera = Camera(DirectX::XMVectorSet(0,0,-120,0), XMVectorSet(0,0,1,0), XMVectorSet(0,1,0,0));

		float																			m_cameraMovementSpeed = 1;
		float																			m_mouseSensitivity = .0006f;
	};
}



