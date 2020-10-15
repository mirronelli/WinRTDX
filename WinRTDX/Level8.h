#pragma once
#include "pch.h"

#include <random>

#include "LevelBase.h"
#include "Drawable.h"
#include "CubeColored.h"
#include "SphereColored.h"
#include "CubeTextured.h"
#include "Mesh.h"
#include "IO.h"
#include "Camera.h"
#include "SceneFactory.h"

using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level8 : public LevelBase
	{
	public:
		using LevelBase::LevelBase;
		concurrency::task<void> Load()
		{
			return concurrency::create_task(
				[this]()
				{
					mVertexShaderWithNormal = VertexShader::Preload(VertexType::SimpleWithNormal, L"8_VertexWithNormal.cso");
					mVertexShaderWithColor = VertexShader::Preload(VertexType::ColoredWithNormal, L"8_VertexWithColor.cso");
					mPixelShaderWithNormal = PixelShader::Preload(VertexType::SimpleWithNormal, L"8_PixelWithNormal.cso");
					mPixelShaderWithColor = PixelShader::Preload(VertexType::ColoredWithNormal, L"8_PixelWithColor.cso");

					SetupModel();
				}
			);
		}

		void SetupModel()
		{
			Dx::Attachables::Attachable::Reset();
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
		}

		void Start()
		{
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
			std::uniform_real_distribution<float> specularPower(0.f, 640.0f);

			for (int i = 0; i < 50; i++)
			{
				std::unique_ptr<Scene> importedScene = SceneFactory::Create("Assets\\suzanne.obj");
				importedScene->Transform(
					XMMatrixScaling(10, 10, 10)
					* XMMatrixTranslation(location(generator), location(generator), location(generator)));
				importedScene->RotationSpeedY(0.1f);

				importedScene->RotationSpeedX(rotationSpeed(generator));
				importedScene->RotationSpeedY(rotationSpeed(generator));
				importedScene->RotationSpeedZ(rotationSpeed(generator));

				static_cast<Mesh*>(importedScene->mScenes[0]->mDrawables[0].get())->Color({ color(generator), color(generator), color(generator), 1 });
				static_cast<Mesh*>(importedScene->mScenes[0]->mDrawables[0].get())->Specular({ reflectiveness(generator),reflectiveness(generator),reflectiveness(generator),1}, specularPower(generator));
				mRootScene.AddScene(std::move(importedScene));
			}

			for (int i = 0; i < 50; i++)
			{
				std::unique_ptr<Scene> importedScene = SceneFactory::Create("Assets\\nanosuit.obj");
				importedScene->Transform(
					XMMatrixScaling(2, 2, 2)
					* XMMatrixTranslation(location(generator), location(generator), location(generator)));
				importedScene->RotationSpeedY(0.1f);

				importedScene->RotationSpeedX(rotationSpeed(generator));
				importedScene->RotationSpeedY(rotationSpeed(generator));
				importedScene->RotationSpeedZ(rotationSpeed(generator));

				for (int j = 0; j < importedScene->mScenes.size(); j++)
				{
					static_cast<Mesh*>(importedScene->mScenes[j]->mDrawables[0].get())->Color(
						{ color(generator), color(generator), color(generator), 1 }
					);
					static_cast<Mesh*>(importedScene->mScenes[j]->mDrawables[0].get())->Specular(
						{ reflectiveness(generator),reflectiveness(generator),reflectiveness(generator), 1 }, 
						specularPower(generator)
					);
				}
				mRootScene.AddScene(std::move(importedScene));
			}

			auto theSun = std::make_unique<SphereColoredWithNormal>(40);

			theSun->Scale(10);
			theSun->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun->Init();

			mRootScene.AddDrawable(std::move(theSun));
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
			
			mRootScene.Update(delta, XMMatrixIdentity());
		}

		void Render()
		{
			float color[4]{ 0.0f, .0f, .02f, .0f };
			Graphics::Instance->StartFrame(color);

			mRootScene.Draw();
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

		Scene																			mRootScene;
	};
}



