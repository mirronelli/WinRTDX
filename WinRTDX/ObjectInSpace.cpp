#include "pch.h"
#include "pch.h"
#include "ObjectInSpace.h"

namespace Dx::Drawables
{
	ObjectInSpace::ObjectInSpace() : mTransform() {};

	void ObjectInSpace::Update(float delta, DirectX::CXMMATRIX parentMatrix) 
	{
		mRotationX = fmod(mRotationX + delta * mRotationSpeedX * DirectX::XM_2PI, DirectX::XM_2PI);
		mRotationY = fmod(mRotationY + delta * mRotationSpeedY * DirectX::XM_2PI, DirectX::XM_2PI);
		mRotationZ = fmod(mRotationZ + delta * mRotationSpeedZ * DirectX::XM_2PI, DirectX::XM_2PI);

		mX += mSpeedX * delta;
		mY += mSpeedY * delta;
		mZ += mSpeedZ * delta;

		mTransform =
			parentMatrix
			* DirectX::XMMatrixScaling(mScaleX, mScaleY, mScaleX)

			// rotate object
			* DirectX::XMMatrixRotationZ(mRotationZ)
			* DirectX::XMMatrixRotationX(mRotationX)
			* DirectX::XMMatrixRotationY(mRotationY)

			// move object to its world coords
			* DirectX::XMMatrixTranslation(mX, mY, mZ);
	};
}