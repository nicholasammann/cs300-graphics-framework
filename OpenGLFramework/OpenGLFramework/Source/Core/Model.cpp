#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "../Utils/MeshLoader.hpp"


namespace ELBA
{
  Model::Model(char * aPath)
  {
    mMeshes.push_back(Utils::LoadMesh(aPath));
  }

  void Model::Draw(Shader *aShader)
  {
    for (unsigned int i = 0; i < mMeshes.size(); ++i)
    {
      mMeshes[i]->Draw(aShader);
    }
  }

  
  unsigned int Model::LoadTexture(const char * aFile, std::string aDir)
  {
    return 0;
  }
}
