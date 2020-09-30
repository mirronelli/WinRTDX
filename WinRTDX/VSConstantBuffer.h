#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	template<class T>
	class VSConstantBuffer : public Attachable
	{
	public:
		static std::shared_ptr<VSConstantBuffer<T>> Create(int key, bool overwrite, T& constantData, UINT slot = 0, bool fastMode = true)
		{
			std::shared_ptr<VSConstantBuffer<T>> instance = std::static_pointer_cast<VSConstantBuffer<T>>(ResourceManager::VSConstantBuffers[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<VSConstantBuffer>(key, constantData, slot, fastMode);
				ResourceManager::VSConstantBuffers[key] = instance;
			}

			return instance;
		}

		VSConstantBuffer(int key, T& constantData, UINT slot, bool fastMode)
			: Attachable(key),
			m_slot(slot),
			m_fastMode(fastMode)
		{
			D3D11_BUFFER_DESC	desc{ 0 };
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			desc.Usage = fastMode ? D3D11_USAGE::D3D11_USAGE_DYNAMIC : D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = fastMode ? D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE : 0;
			desc.ByteWidth = sizeof(T);

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = &constantData;

			Graphics::Device->CreateBuffer(&desc, &srd, m_buffer.put());
		}

		void AttachPrivate(bool force)
		{
			if (force || ResourceManager::CurrentVSConstantBuffer != m_key)
			{
				ID3D11Buffer* VSConstantBuffers[1] = { m_buffer.get() };
				Graphics::Context->VSSetConstantBuffers(m_slot, 1, VSConstantBuffers);
				ResourceManager::CurrentVSConstantBuffer = m_key;
			}
		}

		void Update(T const& constantData)
		{
			if (m_fastMode)
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				Graphics::Context->Map(m_buffer.get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subresource);
				memcpy(subresource.pData, &constantData, sizeof(constantData));
				Graphics::Context->Unmap(m_buffer.get(), 0);
			}
			else
				Graphics::Context->UpdateSubresource(m_buffer.get(), 0, 0, &constantData, 0, 0);
		}

	private:
		com_ptr<ID3D11Buffer>	m_buffer;
		UINT							m_slot;
		bool							m_fastMode;
	};
}
