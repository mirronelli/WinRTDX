#pragma once

#include <pch.h>
#include <ppltasks.h>

using namespace winrt;
using namespace winrt::Windows::Foundation;

class Loader
{
public:
   Loader(winrt::com_ptr<ID3D11Device> device);

   com_ptr<ID3D11VertexShader> LoadVertexShader(std::wstring filename);
   com_ptr<ID3D11PixelShader> LoadPixelShader(std::wstring filename);

private:
   winrt::com_ptr<ID3D11Device>  m_device;
};

