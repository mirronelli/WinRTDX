#pragma once
#include "pch.h"
#include "winrt/Windows.Foundation.Numerics.h"

#include <random>
#include <DirectXMath.h>

#include "Drawable.h"
#include "CubeColored.h"
#include "SphereColored.h"
#include "CubeTextured.h"
#include "Mesh.h"
#include "ILevel.h"
#include "IO.h"
#include "Camera.h"
#include "Scene.h"

using namespace winrt::Windows::Foundation::Numerics;
using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level9 : public ILevel
	{
	public:
		using ILevel::ILevel;
		concurrency::task<void> Load()
		{
			return concurrency::create_task(
				[this]()
				{
					mVertexShaderWithNormal = VertexShader::Load(1, false, m_graphics, L"8_VertexWithNormal.cso");
					mVertexShaderWithColor = VertexShader::Load(2, false, m_graphics, L"8_VertexWithColor.cso");
					mPixelShaderWithNormal = PixelShader::Load(3, false, m_graphics, L"8_PixelWithNormal.cso");
					mPixelShaderWithColor = PixelShader::Load(4, false, m_graphics, L"8_PixelWithColor.cso");
				}
			);
		}

		void SetupModel()
		{
			GenerateScene();

			mPixelPerLevelConstants.lightPosition = { 0, 0, 0, 0 };
			mPixelPerLevelConstants.lightColor = { 1.0f, 1.0f, 1.0, 0.0f };
			mPixelPerLevelConstants.ambientLight = { 0.1f, 0.01f, 0.01f, 0.2f };
			mPixelPerLevelConstants.diffuseIntensity = 1.0f;
			mPixelPerLevelConstants.attenuationQuadratic = 0.00001f;
			mPixelPerLevelConstants.attenuationLinear = 0.01f;
			mPixelPerLevelConstants.attenuationConstant = 0.f;

			mPixelPerLevelConstantsBuffer = PSConstantBuffer<PixelPerLevelConstants>::Create(1u, false, m_graphics, mPixelPerLevelConstants, 0, false);
			mPixelPerFrameConstantsBuffer = PSConstantBuffer<PixelPerFrameConstants>::Create(2u, false, m_graphics, mPixelPerFrameConstants, 1u, true);
			mVertexPerFrameConstantsBuffer = VSConstantBuffer<VertexPerFrameConstants>::Create(3u, false, m_graphics, mVertexPerFrameConstants, 1u, true);

			mPixelPerLevelConstantsBuffer->Attach(false);
			mPixelPerFrameConstantsBuffer->Attach(false);
			mVertexPerFrameConstantsBuffer->Attach(false);
			m_mouseInput->RelativeTrackingEnter();
		}

		void GenerateScene()
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

			for (int i = 0; i < 100; i++)
			{
				std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(
					m_graphics,
					mVertexShaderWithNormal,
					mPixelShaderWithNormal,
					5u
					);

				mesh->X(location(generator));
				mesh->Y(location(generator));
				mesh->Z(location(generator));

				mesh->SpeedX(movementSpeed(generator));
				mesh->SpeedY(movementSpeed(generator));
				mesh->SpeedZ(movementSpeed(generator));

				mesh->RotationX(startAngle(generator));
				mesh->RotationY(startAngle(generator));
				mesh->RotationZ(startAngle(generator));

				mesh->RotationSpeedX(rotationSpeed(generator));
				mesh->RotationSpeedY(rotationSpeed(generator));
				mesh->RotationSpeedZ(rotationSpeed(generator));

				mesh->Color({ color(generator), color(generator), color(generator), 1 });
				mesh->Specular({ refelctiveness(generator), specularPower(generator) });
				mesh->FileName("Assets\\suzanne.obj");
				mesh->Scale(20);
				mesh->Init();

				mRootScene.AddDrawable(std::move(mesh));

			}

			auto theSun = std::make_unique<SphereColored>(
				m_graphics,
				mVertexShaderWithColor,
				mPixelShaderWithColor,
				200u,
				40
				);

			theSun->Scale(10);
			theSun->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun->Init();

			mRootScene.AddDrawable(std::move(theSun));
		}

		void ProcessInput()
		{
			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Up) || m_keyboardInput->IsSet(Windows::System::VirtualKey::W))
				mCamera.MoveForward(mCameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Down) || m_keyboardInput->IsSet(Windows::System::VirtualKey::S))
				mCamera.MoveForward(-mCameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Left) || m_keyboardInput->IsSet(Windows::System::VirtualKey::A))
				mCamera.Strafe(-mCameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Right) || m_keyboardInput->IsSet(Windows::System::VirtualKey::D))
				mCamera.Strafe(mCameraMovementSpeed);

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::X, true))
				m_mouseInput->RelativeTrackingEnter();

			if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Escape, true))
				m_mouseInput->RelativeTrackingExit();

			mCamera.Rotate(
				-m_mouseInput->RelativeDeltaY() * mMouseSensitivity,
				m_mouseInput->RelativeDeltaX() * mMouseSensitivity
			);
		}

		void Update(float delta)
		{
			ProcessInput();

			mVertexPerFrameConstants.worldViewTransform =
				mCamera.GetMatrix()
				* DirectX::XMMatrixPerspectiveFovLH(1.2f, m_graphics->Width() / m_graphics->Height(), .1f, 1000.0f);

			mPixelPerFrameConstants.cameraPosition = mCamera.Position();

			mVertexPerFrameConstantsBuffer->Update(mVertexPerFrameConstants);
			mPixelPerFrameConstantsBuffer->Update(mPixelPerFrameConstants);

			for (auto d : m_drawables)
				d->Update(delta, XMMatrixIdentity());
		}

		void Render()
		{
			float color[4]{ 0.0f, .0f, .02f, .0f };
			m_graphics->StartFrame(color);

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
			float4	lightPosition;
			float4	lightColor;
			float4	ambientLight;

			float		diffuseIntensity;
			float		attenuationQuadratic;
			float		attenuationLinear;
			float		attenuationConstant;
		};

		Scene																			mRootScene;

		std::shared_ptr<VertexShader>											mVertexShaderWithColor;
		std::shared_ptr<VertexShader>											mVertexShaderWithNormal;
		std::shared_ptr<PixelShader>											mPixelShaderWithColor;
		std::shared_ptr<PixelShader>											mPixelShaderWithNormal;

		PixelPerFrameConstants													mPixelPerFrameConstants;
		std::shared_ptr<PSConstantBuffer<PixelPerFrameConstants>>	mPixelPerFrameConstantsBuffer;

		VertexPerFrameConstants													mVertexPerFrameConstants;
		std::shared_ptr<VSConstantBuffer<VertexPerFrameConstants>>	mVertexPerFrameConstantsBuffer;

		PixelPerLevelConstants													mPixelPerLevelConstants;
		std::shared_ptr<PSConstantBuffer<PixelPerLevelConstants>>	mPixelPerLevelConstantsBuffer;

		Camera																		mCamera = Camera(DirectX::XMVectorSet(0, 0, -120, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0));
		float																			mCameraMovementSpeed = 1;
		float																			mMouseSensitivity = .0006f;
	};
}