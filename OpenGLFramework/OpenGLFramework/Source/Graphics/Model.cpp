#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"
#include "../Core/Application.hpp"
#include "../Utils/MeshLoader.hpp"


namespace ELBA
{
  Model::Model(Application *aApp, const char *aPath, std::string aName) 
    : mApp(aApp), mName(aName), mTransform(), mShaderName(""), mCurrentShaderSelect(0)
  {
    mMeshes.push_back(Utils::LoadMesh(aPath, this));
    mMeshes.back()->SetUpMesh();
    SetDebugShader();
  }

  void Model::Draw(Shader *aShader, glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel)
  {
    for (unsigned int i = 0; i < mMeshes.size(); ++i)
    {
      mMeshes[i]->Draw(aShader, aProj, aView, aModel);
    }
  }

  void Model::SetShader(std::string aName)
  {
    Shader *s = mApp->GetShader(aName.c_str());

    if (s)
    {
      mShader = s;
      mShaderName = s->GetName();
    }
    else
    {
      mShader = mApp->GetShader("Simple");
      mShaderName = "Simple";
    }

    // set the currently selected shader name
    for (unsigned i = 0; i < mApp->mShaderNames.size(); ++i)
    {
      if (mShaderName == std::string(mApp->mShaderNames[i]))
      {
        mCurrentShaderSelect = i;
        mPrevShaderSelect = i;
        break;
      }
    }
  }

  Shader* Model::GetShader()
  {
    return mShader;
  }

  void Model::SetDebugShader()
  {
    Shader *s = mApp->GetShader("Debug");

    if (s)
    {
      mDebugShader = s;
    }
    else
    {
      mDebugShader = mApp->GetShader("Simple");
    }
  }

  Shader * Model::GetDebugShader()
  {
    return mDebugShader;
  }

  std::vector<Mesh*>& Model::GetMeshes()
  {
    return mMeshes;
  }

  std::string Model::GetName() const
  {
    return mName;
  }

  std::string Model::GetShaderName()
  {
    return mShaderName;
  }

  Transform & Model::GetTransform()
  {
    return mTransform;
  }
  glm::mat4 Model::ConstructModelMatrix()
  {
    glm::mat4 model;
    model = glm::scale(model,  mTransform.mScale);
    model = glm::rotate(model, mTransform.mWorldRot.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, mTransform.mWorldRot.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, mTransform.mWorldRot.z, glm::vec3(0, 0, 1));
    model = glm::translate(model, mTransform.mWorldPos);
    return model;
  }
}
