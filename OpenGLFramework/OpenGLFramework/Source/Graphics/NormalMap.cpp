#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtx/matrix_cross_product.hpp>

#include "../Utils/stb_image.h"

#include "NormalMap.hpp"


ELBA::NormalMap::NormalMap(const char *aPath)
{
  // load the raw image and store as our height map
  unsigned char *tex = stbi_load(aPath, &mWidth, &mHeight, &mChannels, 3);


  // allocate a 2D array for the height map
  mHeightMap = new unsigned char*[mHeight];
  for (int i = 0; i < mHeight; ++i)
  {
    mHeightMap[i] = new unsigned char[mWidth];
  }

  // copy the R values into our height map
  for (int i = 0; i < mHeight; ++i)
  {
    for (int j = 0; j < mWidth; ++j)
    {
      int rowInd = i * mWidth * 3;

      int colInd = j * 3;

      mHeightMap[i][j] = tex[rowInd + colInd];
    }
  }

  // calculate the total length of our frame buffer
  mBufferLength = mWidth * mHeight * 3;

  // allocate a 2D array for the normal map
  mNormalMap = new unsigned char*[mHeight];
  for (int i = 0; i < mHeight; ++i)
  {
    mNormalMap[i] = new unsigned char[mWidth * 3];
  }

  // iterate through the height map, calculating the normal at each position
  for (int j = 0; j < mHeight; j++)
  {
    for (int i = 0; i < mWidth - 1; i++)
    {
      glm::vec3 s(1, 0, mHeightMap[j][i + 1] - mHeightMap[j][i - 1]);

      glm::vec3 t(0, 1, mHeightMap[j + 1][i] - mHeightMap[j - 1][i]);
      
      glm::vec3 n = glm::cross(s, t);
      
      glm::normalize(n);
      
      int ind = 3 * i;

      mNormalMap[j][ind] = 255 * 0.5f * (s.x + 1);      // R
      mNormalMap[j][ind + 1] = 255 * 0.5f * (s.y + 1);  // G
      mNormalMap[j][ind + 2] = 255 * 0.5f * (s.z + 1);  // B
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

unsigned char** ELBA::NormalMap::GetNormalMap()
{
  return mNormalMap;
}

unsigned char** ELBA::NormalMap::GetHeightMap()
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
