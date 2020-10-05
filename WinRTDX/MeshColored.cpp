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
		m_vertexBuffer =		VertexBuffer<VertexSimpleWithNormal>	::Get("mesh:" + mName, mVertices);
		m_indexBuffer =		IndexBuffer										::Get("mesh:" + mName, mIndices);
		m_inputLayout =		InputLayout										::Get(VertexType::SimpleWithNormal);
		m_vsConstantBuffer = VSConstantBuffer<WorldTransform>			::Create(id++, false, mVertexPerInstanceConstants, (UINT)ResourceSlots::PerInstance);
		m_psConstantBuffer = PSConstantBuffer<ColorSpecular>			::Create(id++, false, mPixelPerInstanceConstants, (UINT)ResourceSlots::PerInstance);
		m_indicesCount = (UINT)mIndices.size();
	}

	void MeshColored::UpdateConstants(DirectX::CXMMATRIX worldTransform)
	{
		mVertexPerInstanceConstants.worldTransform = worldTransform;

		std::static_pointer_cast<VSConstantBuffer<WorldTransform>>		(m_vsConstantBuffer)->Update(mVertexPerInstanceConstants);
		std::static_pointer_cast<PSConstantBuffer<ColorSpecular>>		(m_psConstantBuffer)->Update(mPixelPerInstanceConstants);
	}
}