#pragma once

namespace Dx::Drawables
{

	class ObjectInSpace
	{
	public:
		ObjectInSpace();
		virtual ~ObjectInSpace() {};

		virtual void Update(float delta, DirectX::CXMMATRIX parentMatrix);

		void X(float value);
		void Y(float value);
		void Z(float value);

		void SpeedX(float value);
		void SpeedY(float value);
		void SpeedZ(float value);

		void RotationSpeedX(float value);
		void RotationSpeedY(float value);
		void RotationSpeedZ(float value);

		void RotationX(float value);
		void RotationY(float value);
		void RotationZ(float value);

		void ScaleX(float value);
		void ScaleY(float value);
		void ScaleZ(float value);
		void Scale(float value);

		void Transform(DirectX::CXMMATRIX transform);

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