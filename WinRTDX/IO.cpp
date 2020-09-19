#include "pch.h"
#include "IO.h"
#include <codecvt>

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;

IBuffer IO::ReadFile(std::wstring filename)
{
   StorageFolder storageFolder = Windows::ApplicationModel::Package::Current().InstalledLocation();
   StorageFile file { storageFolder.GetFileAsync(filename).get() };
   IBuffer buffer { FileIO::ReadBufferAsync(file).get() };
   return buffer;
}
