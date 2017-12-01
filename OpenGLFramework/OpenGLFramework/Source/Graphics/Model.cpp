/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.cpp
Purpose: Model class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/euler_angles.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CubeMap.hpp"
#include "Model.hpp"
#include "../Core/Application.hpp"
#include "../Utils/MeshLoader.hpp"
#include "Texture.hpp"
#include "NormalMap.hpp"


namespace ELBA
{
  Model::Model(Application *aApp, const char *aPath, std::string aName)
    : mApp(aApp), mName(aName), mTransform(), mShaderName(""),
      mCurrentShaderSelect(0), mDiffuseTexture(nullptr),
      mSpecularTexture(nullptr), mUsingTextures(0), mMappingType(0),
      mNormalTexture(nullptr), mUsingNormalMap(true),
      mUsingEnvironmentMap(true)
  {
    mMeshes.push_back(Utils::LoadMesh(aPath, this));
    mMeshes.back()->SetUpMesh();
    SetDebugShader();
    mEnvironmentMap = new CubeMap(mApp);
  }

  void Model::Draw(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel)
  {
    auto prg = mShader->GetShaderProgram();

    int loc = glGetUniformLocation(prg, "MappingType");
    glUniform1i(loc, mMappingType);

    // set use textures to "false" so we don't need to bind other uniforms
    loc = glGetUniformLocation(mShader->GetShaderProgram(), "UseTextures");
    glUniform1i(loc, mUsingTextures);

    // bind uniforms for texture
    if (mUsingTextures)
    {
      if (mDiffuseTexture && mSpecularTexture)
      {
        mDiffuseTexture->SetUniform(prg, "diffuseTexture", 0);
        mDiffuseTexture->Bind(0);

        mSpecularTexture->SetUniform(prg, "specularTexture", 1);
        mSpecularTexture->Bind(1);

        loc = glGetUniformLocation(prg, "pMin");
        glUniform3fv(loc, 1, &pMin[0]);

        loc = glGetUniformLocation(prg, "pMax");
        glUniform3fv(loc, 1, &pMax[0]);
      }
      else
      {
        // set use textures to "false" so we don't need to bind other uniforms
        loc = glGetUniformLocation(mShader->GetShaderProgram(), "UseTextures");
        glUniform1i(loc, 0);
      }
    }

    // set use textures to "true"
    loc = glGetUniformLocation(prg, "UseNormalMap");
    glUniform1i(loc, mUsingNormalMap);

    if (mUsingNormalMap)
    {
      if (mNormalTexture)
      {
        mNormalTexture->SetNormalMapUniform(prg, "normalTexture", 2);
        mNormalTexture->BindNormalMapTexture(2);
      }
      else
      {
        loc = glGetUniformLocation(prg, "UseNormalMap");
        glUniform1i(loc, 0);
      }
    }

    loc = glGetUniformLocation(prg, "UseEnvironmentMap");
    glUniform1i(loc, mUsingEnvironmentMap);

    if (mUsingEnvironmentMap)
    {
      mEnvironmentMap->SetTextureUniforms(prg);
    }


    for (unsigned int i = 0; i < mMeshes.size(); ++i)
    {
      mMeshes[i]->Draw(aProj, aView, aModel);
    }

    if (mDiffuseTexture && mSpecularTexture)
    {
      mDiffuseTexture->Unbind();
      mSpecularTexture->Unbind();
    }

    if (mNormalTexture)
    {
      mNormalTexture->UnbindNormalMapTexture();
    }

    if (mUsingEnvironmentMap)
    {
      mEnvironmentMap->UnbindTextures();
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

  Shader* Model::GetDebugShader()
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

  Transform& Model::GetTransform()
  {
    return mTransform;
  }

  glm::mat4 Model::ConstructModelMatrix()
  {
    glm::mat4 scale = glm::scale(glm::mat4(), mTransform.mScale);
    
    glm::mat4 rot = glm::yawPitchRoll(mTransform.mWorldRot.y, mTransform.mWorldRot.x, mTransform.mWorldRot.z);

    glm::mat4 trans = glm::translate(glm::mat4(), mTransform.mWorldPos);
    
    return  trans * rot * scale;
  }

  void Model::UpdateEnvironmentMap()
  {
    mEnvironmentMap->UpdateTextures(mTransform.mWorldPos);
  }

}
