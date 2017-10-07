#pragma once

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Shader.hpp"

namespace ELBA
{
  class Model;

  struct Vertex
  {
    glm::vec3 mPos;
    glm::vec3 mNormal;

    Vertex() : mPos(glm::vec3()), mNormal(glm::vec3()) { }
  };

  struct Face
  {
    union
    {
      struct
      {
        unsigned int a, b, c;
      };
      unsigned int mIndices[3];
    };

    Face(unsigned int aA, unsigned int aB, unsigned int aC) : a(aA), b(aB), c(aC) { }

  };

  struct Texture
  {
    unsigned int mID;
    std::string mType;
  };

  class Mesh
  {
  public:

    Mesh(Model *aParent);

    void Draw(Shader *aShader);

    void AddVertex(float aA, float aB, float aC);
    void AddFace(GLuint aA, GLuint aB, GLuint aC);

    void SetUpMesh();

    void Preprocess();

    glm::vec3 GetFaceCentroid(Face aFace);

    int* GetDebugMode();

  private:

    Model *mParent;

    void DrawFaceNormals();
    void DrawVertexNormals();

    std::vector<Vertex> mVertices;
    std::vector<Face> mFaces;
    std::vector<glm::vec3> mFaceNormals;

    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;

    void BindVertNormals();
    unsigned int mVertNorm_VAO;
    unsigned int mVertNorm_VBO;

    void BindFaceNormals();
    unsigned int mFaceNorm_VAO;
    unsigned int mFaceNorm_VBO;

    void CenterMesh();
    void NormalizeVertices();

  public:

    enum DebugMode
    {
      None,
      VertNormals,
      FaceNormals
    };

  private:

    int mDebugMode;

  };


}