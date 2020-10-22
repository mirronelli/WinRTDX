#pragma once
#include "pch.h"

#include <random>

#include "LevelBase.h"
#include "Drawable.h"
#include "IO.h"
#include "Camera.h"
#include "Scene.h"
#include "SphereColoredWithNormal.h"
#include "SphereColored.h"
#include "SceneFactory.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level9 : public LevelBase
	{
	public:
		using LevelBase::LevelBase;
		concurrency::task<void> Load()
		{
			return concurrency::create_task(
				[this]()
				{
					VertexShader::Preload(Dx::Drawables::VertexType::Colored, L"9_VertexColored.cso");
					VertexShader::Preload(Dx::Drawables::VertexType::ColoredWithNormal, L"9_VertexColoredWithNormal.cso");
					VertexShader::Preload(Dx::Drawables::VertexType::SimpleWithNormal, L"9_VertexSimpleWithNormal.cso");
					VertexShader::Preload(Dx::Drawables::VertexType::TexturedWithNormal, L"9_VertexTexturedWithNormal.cso");
					VertexShader::Preload(Dx::Drawables::VertexType::TexturedWithNormalTangent, L"9_VertexTexturedWithNormalTangent.cso");
					
					PixelShader::Preload(Dx::Drawables::VertexType::Colored, L"9_PixelColored.cso");
					PixelShader::Preload(Dx::Drawables::VertexType::ColoredWithNormal, L"9_PixelColoredWithNormal.cso");
					PixelShader::Preload(Dx::Drawables::VertexType::SimpleWithNormal, L"9_PixelSimpleWithNormal.cso");
					PixelShader::Preload(Dx::Drawables::VertexType::TexturedWithNormal, L"9_PixelTexturedWithNormal.cso");
					PixelShader::Preload(Dx::Drawables::VertexType::TexturedWithNormalTangent, L"9_PixelTexturedWithNormalTangent.cso");

					SetupModel();
				}
			);
		}

		void Start()
		{
			//mMouseInput->RelativeTrackingEnter();
		}

		void SetupModel()
		{
			Dx::Attachables::Attachable::Reset();
			std::unique_ptr<Dx::Drawables::Scene> importedScene;

			importedScene = SceneFactory::Create("Assets\\nano_textured\\nanosuit.obj");
			importedScene->Transform(
				XMMatrixScaling(2, 2, 2)
				* XMMatrixTranslation(0, 0, 40));
			importedScene->RotationSpeedY(0.01f);
			mRootScene.AddScene(std::move(importedScene));

			importedScene = SceneFactory::Create("Assets\\muro\\muro.obj");
			importedScene->Transform(
				XMMatrixScaling(18, 18, 18)
				* XMMatrixTranslation(20, 0, 40));
			importedScene->RotationSpeedY(0.01f);
			mRootScene.AddScene(std::move(importedScene));

			importedScene = SceneFactory::Create("Assets\\brick_wall\\brick_wall.obj");
			importedScene->Transform(
				XMMatrixScaling(80, 80, 80)
				* XMMatrixTranslation(-20, 20, 120));
			mRootScene.AddScene(std::move(importedScene));

			importedScene = SceneFactory::Create("Assets\\goblin\\GoblinX.obj");
			importedScene->Transform(
				XMMatrixScaling(7,7, 7)
				* XMMatrixTranslation(-30, 30, 40));
			importedScene->RotationSpeedY(0.05f);
			mRootScene.AddScene(std::move(importedScene));

			importedScene = SceneFactory::Create("Assets\\suzanne.obj");
			importedScene->Transform(
				XMMatrixScaling(10, 10, 10)
				* XMMatrixTranslation(50, 0, 0));
			importedScene->RotationSpeedY(-0.1f);
			mRootScene.AddScene(std::move(importedScene));

			auto theSun = std::make_unique<Dx::Drawables::SphereColored>(40);
			theSun->Scale(10);
			theSun->ColorRanges(XMFLOAT3(0.8f, 0.4f, 0), XMFLOAT3(1, 0.7f, 0));
			theSun->Init();
			mRootScene.AddDrawable(std::move(theSun));
			
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

			mPixelPerLevelConstantsBuffer->Attach();
			mPixelPerFrameConstantsBuffer->Attach();
			mVertexPerFrameConstantsBuffer->Attach();

			mCamera = Camera(XMVectorSet(0, 30, -50, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0));
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

			mRootScene.mScenes[0]->RotationSpeedY(crytekRotatioSpeed);
			mRootScene.mScenes[1]->RotationSpeedY(moroRotatioSpeed);
			mRootScene.mScenes[3]->RotationSpeedY(goblinRotatioSpeed);

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
			RenderGui();
		}

		void RenderGui()
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(400, 0));
			ImGui::Begin("Rotation");												  // Create a window called "Hello, world!" and append into it.
			ImGui::SliderFloat("Moro", &moroRotatioSpeed, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Goblin", &goblinRotatioSpeed, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Crytek", &crytekRotatioSpeed, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::Text("(%.0f FPS)", ImGui::GetIO().Framerate);
			ImGui::End();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

	private:
		float goblinRotatioSpeed = 0;
		float moroRotatioSpeed = 0;
		float crytekRotatioSpeed = 0;


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

		Camera																		mCamera{};
		float																			mCameraMovementSpeed = 1;
		float																			mMouseSensitivity = .0006f;
	};
}