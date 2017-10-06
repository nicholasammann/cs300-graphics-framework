#pragma once

#include <vector>

#include "Shader.hpp"
#include "Mesh.hpp"

namespace ELBA
{

  class Model
  {
  public:

    Model(char *aPath);

    void Draw(Shader *aShader);

  private:

    std::vector<Mesh> mMeshes;

    std::vector<Texture> mLoadedTextures;

    std::string mDirectory;

    void LoadModel(std::string aPath);

    unsigned int LoadTexture(const char *aFile, std::string aDir);

  };

}