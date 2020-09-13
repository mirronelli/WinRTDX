#pragma once

#include "pch.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage::Streams;

class IO
{
public:
	static IBuffer ReadFile(std::wstring filename);
};

