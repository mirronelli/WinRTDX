#pragma once
#include "Attachable.h"

namespace Dx::Attachables
{

   class Sampler : public Attachable
   {
   public:
      static std::shared_ptr<Sampler> Create(
         std::string name, 
         D3D11_FILTER filter, 
         D3D11_TEXTURE_ADDRESS_MODE mode, 
         unsigned int slot
      );
      
      Sampler(
         std::string key, 
         D3D11_FILTER filter, 
         D3D11_TEXTURE_ADDRESS_MODE mode, 
         unsigned int slot
      );
      
      void Attach();
   private:
      com_ptr<ID3D11SamplerState>				mSampler;
      unsigned int mSlot;
      std::string mKey;
      inline static std::map < std::string, std::shared_ptr<Sampler>> mMap;
   };

}
