/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.hpp
Purpose: Mesh class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Shader.hpp"

namespace ELBA
{
  class Model;

  struct Vertex
  {
    glm::vec3 mPos;
    glm::vec3 mNormal;

    glm::vec3 mTangent;
    glm::vec3 mBitangent;

    Vertex() : mPos(), mNormal(), mTangent(), mBitangent() { }

    glm::vec3 operator-(Vertex const& v2)
    {
      glm::vec3 ret;
      ret.x = mPos.x - v2.mPos.x;
      ret.y = mPos.y - v2.mPos.y;
      ret.z = mPos.z - v2.mPos.z;
      return ret;
    }

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

    Face(unsigned int aA, unsigned int aB, unsigned int aC) 
      : a(aA), b(aB), c(aC), tangent(), bitangent()
    {
    }

    glm::vec3 tangent;
    glm::vec3 bitangent;
  };

  struct Material
  {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emissive[4];
    float shininess;

    Material() 
      : ambient{ 0.25f, 0.25f, 0.25f, 0.25f },
        diffuse{ 0.5f, 0.5f, 0.5f, 0.5f },
        shininess(1.0f)
    { }

    void SetAmbient(float r, float g, float b, float a)
    {
      ambient[0] = r;
      ambient[1] = g;
      ambient[2] = b;
      ambient[3] = a;
    }
  };


  class Mesh
  {
  public:

    Mesh(Model *aParent);

    void Draw(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel);

    void AddVertex(float aA, float aB, float aC);
    void AddFace(GLuint aA, GLuint aB, GLuint aC);

    void SetUpMesh();

    void Preprocess();

    glm::vec3 GetFaceCentroid(Face aFace);

    int* GetDebugColorMode();
    int* GetDebugLineMode();

    Material &GetMaterial();

    float& GetDebugLineWidth();
    float& GetDebugLineLength();

  private:

    Model *mParent;

    Material mMaterial;

    void DrawFaceNormals(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel);
    void DrawVertexNormals(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel);
    void DrawTangents(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel);
    void DrawBitangents(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel);

    std::vector<Vertex> mVertices;
    std::vector<Face> mFaces;
    std::vector<glm::vec3> mFaceNormals;

    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;

    void CenterMesh();
    void NormalizeVertices();

    void BindVertNorms();
    std::vector<glm::vec3> mVertNormPoints;
    unsigned int mVertDebugVAO;
    unsigned int mVertDebugVBO;

    void BindFaceNorms();
    std::vector<glm::vec3> mFaceNormPoints;
    unsigned int mFaceDebugVAO;
    unsigned int mFaceDebugVBO;

    void BindTangents();
    std::vector <glm::vec3> mTangentPoints;
    unsigned int mTangentDebugVAO;
    unsigned int mTangentDebugVBO;

    void BindBitangents();
    std::vector <glm::vec3> mBitangentPoints;
    unsigned int mBitangentDebugVAO;
    unsigned int mBitangentDebugVBO;

    float mDebugLineWidth;
    float mDebugLineLength;

    glm::vec2 GetUVs(Vertex const& aVert);
    void PreprocessTangentsAndBitangents();

  public:

    enum DebugLineMode
    {
      None,
      VertNormals,
      FaceNormals,
      TBNFrame
    };

  private:

    int mDebugColorMode;

    int mDebugLineMode;

    bool mDrawTangentAxes;

  };


}