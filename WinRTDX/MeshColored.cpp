#include "pch.h"
#include "MeshColored.h"


namespace Dx::Drawables
{
	void MeshColored::Color(DirectX::XMFLOAT4 color)
	{
		mPixelPerInstanceConstants.color = color;
	}

	void MeshColored::Specular(float reflectiveness, float reflectionPower)
	{
		mPixelPerInstanceConstants.reflectiveness = reflectiveness;
		mPixelPerInstanceConstants.reflectionPower = reflectionPower;
	}

	void MeshColored::RegisterResources()
	{
		id++;
		//mVertexBuffer =		VertexBuffer<VertexSimpleWithNormal>	::Get("mesh:" + mName, mVertices);
		//mIndexBuffer =		IndexBuffer										::Get("mesh:" + mName, mIndices);
		mInputLayout =		InputLayout										::Get(VertexType::SimpleWithNormal);
		mVsConstantBuffer = VSConstantBuffer<WorldTransform>			::Create(mVertexPerInstanceConstants, ResourceSlots::PerInstance);
		mPsConstantBuffer = PSConstantBuffer<ColorSpecular>			::Create(mPixelPerInstanceConstants, ResourceSlots::PerInstance);
		mIndicesCount = (UINT)mIndices.size();
	}

	void MeshColored::UpdateConstants(DirectX::CXMMATRIX worldTransform)
	{
		mVertexPerInstanceConstants.worldTransform = worldTransform;

		std::static_pointer_cast<VSConstantBuffer<WorldTransform>>		(mVsConstantBuffer)->Update(mVertexPerInstanceConstants);
		std::static_pointer_cast<PSConstantBuffer<ColorSpecular>>		(mPsConstantBuffer)->Update(mPixelPerInstanceConstants);
	}
}