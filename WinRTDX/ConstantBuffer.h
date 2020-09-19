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
		static void ClearCache() { m_instances.clear(); m_current_pixel_instance_key.clear(); m_current_vertex_instance_key.clear(); }
		static std::shared_ptr<ConstantBuffer<T>> Create(std::wstring const& key, bool overwrite, std::shared_ptr<Graphics> graphics, T& constantData)
		{
			std::shared_ptr<ConstantBuffer<T>> instance = m_instances[key];

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<ConstantBuffer>(key, graphics, constantData);
				m_instances[key] = instance;
			}

			return instance;
		}


		ConstantBuffer(std::wstring key, std::shared_ptr<Graphics> graphics, T& constantData)
			: Attachable(key, graphics)
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

		void AttachForPixelShader(bool force)
		{
			if (force || m_current_pixel_instance_key != m_key)
			{
				ID3D11Buffer* constantBuffers[1] = { m_buffer.get() };
				m_context->PSSetConstantBuffers(0, 1, constantBuffers);
				m_current_pixel_instance_key = m_key;
			}
		}

		void AttachForVertexShader(bool force)
		{
			if (force || m_current_vertex_instance_key != m_key)
			{
				ID3D11Buffer* constantBuffers[1] = { m_buffer.get() };
				m_context->VSSetConstantBuffers(0, 1, constantBuffers);
				m_current_vertex_instance_key = m_key;
			}
		}

		void AttachPrivate(bool force)
		{
			AttachForVertexShader(force);
			AttachForPixelShader(force);
		}

		void Update(T const& constantData)
		{
			m_context->UpdateSubresource(m_buffer.get(), 0, 0, &constantData, 0, 0);
		}

	private:
		inline static std::map<std::wstring, std::shared_ptr<ConstantBuffer<T>>> m_instances = {};
		inline static std::wstring m_current_vertex_instance_key = {};
		inline static std::wstring m_current_pixel_instance_key = {};
		com_ptr<ID3D11Buffer> m_buffer;
	};
}
