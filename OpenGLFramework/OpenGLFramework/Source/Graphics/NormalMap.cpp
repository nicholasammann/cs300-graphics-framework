#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <fstream>

#include "../Utils/stb_image.h"

#include "NormalMap.hpp"


ELBA::NormalMap::NormalMap(const char *aPath)
{
  // load the raw image and store as our height map
  mHeightMap = stbi_load(aPath, &mWidth, &mHeight, &mChannels, 3);

  // calculate the total length of our frame buffer
  mBufferLength = mWidth * mHeight * 3;

  // allocate a 2D array for the normal map
  mNormalMap = new unsigned char[mBufferLength];

  // iterate through the height map, calculating the normal at each position
  for (int j = 0; j < mHeight; j++)
  {
    int jWoo = j + 1;
    if (jWoo >= mHeight)
    {
      jWoo = 0;
    }

    int jSwooce = j - 1;
    if (jSwooce < 0)
    {
      jSwooce = 0;
    }

    for (int i = 0; i < mWidth - 1; i++)
    {
      int iWoo = i + 1;
      if (iWoo >= mWidth)
      {
        iWoo = 0;
      }

      int iSwooce = i - 1;
      if (iSwooce < 0)
      {
        iSwooce = mWidth - 1;
      }

      glm::vec3 s(1, 0, mHeightMap[j * mWidth * 3 + iWoo * 3] - mHeightMap[j * mWidth + iSwooce * 3]);

      glm::vec3 t(0, 1, mHeightMap[jWoo * mWidth * 3 + i * 3] - mHeightMap[jSwooce * mWidth * 3 + i * 3]);
      
      glm::vec3 n = glm::cross(s, t);
      
      glm::normalize(n);
      
      //         height            width
      int ind = (j * mWidth * 3) + (i * 3);

      mNormalMap[ind] = 255 * 0.5f * (s.x + 1);      // R
      mNormalMap[ind + 1] = 255 * 0.5f * (s.y + 1);  // G
      mNormalMap[ind + 2] = 255 * 0.5f * (s.z + 1);  // B
    }
  }

  glGenTextures(1, &mTexture);

  glBindTexture(GL_TEXTURE_2D, mTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mNormalMap);

  // unbind the texture
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned char* ELBA::NormalMap::GetNormalMap()
{
  return mNormalMap;
}

unsigned char* ELBA::NormalMap::GetHeightMap()
{
  return mHeightMap;
}

void ELBA::NormalMap::BindNormalMapTexture(char aSlot)
{
  glActiveTexture(GL_TEXTURE0 + aSlot);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  mSlot = aSlot;
}

void ELBA::NormalMap::SetNormalMapUniform(unsigned int aShaderPrg, const char* aUniform, char aSlot)
{
  int loc = glGetUniformLocation(aShaderPrg, aUniform);
  glUniform1i(loc, GL_TEXTURE0 + aSlot);
}

void ELBA::NormalMap::UnbindNormalMapTexture()
{
  glActiveTexture(GL_TEXTURE0 + mSlot);
  glBindTexture(GL_TEXTURE_2D, 0);
}
