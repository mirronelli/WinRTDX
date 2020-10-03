#include "pch.h"

#include "ObjectInSpace.h"

using namespace DirectX;

namespace Dx::Drawables
{
	ObjectInSpace::ObjectInSpace() : mTransform(XMMatrixIdentity()), mInitialQuaternion (XMQuaternionIdentity()) { };

	void ObjectInSpace::Update(float delta, CXMMATRIX parentMatrix) 
	{
		mRotationX = fmod(mRotationX + delta * mRotationSpeedX * XM_2PI, XM_2PI);
		mRotationY = fmod(mRotationY + delta * mRotationSpeedY * XM_2PI, XM_2PI);
		mRotationZ = fmod(mRotationZ + delta * mRotationSpeedZ * XM_2PI, XM_2PI);

		mX += mSpeedX * delta;
		mY += mSpeedY * delta;
		mZ += mSpeedZ * delta;

		mTransform =
			XMMatrixRotationQuaternion(mInitialQuaternion)
			
			* XMMatrixScaling(mScaleX, mScaleY, mScaleX)

			// rotate object
			* XMMatrixRotationZ(mRotationZ)
			* XMMatrixRotationX(mRotationX)
			* XMMatrixRotationY(mRotationY)

			// move object to its coords in parent
			* XMMatrixTranslation(mX, mY, mZ)

			// apply parent matrix
			* parentMatrix;
	};

	void ObjectInSpace::X(float value) { mX = value; }
	void ObjectInSpace::Y(float value) { mY = value; }
	void ObjectInSpace::Z(float value) { mZ = value; }

	void ObjectInSpace::SpeedX(float value) { mSpeedX = value; }
	void ObjectInSpace::SpeedY(float value) { mSpeedY = value; }
	void ObjectInSpace::SpeedZ(float value) { mSpeedZ = value; }

	void ObjectInSpace::RotationSpeedX(float value) { mRotationSpeedX = value; }
	void ObjectInSpace::RotationSpeedY(float value) { mRotationSpeedY = value; }
	void ObjectInSpace::RotationSpeedZ(float value) { mRotationSpeedZ = value; }

	void ObjectInSpace::RotationX(float value) { mRotationX = value; }
	void ObjectInSpace::RotationY(float value) { mRotationY = value; }
	void ObjectInSpace::RotationZ(float value) { mRotationZ = value; }

	void ObjectInSpace::ScaleX(float value) { mScaleX = value; }
	void ObjectInSpace::ScaleY(float value) { mScaleY = value; }
	void ObjectInSpace::ScaleZ(float value) { mScaleZ = value; }

	void ObjectInSpace::Scale(float value) { mScaleX = value; mScaleY = value, mScaleZ = value; }

	void ObjectInSpace::Transform(CXMMATRIX transform)
	{
		XMVECTOR translationVector;
		XMVECTOR rotationVector;
		XMVECTOR scaleVector;
		
		XMFLOAT3 translation;
		XMFLOAT3 scale;

		XMMatrixDecompose(&scaleVector, &mInitialQuaternion, &translationVector, transform);
		XMStoreFloat3(&translation, translationVector);
		XMStoreFloat3(&scale, scaleVector);

		mX = translation.x;
		mY = translation.y;
		mZ = translation.z;

		mScaleX = scale.x;
		mScaleY = scale.y;
		mScaleZ = scale.z;

		// the rotation is extracted to mInitialQuaternion, these need to be reset to zero
		mRotationX = 0;
		mRotationY = 0;
		mRotationZ = 0;
	}
}