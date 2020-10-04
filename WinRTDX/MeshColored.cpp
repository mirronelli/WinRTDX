#include "pch.h"
#include "MeshColored.h"
#include "Cache.h"


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
		m_vertexBuffer = VertexBuffer<VertexSimpleWithNormal>			::Create(m_resourceCacheID, false, mVertices);
		m_indexBuffer = IndexBuffer											::Create(m_resourceCacheID, false, mIndices);
		m_vsConstantBuffer = VSConstantBuffer<WorldTransform>			::Create(m_resourceCacheID, false, mVertexPerInstanceConstants, (UINT)ResourceSlots::PerInstance);
		m_psConstantBuffer = PSConstantBuffer<ColorSpecular>			::Create(m_resourceCacheID, false, mPixelPerInstanceConstants, (UINT)ResourceSlots::PerInstance);
		m_inputLayout = Cache<VertexType, InputLayout>::Get(VertexType::ColoredWithNormal);
		m_indicesCount = (UINT)mIndices.size();
	}

	void MeshColored::UpdateConstants(DirectX::CXMMATRIX worldTransform)
	{
		mVertexPerInstanceConstants.worldTransform = worldTransform;

		std::static_pointer_cast<VSConstantBuffer<WorldTransform>>		(m_vsConstantBuffer)->Update(mVertexPerInstanceConstants);
		std::static_pointer_cast<PSConstantBuffer<ColorSpecular>>		(m_psConstantBuffer)->Update(mPixelPerInstanceConstants);
	}
}