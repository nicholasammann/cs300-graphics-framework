#pragma once

#include <string>

namespace ELBA
{
  
  class Framebuffer
  {
  public:

    Framebuffer();

    void Build(unsigned int aShaderPrg);

    void Bind();
    void Unbind();

    void Clear();

    void Destroy();

    void BindTexture(int aSlot);

    void SetTextureUniform(unsigned int aShaderPrg, std::string aUniform , int aSlot);

    void UnbindTexture();

    unsigned int mHandle;
    unsigned int mRenderBuffer;
    unsigned int mTexture;
    char mTextureSlot;


  };

}
