#pragma once
#include "pch.h"

#include <random>

#include "Drawable.h"
#include "CubeColored.h"
#include "SphereColored.h"
#include "SphereColoredWithNormal.h"
#include "CubeTextured.h"
#include "LevelBase.h"
#include "IO.h"
#include "Camera.h"
#include <DirectXMath.h>
#include "Structures.h"

using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level7 : public LevelBase
	{
	public:
		using LevelBase::LevelBase;
		concurrency::task<void> Load()
		{
			return concurrency::create_task([this]
				{
					mVertexShaderTextured = VertexShader::Preload(VertexType::TexturedWithNormal, L"VertexShader7Textured.cso");
					mVertexShaderColored = VertexShader::Preload(VertexType::ColoredWithNormal, L"9_VertexColoredWithNormal.cso");
					mVertexShaderStatic = VertexShader::Preload(VertexType::Colored, L"9_VertexColored.cso");

					mPixelShaderTextured	= PixelShader::Preload(VertexType::TexturedWithNormal, L"PixelShader7Textured.cso");
					mPixelShaderColored = PixelShader::Preload(VertexType::ColoredWithNormal, L"9_PixelColoredWithNormal.cso");
					mPixelShaderStatic = PixelShader::Preload(VertexType::Colored, L"9_PixelColored.cso");

					mTexture = Texture::Preload("karin", L"Assets\\karin3.dds");

					SetupModel();
				}
			);
		}

		void SetupModel()
		{
			Dx::Attachables::Attachable::Reset();

			GenerateDrawables();

			mVertexPerFrameConstantsBuffer = VSConstantBuffer<VertexPerFrameConstants>::Create(mVertexPerFrameConstants, ResourceSlots::PerFrame);
			mVertexPerFrameConstantsBuffer->Attach();

			mPixelPerFrameConstantsBuffer = PSConstantBuffer<PixelPerFrameConstants>::Create(mPixelPerFrameConstants, ResourceSlots::PerFrame);
			mPixelPerFrameConstantsBuffer->Attach();

			mLight.lightPosition				= { 0, 0, 0, 0 };
			mLight.lightColor					= { 1.0f, 1.0f, 1.0, 0.0f };
			mLight.ambientLight					= { 0.1f, 0.1f, 0.1f, 0.0f };
			mLight.diffuseIntensity			= 1.0f;
			mLight.attenuationQuadratic		= 0.00001f;
			mLight.attenuationLinear			= 0.01f;
			mLight.attenuationConstant		= 0.f;

			mLightConstantBuffer = PSConstantBuffer<PSConstants>::Create(mLight, ResourceSlots::PerLevel);
			mLightConstantBuffer->Attach();

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

			for (int i = 0; i <= 100; i++)
			{
				float radius = scale(generator);
				auto cube = std::make_unique<CubeTextured>(mVertexShaderTextured, mPixelShaderTextured);

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

				cube->ScaleX(radius);
				cube->ScaleY(radius);
				cube->ScaleZ(radius);

				cube->Texture(mTexture);
				cube->Init();

				mDrawables.push_back(std::move(cube));
			}

			for (int i = 0; i <= 100; i++)
			{
				float radius = scale(generator);
				auto cube = std::make_unique<CubeColored>(mVertexShaderColored, mPixelShaderColored);

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

				cube->ScaleX(radius);
				cube->ScaleY(radius);
				cube->ScaleZ(radius);
				cube->Init();

				mDrawables.push_back(std::move(cube));
			}

			for (int i = 0; i <= 100; i++)
			{
				float radius = scale(generator);
				auto sphere = std::make_unique<SphereColoredWithNormal>(24);

				sphere->X(location(generator));
				sphere->Y(location(generator));
				sphere->Z(location(generator));

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

				sphere->Init();

				mDrawables.push_back(std::move(sphere));
			}

			auto theSun = std::make_unique<SphereColored>(40);

			theSun->ScaleX(10);
			theSun->ScaleY(10);
			theSun->ScaleZ(10);
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

		struct PSConstants {
			XMFLOAT4	lightPosition;
			XMFLOAT4	lightColor;
			XMFLOAT4	ambientLight;
			
			float		diffuseIntensity;
			float		attenuationQuadratic;
			float		attenuationLinear;
			float		attenuationConstant;
		} ;

		std::vector<std::shared_ptr<Drawable>>								mDrawables;

		std::shared_ptr<VertexShader>											mVertexShaderTextured;
		std::shared_ptr<VertexShader>											mVertexShaderColored;
		std::shared_ptr<VertexShader>											mVertexShaderStatic;
		std::shared_ptr<PixelShader>											mPixelShaderTextured;
		std::shared_ptr<PixelShader>											mPixelShaderColored;
		std::shared_ptr<PixelShader>											mPixelShaderStatic;

		std::shared_ptr<Texture>												mTexture;

		VertexPerFrameConstants													mVertexPerFrameConstants;
		std::shared_ptr<VSConstantBuffer<VertexPerFrameConstants>>	mVertexPerFrameConstantsBuffer;
		PixelPerFrameConstants													mPixelPerFrameConstants;
		std::shared_ptr<PSConstantBuffer<PixelPerFrameConstants>>	mPixelPerFrameConstantsBuffer;

		PSConstants																	mLight;
		std::shared_ptr<PSConstantBuffer<PSConstants>>					mLightConstantBuffer;

		Camera																		mCamera = Camera(DirectX::XMVectorSet(0,0,-120,0), XMVectorSet(0,0,1,0), XMVectorSet(0,1,0,0));

		float																			mCameraMovementSpeed = 1;
		float																			mMouseSensitivity = .0006f;
	};
}



