#pragma once

#include <string>

namespace ELBA
{
  
  class Framebuffer
  {
  public:

    Framebuffer();

    void Build();

    void Bind();
    void Unbind();

    void Clear();

    void Destroy();

    void BindTexture(char aSlot);

    void SetTextureUniform(unsigned int aShaderPrg, std::string aUniform , char aSlot);

    void UnbindTexture();

    unsigned int mHandle;
    unsigned int mRenderBuffer;
    unsigned int mTexture;
    char mTextureSlot;
    unsigned char *mTextureData;


  };

}
