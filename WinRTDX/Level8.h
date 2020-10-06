#pragma once
#include "pch.h"

#include <random>

#include "ILevel.h"
#include "Drawable.h"
#include "CubeColored.h"
#include "SphereColored.h"
#include "CubeTextured.h"
#include "MeshColored.h"
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
					mVertexShaderWithNormal = VertexShader::Preload(VertexType::SimpleWithNormal, L"8_VertexWithNormal.cso");
					mVertexShaderWithColor = VertexShader::Preload(VertexType::ColoredWithNormal, L"8_VertexWithColor.cso");
					mPixelShaderWithNormal = PixelShader::Preload(VertexType::SimpleWithNormal, L"8_PixelWithNormal.cso");
					mPixelShaderWithColor = PixelShader::Preload(VertexType::ColoredWithNormal, L"8_PixelWithColor.cso");
				}
			);
		}

		void SetupModel()
		{
			GenerateDrawables();

			mPixelPerLevelConstants.lightPosition				= { 0, 0, 0, 0 };
			mPixelPerLevelConstants.lightColor					= { 1.0f, 1.0f, 1.0, 0.0f };
			mPixelPerLevelConstants.ambientLight				= { 0.1f, 0.01f, 0.01f, 0.2f };
			mPixelPerLevelConstants.diffuseIntensity			= 1.0f;
			mPixelPerLevelConstants.attenuationQuadratic	= 0.00001f;
			mPixelPerLevelConstants.attenuationLinear		= 0.01f;
			mPixelPerLevelConstants.attenuationConstant		= 0.f;

			mPixelPerLevelConstantsBuffer	= PSConstantBuffer<PixelPerLevelConstants>::	Create(mPixelPerLevelConstants, ResourceSlots::PerLevel);
			mPixelPerFrameConstantsBuffer	= PSConstantBuffer<PixelPerFrameConstants>::	Create(mPixelPerFrameConstants, ResourceSlots::PerFrame);
			mVertexPerFrameConstantsBuffer	= VSConstantBuffer<VertexPerFrameConstants>::Create(mVertexPerFrameConstants, ResourceSlots::PerFrame);

			mPixelPerLevelConstantsBuffer->Attach();
			mPixelPerFrameConstantsBuffer->Attach();
			mVertexPerFrameConstantsBuffer->Attach();
			mMouseInput->RelativeTrackingEnter();
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
			std::uniform_real_distribution<float> reflectiveness(0.f, 1.0f);
			std::uniform_real_distribution<float> specularPower(0.f, 64.0f);

			//for (int i = 0; i < 100; i++)
			//{
			//	std::unique_ptr<MeshColored> mesh = std::make_unique<MeshColored>(
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
			//	mesh->Specular(reflectiveness(generator), specularPower(generator) );
			//	mesh->FileName("Assets\\suzanne.obj");
			//	mesh->Scale(20);
			//	mesh->Init();

			//	m_drawables.push_back(std::move(mesh));

			//}

			auto theSun = std::make_unique<SphereColoredWithNormal>(40);

			theSun->Scale(10);
			theSun->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun->Init();

			mDrawables.push_back(std::move(theSun));

			for (auto d : mDrawables) {
				d->RegisterResources();
			}
		}

		void ProcessInput()
		{
			if (mKeyboardInput->IsSet(Windows::System::VirtualKey::Up) || mKeyboardInput->IsSet(Windows::System::VirtualKey::W))
				mCamera.MoveForward(mCameraMovementSpeed);

			if (mKeyboardInput->IsSet(Windows::System::VirtualKey::Down) || mKeyboardInput->IsSet(Windows::System::VirtualKey::S))
				mCamera.MoveForward(-mCameraMovementSpeed);

			if (mKeyboardInput->IsSet(Windows::System::VirtualKey::Left) || mKeyboardInput->IsSet(Windows::System::VirtualKey::A))
				mCamera.Strafe(-mCameraMovementSpeed);

			if (mKeyboardInput->IsSet(Windows::System::VirtualKey::Right) || mKeyboardInput->IsSet(Windows::System::VirtualKey::D))
				mCamera.Strafe(mCameraMovementSpeed);

			if (mKeyboardInput->IsSet(Windows::System::VirtualKey::X, true))
				mMouseInput->RelativeTrackingEnter();

			if (mKeyboardInput->IsSet(Windows::System::VirtualKey::Escape, true))
				mMouseInput->RelativeTrackingExit();

			mCamera.Rotate(
				-mMouseInput->RelativeDeltaY() * mMouseSensitivity,
				mMouseInput->RelativeDeltaX() * mMouseSensitivity
			);
		}

		void Update(float delta)
		{
			ProcessInput();

			mVertexPerFrameConstants.worldViewTransform =
				mCamera.GetMatrix()
				* DirectX::XMMatrixPerspectiveFovLH(1.2f, Graphics::Instance->Width() / Graphics::Instance->Height(), .1f, 1000.0f);

			mPixelPerFrameConstants.cameraPosition = mCamera.Position();

			mVertexPerFrameConstantsBuffer->Update(mVertexPerFrameConstants);
			mPixelPerFrameConstantsBuffer->Update(mPixelPerFrameConstants);

			for (auto d : mDrawables)
				d->Update(delta, XMMatrixIdentity());
		}

		void Render()
		{
			float color[4]{ 0.0f, .0f, .02f, .0f };
			Graphics::Instance->StartFrame(color);

			for (auto d : mDrawables)
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

		std::vector<std::shared_ptr<Dx::Drawables::Drawable>>			mDrawables;

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



