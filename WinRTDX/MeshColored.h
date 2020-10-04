#pragma once
#include "pch.h"
#include "Drawable.h"
#include "Structures.h"
#include "SceneFactory.h"

namespace Dx::Drawables
{

	class MeshColored : public Drawable
	{
		friend class Dx::Levels::SceneFactory;

	public:
		using Drawable::Drawable;
		void Color(DirectX::XMFLOAT4 color);
		void Specular(float reflectiveness, float reflectionPower);

	protected:
		void RegisterResources();
		void UpdateConstants(DirectX::CXMMATRIX worldTransform);

	private:
		ColorSpecular							mPixelPerInstanceConstants;
		WorldTransform							mVertexPerInstanceConstants;
		std::vector<VertexSimpleWithNormal>		mVertices;
		std::vector<UINT>						mIndices;
	};

}
