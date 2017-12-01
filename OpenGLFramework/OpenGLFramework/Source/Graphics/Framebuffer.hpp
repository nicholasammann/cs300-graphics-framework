#pragma once

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


    unsigned int mHandle;
    unsigned int mRenderBuffer;
    unsigned int mTexture;
    unsigned char *mTextureData;

  };

}
