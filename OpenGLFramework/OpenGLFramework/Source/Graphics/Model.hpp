#pragma once

#include <vector>
#include <string>

#include "Shader.hpp"
#include "Mesh.hpp"

namespace ELBA
{
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

    Model(Application *aApp, char *aPath, std::string aName);

    void Draw(Shader *aShader);

    void SetShader(std::string aName);

    std::vector<Mesh*>& GetMeshes();
    
    std::string GetName() const;

    Transform &GetTransform();

  private:

    Application *mApp;

    Transform mTransform;

    std::string mName;

    std::vector<Mesh*> mMeshes;

    std::string mShaderName;

  };

}