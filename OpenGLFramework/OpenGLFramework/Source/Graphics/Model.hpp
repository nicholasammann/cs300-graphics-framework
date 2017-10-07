#pragma once

#include <vector>
#include <string>

#include "Shader.hpp"
#include "Mesh.hpp"

namespace ELBA
{

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

    Model(char *aPath, std::string aName);

    void Draw(Shader *aShader);

    std::vector<Mesh*>& GetMeshes();
    
    std::string GetName() const;

    Transform &GetTransform();

  private:

    Transform mTransform;

    std::string mName;

    std::vector<Mesh*> mMeshes;

  };

}