#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>

namespace Dx::Attachables
{
	template<class T>
	class VertexBuffer : public Attachable
	{
	public:
		static std::shared_ptr<VertexBuffer<T>> Create(std::string const& key, bool overwrite, std::shared_ptr<Graphics> graphics, std::vector<T> const& vertices) 
		{
			std::shared_ptr<VertexBuffer<T>> instance = m_instances[key];

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<VertexBuffer>(graphics, vertices);
				m_instances[key] = instance;
			}

			return instance;
		}

		VertexBuffer(std::shared_ptr<Graphics> graphics, std::vector<T> const& vertices)	
			: Attachable(graphics)
		{
			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = vertices.data();

			m_device->CreateBuffer(&desc, &srd, m_buffer.put());
		}

		void AttachPrivate()
		{
			UINT strideVertices = sizeof(T);
			UINT offsetVertices = 0;
			ID3D11Buffer* vertexBuffers[1] = { m_buffer.get() };
			m_context->IASetVertexBuffers(0, 1, vertexBuffers, &strideVertices, &offsetVertices);
		}

	private:
		inline static std::map<std::string, std::shared_ptr<VertexBuffer<T>>> m_instances = {};
		com_ptr<ID3D11Buffer> m_buffer;
	};
}
