#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	template<class T>
	class PSConstantBuffer : public Attachable
	{
	public:
		static std::shared_ptr<PSConstantBuffer<T>> Create(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, T& constantData, UINT slot = 0)
		{
			std::shared_ptr<PSConstantBuffer<T>> instance = std::static_pointer_cast<PSConstantBuffer<T>>(ResourceManager::PSConstantBuffers[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<PSConstantBuffer>(key, graphics, constantData, slot);
				ResourceManager::PSConstantBuffers[key] = instance;
			}

			return instance;
		}

		PSConstantBuffer(uint16_t key, std::shared_ptr<Graphics> graphics, T& constantData, UINT slot)
			: Attachable(key, graphics),
			m_slot(slot)
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

		void AttachPrivate(bool force)
		{
			if (force || ResourceManager::CurrentPSConstantBuffer != m_key)
			{
				ID3D11Buffer* PSConstantBuffers[1] = { m_buffer.get() };
				m_context->PSSetConstantBuffers(m_slot, 1, PSConstantBuffers);
				ResourceManager::CurrentPSConstantBuffer = m_key;
			}
		}

		void Update(T const& constantData)
		{
			m_context->UpdateSubresource(m_buffer.get(), 0, 0, &constantData, 0, 0);
		}

	private:
		com_ptr<ID3D11Buffer>	m_buffer;
		UINT							m_slot;
	};
}
