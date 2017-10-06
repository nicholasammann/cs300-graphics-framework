#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"


namespace ELBA
{
  Model::Model(char * aPath)
  {
    LoadModel(aPath);
  }

  void Model::Draw(Shader *aShader)
  {
    for (unsigned int i = 0; i < mMeshes.size(); ++i)
    {
      mMeshes[i].Draw(aShader);
    }
  }

  void Model::LoadModel(std::string aPath)
  {
    

  }

  

  unsigned int Model::LoadTexture(const char * aFile, std::string aDir)
  {
    return 0;
  }
}


