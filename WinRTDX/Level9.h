#pragma once
#include "pch.h"

#include <random>

#include "ILevel.h"
#include "Drawable.h"
#include "IO.h"
#include "Camera.h"
#include "Scene.h"
#include "SphereColoredWithNormal.h"
#include "SphereColored.h"
#include "SceneFactory.h"

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
					VertexShader::Preload(Dx::Drawables::VertexType::Colored, L"9_VertexColored.cso");
					VertexShader::Preload(Dx::Drawables::VertexType::ColoredWithNormal, L"9_VertexColoredWithNormal.cso");
					VertexShader::Preload(Dx::Drawables::VertexType::SimpleWithNormal, L"9_VertexSimpleWithNormal.cso");
					
					PixelShader::Preload(Dx::Drawables::VertexType::Colored, L"9_PixelColored.cso");
					PixelShader::Preload(Dx::Drawables::VertexType::ColoredWithNormal, L"9_PixelColoredWithNormal.cso");
					PixelShader::Preload(Dx::Drawables::VertexType::SimpleWithNormal, L"9_PixelSimpleWithNormal.cso");
				}
			);
		}

		void SetupModel()
		{
			std::unique_ptr<Dx::Drawables::Scene> importedScene;

			importedScene = SceneFactory::LoadFromFile("Assets\\suzanne.obj");
			importedScene->Transform(
				XMMatrixScaling(10, 10, 10)
				* XMMatrixTranslation(-50, 0, 0));
			importedScene->RotationSpeedY(0.1f);
			mRootScene.AddScene(std::move(importedScene));

			importedScene = SceneFactory::LoadFromFile("Assets\\suzanne.obj");
			importedScene->Transform(
				XMMatrixScaling(10, 10, 10)
				* XMMatrixTranslation(50, 0, 0));
			importedScene->RotationSpeedY(-0.1f);
			mRootScene.AddScene(std::move(importedScene));

			importedScene = SceneFactory::LoadFromFile("Assets\\nanosuit.obj");
			importedScene->Transform(
				XMMatrixTranslation(0, -10, 100));
			importedScene->RotationSpeedY(-0.1f);
			mRootScene.AddScene(std::move(importedScene));

			AddSun();

			mPixelPerLevelConstants.lightPosition = { 0, 0, 0, 0 };
			mPixelPerLevelConstants.lightColor = { 1.0f, 1.0f, 1.0, 0.0f };
			mPixelPerLevelConstants.ambientLight = { 0.1f, 0.01f, 0.01f, 0.2f };
			mPixelPerLevelConstants.diffuseIntensity = 1.0f;
			mPixelPerLevelConstants.attenuationQuadratic = 0.00001f;
			mPixelPerLevelConstants.attenuationLinear = 0.01f;
			mPixelPerLevelConstants.attenuationConstant = 0.f;

			mPixelPerLevelConstantsBuffer = PSConstantBuffer<PixelPerLevelConstants>::Create(mPixelPerLevelConstants, ResourceSlots::PerLevel, false);
			mPixelPerFrameConstantsBuffer = PSConstantBuffer<PixelPerFrameConstants>::Create(mPixelPerFrameConstants, ResourceSlots::PerFrame, true);
			mVertexPerFrameConstantsBuffer = VSConstantBuffer<VertexPerFrameConstants>::Create(mVertexPerFrameConstants, ResourceSlots::PerFrame, true);

			mPixelPerLevelConstantsBuffer->Attach(false);
			mPixelPerFrameConstantsBuffer->Attach(false);
			mVertexPerFrameConstantsBuffer->Attach(false);

			//mRootScene.RotationSpeedY(.05f);

			m_mouseInput->RelativeTrackingEnter();
		}

		void AddSun()
		{
			auto theSun = std::make_unique<Dx::Drawables::SphereColored>(40);

			theSun->Scale(10);
			theSun->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun->Init();

			mRootScene.AddDrawable(std::move(theSun));

			auto theSun2 = std::make_unique<Dx::Drawables::SphereColored>(40);

			theSun2->Scale(10);
			theSun2->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun2->X(-120);
			theSun2->Init();

			mRootScene.AddDrawable(std::move(theSun2));

			auto theSun3 = std::make_unique<Dx::Drawables::SphereColoredWithNormal>(40);

			theSun3->Scale(10);
			theSun3->X(120);
			theSun3->Color({ 1,0.5,0 });
			theSun3->Init();

			mRootScene.AddDrawable(std::move(theSun3));

			auto theSun4 = std::make_unique<Dx::Drawables::SphereColoredWithNormal>(40);

			theSun4->Scale(10);
			theSun4->X(80);
			theSun4->Color({ 1,0.5,0 });
			theSun4->Init();

			mRootScene.AddDrawable(std::move(theSun4));
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

		Dx::Drawables::Scene														mRootScene;

		std::shared_ptr<VertexShader>											mVertexShaderColored;
		std::shared_ptr<VertexShader>											mVertexShaderColoredWithNormal;
		std::shared_ptr<VertexShader>											mVertexShaderSimpleWithNormal;

		std::shared_ptr<PixelShader>											mPixelShaderColored;
		std::shared_ptr<PixelShader>											mPixelShaderColoredWithNormal;
		std::shared_ptr<PixelShader>											mPixelShaderSimpleWithNormal;

		PixelPerFrameConstants													mPixelPerFrameConstants;
		std::shared_ptr<PSConstantBuffer<PixelPerFrameConstants>>	mPixelPerFrameConstantsBuffer;

		VertexPerFrameConstants													mVertexPerFrameConstants;
		std::shared_ptr<VSConstantBuffer<VertexPerFrameConstants>>	mVertexPerFrameConstantsBuffer;

		PixelPerLevelConstants													mPixelPerLevelConstants;
		std::shared_ptr<PSConstantBuffer<PixelPerLevelConstants>>	mPixelPerLevelConstantsBuffer;

		Camera																		mCamera = Camera(DirectX::XMVectorSet(0, 0, -150, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0));
		float																			mCameraMovementSpeed = 1;
		float																			mMouseSensitivity = .0006f;
	};
}