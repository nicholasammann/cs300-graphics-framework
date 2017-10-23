#include "Texture.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "../Utils/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

namespace ELBA
{

  Texture::Texture(std::string aPath) : width(0), height(0), channels(0)
  {
    stbi_load(aPath.c_str(), &width, &height, &channels, 3);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, rawImage);

    // unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
  }


  void Texture::Bind(char aSlot)
  {
    glActiveTexture(GL_TEXTURE0 + aSlot);
    glBindTexture(GL_TEXTURE_2D, texture);
    slot = aSlot;
  }


  void Texture::SetUniform(unsigned int aShaderPrg, std::string aUniform)
  {
    int loc = glGetUniformLocation(aShaderPrg, aUniform.c_str());
    glUniform1i(loc, slot);
  }


  void Texture::Unbind()
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

}