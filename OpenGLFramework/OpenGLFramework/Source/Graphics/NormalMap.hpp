#pragma once

namespace ELBA
{

  class NormalMap
  {
  public:

    NormalMap(const char *aPath);
    
    unsigned char** GetNormalMap();
    unsigned char** GetHeightMap();

    void BindNormalMapTexture(char aSlot);
    void SetNormalMapUniform(unsigned int aShaderPrg, const char* aUniform, char aSlot);
    void UnbindNormalMapTexture();

  private:

    unsigned char** mNormalMap;
    unsigned char** mHeightMap;

    int mBufferLength;
    int mWidth;
    int mHeight;
    int mChannels;

    // actual texture for normal map
    unsigned int mTexture;
    char mSlot;



  };

}
