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
					mVertexShaderTextured = VertexShader::Preload(VertexType::TexturedWithNormal, L"VertexShader5_6.cso");
					mPixelShaderTextured = PixelShader::Preload(VertexType::TexturedWithNormal, L"PixelShader5_6.cso");

					mTexture = Texture::Preload("karin", L"Assets\\karin3.dds");
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

				cube->ScaleX(scale(generator));
				cube->ScaleY(scale(generator));
				cube->ScaleZ(scale(generator));

				cube->Texture(mTexture);
				cube->Init();

				mDrawables.push_back(std::move(cube));
			}

			for (auto d : mDrawables) {
				d->RegisterResources();
			}

			mWorldViewTransformConstantBuffer = VSConstantBuffer<DirectX::XMMATRIX>::Create(mWorldViewTransform, ResourceSlots::PerLevel);
			mWorldViewTransformConstantBuffer->Attach();
			mWorldRotationSpeedY = 0.f;
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

			//m_camera.Rotate(0, 0.01, 0);
			mCamera.Rotate(
				-mMouseInput->RelativeDeltaY() * mMouseSensitivity,
				mMouseInput->RelativeDeltaX() * mMouseSensitivity
			);
		}

		void Update(float delta)
		{
			ProcessInput();

			mWorldRotationX = fmod(mWorldRotationX + delta * mWorldRotationSpeedX * DirectX::XM_2PI, DirectX::XM_2PI);
			mWorldRotationY = fmod(mWorldRotationY + delta * mWorldRotationSpeedY * DirectX::XM_2PI, DirectX::XM_2PI);
			mWorldRotationZ = fmod(mWorldRotationZ + delta * mWorldRotationSpeedZ * DirectX::XM_2PI, DirectX::XM_2PI);

			mWorldViewTransform = DirectX::XMMatrixIdentity();
			if (mWorldRotationZ != 0)
				mWorldViewTransform *= DirectX::XMMatrixRotationZ(mWorldRotationZ);

			if (mWorldRotationX != 0)
				mWorldViewTransform *= DirectX::XMMatrixRotationX(mWorldRotationX);

			if (mWorldRotationY != 0)
				mWorldViewTransform *= DirectX::XMMatrixRotationY(mWorldRotationY);

			mWorldViewTransform *= mCamera.GetMatrix();
			mWorldViewTransform *= DirectX::XMMatrixPerspectiveFovLH(1.2f, Dx::Graphics::Instance->Width() / Dx::Graphics::Instance->Height(), .1f, 1000.0f);
			mWorldViewTransformConstantBuffer->Update(mWorldViewTransform);

			for (auto d : mDrawables)
				d->Update(delta, XMMatrixIdentity());
		}

		void Render()
		{
			float color[4]{ 0.3f, .0f, .1f, .2f };
			Graphics::Instance->StartFrame(color);

			for (auto d : mDrawables)
				d->Draw();
		}

	private:
		std::vector<std::shared_ptr<Drawable>>								mDrawables;
		std::shared_ptr<VertexShader>											mVertexShaderSimple;
		std::shared_ptr<PixelShader>											mPixelShaderSimple;
		std::shared_ptr<VertexShader>											mVertexShaderTextured;
		std::shared_ptr<PixelShader>											mPixelShaderTextured;
		std::shared_ptr<Texture>												mTexture;

		float																			mWorldRotationX{ 0 };
		float																			mWorldRotationY{ 0 };
		float																			mWorldRotationZ{ 0 };

		float																			mWorldRotationSpeedX{ 0 };
		float																			mWorldRotationSpeedY{ 0 };
		float																			mWorldRotationSpeedZ{ 0 };

		DirectX::XMMATRIX															mWorldViewTransform{};
		std::shared_ptr<VSConstantBuffer<DirectX::XMMATRIX>>			mWorldViewTransformConstantBuffer;
		Camera																		mCamera;

		float																			mCameraMovementSpeed = 1;
		float																			mCameraRotationSpeed = .03f;
		float																			mMouseSensitivity = .0006f;
	};
}



