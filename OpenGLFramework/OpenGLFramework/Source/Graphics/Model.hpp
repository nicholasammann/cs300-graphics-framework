/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.hpp
Purpose: Model class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#pragma once

#include <vector>
#include <string>

#include "Shader.hpp"
#include "Mesh.hpp"

namespace ELBA
{
  class Texture;
  class Application;
  class NormalMap;
  class CubeMap;

  struct Transform
  {
    Transform() : mWorldPos(), mWorldRot(), mScale(1.0f, 1.0f, 1.0f) { }

    glm::vec3 mWorldPos;
    glm::vec3 mWorldRot;
    glm::vec3 mScale;
  };

  class Model
  {
  public:

    Model(Application *aApp, const char *aPath, std::string aName, bool invertNormals = false);

    void Draw(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel);

    void SetShader(std::string aName);
    Shader* GetShader();
  
    void SetDebugShader();
    Shader* GetDebugShader();

    std::vector<Mesh*>& GetMeshes();
    
    std::string GetName() const;
    std::string GetShaderName();

    Transform &GetTransform();

    glm::mat4 ConstructModelMatrix();

    // shader selection for editor
    int mCurrentShaderSelect;
    int mPrevShaderSelect;

    Texture *mDiffuseTexture;
    Texture *mSpecularTexture;
    
    bool mUsingTextures;
    int mMappingType;
    
    NormalMap *mNormalTexture;
    bool mUsingNormalMap;

    glm::vec3 pMin;
    glm::vec3 pMax;

    void UpdateEnvironmentMap();
    CubeMap *mEnvironmentMap;
    bool mReflection;
    bool mRefraction;
    float mRefIndex;
    bool mUseEmissive;

    bool mIsSkybox;

    // skybox
    Texture *mSkyTop;
    Texture *mSkyBot;
    Texture *mSkyFro;
    Texture *mSkyBac;
    Texture *mSkyLef;
    Texture *mSkyRig;

  private:

    Application *mApp;

    // model name
    std::string mName;

    Transform mTransform;
    
    // all submeshes
    std::vector<Mesh*> mMeshes;
    
    // shader for submeshes
    Shader *mShader;

    // storing shader name for reloading
    std::string mShaderName;

    // shader for drawing debug lines
    Shader *mDebugShader;


  };

}