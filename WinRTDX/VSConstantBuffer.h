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
		static std::shared_ptr<VSConstantBuffer<T>> Create(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, T& constantData)
		{
			VSConstantBuffer<T>* instancePtr = (VSConstantBuffer<T>*) Dx::ResourceManager::GetResource(TypeIndex, key);
			std::shared_ptr<VSConstantBuffer<T>> instance = std::shared_ptr<VSConstantBuffer<T>>(instancePtr);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<VSConstantBuffer>(key, graphics, constantData);
				Dx::ResourceManager::SetResource(TypeIndex, key, instance.get());
			}

			return instance;
		}

		VSConstantBuffer(uint16_t key, std::shared_ptr<Graphics> graphics, T& constantData)
			: Attachable(key, graphics)
		{
			VSConstantBuffer<T>::TypeIndex = std::type_index(typeid(VSConstantBuffer<T>));

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
			if (force || Dx::ResourceManager::GetCurrentInstance(TypeIndex) != m_key)
			{
				ID3D11Buffer* VSConstantBuffers[1] = { m_buffer.get() };
				m_context->VSSetConstantBuffers(0, 1, VSConstantBuffers);
				Dx::ResourceManager::SetCurrentInstance(TypeIndex, m_key);
			}
		}

		void Update(T const& constantData)
		{
			m_context->UpdateSubresource(m_buffer.get(), 0, 0, &constantData, 0, 0);
		}

	private:
		com_ptr<ID3D11Buffer> m_buffer;

		inline static std::type_index TypeIndex = std::type_index(typeid(std::string)); // dummy value, is overwritten at runtime
	};
}
