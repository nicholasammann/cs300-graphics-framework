#pragma once

#include <vector>
#include <string>

#include "Shader.hpp"
#include "Mesh.hpp"

namespace ELBA
{
  class Texture;
  class Application;

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

    Model(Application *aApp, const char *aPath, std::string aName);

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

    glm::vec3 pMin;
    glm::vec3 pMax;

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