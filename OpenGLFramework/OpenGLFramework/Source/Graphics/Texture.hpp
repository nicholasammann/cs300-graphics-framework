/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.hpp
Purpose: Texture class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

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

    void SetUniform(unsigned int aShaderPrg, std::string aUniform, char aSlot);

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