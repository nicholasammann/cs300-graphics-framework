#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "../Core/Application.hpp"
#include "../Utils/MeshLoader.hpp"


namespace ELBA
{
  Model::Model(Application *aApp, char * aPath, std::string aName) : mApp(aApp), mName(aName), mTransform()
  {
    mMeshes.push_back(Utils::LoadMesh(aPath));
    mMeshes.back()->SetUpMesh();
  }

  void Model::Draw(Shader *aShader)
  {
    for (unsigned int i = 0; i < mMeshes.size(); ++i)
    {
      mMeshes[i]->Draw(aShader);
    }
  }

  void Model::SetShader(std::string aName)
  {
    mShaderName = aName;

  }

  std::vector<Mesh*>& Model::GetMeshes()
  {
    return mMeshes;
  }

  std::string Model::GetName() const
  {
    return mName;
  }

  Transform & Model::GetTransform()
  {
    return mTransform;
  }
}
