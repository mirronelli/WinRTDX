#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>

namespace Dx::Attachables
{
	template<class T>
	class ConstantBuffer : public Attachable
	{
	public:
		static std::shared_ptr<ConstantBuffer<T>> Create(std::string const& key, bool overwrite, std::shared_ptr<Graphics> graphics, T& constantData)
		{
			std::shared_ptr<ConstantBuffer<T>> instance = m_instances[key];

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<ConstantBuffer>(graphics, vertices, constantData);
				m_instances[key] = instance;
			}

			return instance;
		}

			
		ConstantBuffer(std::string key, std::shared_ptr<Graphics> graphics, T & constantData)
			: Attachable(graphics)
		{
			D3D11_BUFFER_DESC	desc{ 0 };
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;
			desc.ByteWidth = sizeof(T);

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = &constantData;

			HRESULT hr = m_device->CreateBuffer(&desc, &srd, m_buffer.put());
		}

		void AttachForPixelShader()
		{
			ID3D11Buffer* constantBuffers[1] = { m_buffer.get() };
			m_context->PSSetConstantBuffers(0, 1, constantBuffers);
		}
		
		void AttachForVertexShader()
		{
			ID3D11Buffer* constantBuffers[1] = { m_buffer.get() };
			m_context->VSSetConstantBuffers(0, 1, constantBuffers);
		}

		void AttachPrivate()
		{
			AttachForVertexShader();
			AttachForPixelShader();
		}

		void Update(T const& constantData)
		{
			m_context->UpdateSubresource(m_buffer.get(), 0, 0, &constantData, 0, 0);
		}

	private:
		inline static std::shared_ptr<std::map<std::string, com_ptr<ID3D11Buffer>>> m_buffers = std::make_shared< std::map<std::string, com_ptr<ID3D11Buffer>>>();
		com_ptr<ID3D11Buffer> m_buffer;
	};
}
