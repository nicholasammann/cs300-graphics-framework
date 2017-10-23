#pragma once

#include <string>
#include <GL/glew.h>


namespace ELBA
{

  class Texture
  {
  public:

    Texture(std::string aPath);

    void Bind(char aSlot);


    void SetUniform(unsigned int aShaderPrg, std::string aUniform);


    void Unbind();


  private:

    // gl texture
    unsigned int texture;
    char slot;


    // raw image
    unsigned char *rawImage;

    int width;
    int height;
    int channels;

    unsigned char* GetImage()
    {
      return rawImage;
    }
  };

}