#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/euler_angles.hpp>
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

  void Model::Draw(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel)
  {
    for (unsigned int i = 0; i < mMeshes.size(); ++i)
    {
      mMeshes[i]->Draw(aProj, aView, aModel);
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
    for (int i = 0; i < mApp->mShaderNames.size(); ++i)
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
    glm::mat4 scale = glm::scale(glm::mat4(),  mTransform.mScale);
    
    glm::mat4 rot = glm::yawPitchRoll(mTransform.mWorldRot[1], mTransform.mWorldRot[0], mTransform.mWorldRot[2]);

    glm::mat4 trans = glm::translate(glm::mat4(), mTransform.mWorldPos);
    
    return  trans *rot * scale;
  }
}
