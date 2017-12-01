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
    for (int i = 0; i < mWidth; i++)
    {

      float r = (float)mHeightMap[indexAt(i + 1, j)] / 255.0f;
      float l = (float)mHeightMap[indexAt(i - 1, j)] / 255.0f;

      glm::vec3 s(1, 0, r - l);

      float u = (float)mHeightMap[indexAt(i, j + 1)] / 255.0f;
      float d = (float)mHeightMap[indexAt(i, j - 1)] / 255.0f;
      
      glm::vec3 t(0, 1, u - d);

      glm::vec3 n = glm::cross(s, t);

      n = glm::normalize(n);

      int ind = indexAt(i, j);

      mNormalMap[ind + 0] = 255 * 0.5f * (n.x + 1.0f);  // R
      mNormalMap[ind + 1] = 255 * 0.5f * (n.y + 1.0f);  // G
      mNormalMap[ind + 2] = 255 * 0.5f * (n.z + 1.0f);  // B
    }
  }

  std::ofstream file("normal_map.ppm");

  if (file.is_open())
  {
    file << "P3 " << mWidth << " " << mHeight << " " << 255 << '\n';

    for (int j = 0; j < mHeight; j++)
    {
      for (int i = 0; i < mWidth; i++)
      {
        int col = mNormalMap[j * mWidth + i];
        file << col << " ";

        col = mNormalMap[j * mWidth + i + 1];
        file << col << " ";

        col = mNormalMap[j * mWidth + i + 2];
        file << col << " ";
      }
      file << '\n';
    }
    file.close();
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

int ELBA::NormalMap::indexAt(int x, int y)
{
  int yRet = (y < 0) ? mHeight - y : y % mHeight;

  int xRet = (x < 0) ? mWidth - x : x % mWidth;

  return 3 * (yRet * mWidth + xRet);
}
