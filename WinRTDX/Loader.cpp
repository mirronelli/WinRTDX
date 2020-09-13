#include <pch.h>

#include "Loader.h"
#include "IO.h"

using namespace winrt;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Foundation;

Loader::Loader(com_ptr<ID3D11Device> device):
   m_device(device)
{}

com_ptr<ID3D11VertexShader> Loader::LoadVertexShader(std::wstring filename)
{
   //IAsyncAction Loader::LoadVertexShader(std::wstring filename)
   IBuffer buffer { IO::ReadFile(filename) };
   uint8_t* data = buffer.data();

   com_ptr<ID3D11VertexShader> shader{ nullptr };
   m_device->CreateVertexShader(
      data,
      buffer.Length(),
      nullptr,
      shader.put()
   );

   return shader;
}

com_ptr<ID3D11PixelShader> Loader::LoadPixelShader(std::wstring filename)
{
    return com_ptr<ID3D11PixelShader>();
}