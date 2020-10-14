#include "pch.h"
#include "Level4.h"
#include "IO.h"
#include "Cube.h"
#include <random>

concurrency::task<void> Dx::Levels::Level4::Load()
{
	return concurrency::create_task([this]
		{
			mVertexShaderSimple = VertexShader::Preload(VertexType::Simple, L"VertexShader4.cso");
			mPixelShaderSimple = PixelShader::Preload(VertexType::Simple, L"PixelShader4.cso");

			SetupModel();
		}
	);
}

void Dx::Levels::Level4::SetupModel()
{
	Dx::Attachables::Attachable::Reset();
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_real_distribution<float> location(-140.0f, 140.0f);
	std::uniform_real_distribution<float> movementSpeed(.0f, .0f);
	std::uniform_real_distribution<float> startAngle(-DirectX::XM_PI, DirectX::XM_PI);
	std::uniform_real_distribution<float> rotationSpeed(-DirectX::XM_PI/10.f, DirectX::XM_PI/10.f);
	std::uniform_real_distribution<float> scale(0.2f,4.0f);

	for (int i = 0; i <= 5000; i++)
	{
		auto cube = std::make_unique<Cube>(mVertexShaderSimple, mPixelShaderSimple);

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
		cube->Init();

		mDrawables.push_back(std::move(cube));
	}

	for (auto d : mDrawables) {
		d->RegisterResources();
	}

	mWorldViewTransformConstantBuffer = VSConstantBuffer<DirectX::XMMATRIX>::Create(mWorldViewTransform, ResourceSlots::PerLevel, false);
	mWorldViewTransformConstantBuffer->Attach();

	mWorldRotationSpeedY = 0.01f;
}

void Dx::Levels::Level4::Update(float delta)
{
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

	mWorldViewTransform *= DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, Graphics::Instance->Width() / Graphics::Instance->Height(), 1.f, 100.0f);
	mWorldViewTransformConstantBuffer->Update(mWorldViewTransform);


	for (auto d : mDrawables)
		d->Update(delta, DirectX::XMMatrixIdentity());
}

void Dx::Levels::Level4::Render()
{
	float color[4]{ .2f, .2f, .3f, .2f};
	Graphics::Instance->StartFrame(color);

	for (auto d : mDrawables)
		d->Draw();
}