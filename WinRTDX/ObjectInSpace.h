#pragma once

namespace Dx::Drawables
{

	class ObjectInSpace
	{
	public:
		ObjectInSpace();
		virtual ~ObjectInSpace() {};

		virtual void Update(float delta, DirectX::CXMMATRIX parentMatrix);

		void X(float value) { mX = value; }
		void Y(float value) { mY = value; }
		void Z(float value) { mZ = value; }

		void SpeedX(float value) { mSpeedX = value; }
		void SpeedY(float value) { mSpeedY = value; }
		void SpeedZ(float value) { mSpeedZ = value; }

		void RotationSpeedX(float value) { mRotationSpeedX = value; }
		void RotationSpeedY(float value) { mRotationSpeedY = value; }
		void RotationSpeedZ(float value) { mRotationSpeedZ = value; }

		void RotationX(float value) { mRotationX = value; }
		void RotationY(float value) { mRotationY = value; }
		void RotationZ(float value) { mRotationZ = value; }

		void ScaleX(float value) { mScaleX = value; }
		void ScaleY(float value) { mScaleY = value; }
		void ScaleZ(float value) { mScaleZ = value; }
		void Scale(float value) { mScaleX = value; mScaleY = value, mScaleZ = value; }

	protected:
		DirectX::XMMATRIX	mTransform;

		float	mX = 0;
		float	mY = 0;
		float	mZ = 0;

		float	mSpeedX = 0;
		float	mSpeedY = 0;
		float	mSpeedZ = 0;

		float	mScaleX = 1;
		float	mScaleY = 1;
		float	mScaleZ = 1;

		float	mRotationSpeedX = 0;
		float	mRotationSpeedY = 0;
		float	mRotationSpeedZ = 0;

		float	mRotationX = 0;
		float	mRotationY = 0;
		float	mRotationZ = 0;
	};
}