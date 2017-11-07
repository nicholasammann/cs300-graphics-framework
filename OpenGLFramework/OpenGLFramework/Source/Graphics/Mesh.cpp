/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: Mesh class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "../Utils/MathUtils.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "TextureMapping.hpp"
#include "../Core/Application.hpp"

namespace ELBA
{

  Mesh::Mesh(Model *aParent)
    : mParent(aParent), mDebugMode(0), mDebugLineWidth(1.5f), mDebugLineLength(0.5f),
    mVertices(), mFaces(), mFaceNormals()
  {
  }

  void Mesh::Draw(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel)
  {
    switch (mDebugMode)
    {
    case VertNormals:
    {
      DrawVertexNormals(aProj, aView, aModel);
      break;
    }

    case FaceNormals:
    {
      DrawFaceNormals(aProj, aView, aModel);
      break;
    }
    }

    mParent->GetShader()->UseShaderProgram();

    unsigned int shdrPrg = mParent->GetShader()->GetShaderProgram();

    unsigned int matLoc = glGetUniformLocation(shdrPrg, "Material.ambient");
    glUniform4fv(matLoc, 1, mMaterial.ambient);

    matLoc = glGetUniformLocation(shdrPrg, "Material.diffuse");
    glUniform4fv(matLoc, 1, mMaterial.diffuse);

    matLoc = glGetUniformLocation(shdrPrg, "Material.specular");
    glUniform4fv(matLoc, 1, mMaterial.specular);

    matLoc = glGetUniformLocation(shdrPrg, "Material.emissive");
    glUniform4fv(matLoc, 1, mMaterial.emissive);

    matLoc = glGetUniformLocation(shdrPrg, "Material.shininess");
    glUniform1f(matLoc, mMaterial.shininess);


    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mFaces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  void Mesh::DrawFaceNormals(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel)
  {
    mFaceNormPoints.clear();

    for (unsigned int i = 0; i < mFaces.size(); ++i)
    {
      glm::vec3 norm = mFaceNormals[i];
      glm::vec3 a = GetFaceCentroid(mFaces[i]);
      glm::vec3 b = a + mDebugLineLength * norm;

      mFaceNormPoints.push_back(a);
      mFaceNormPoints.push_back(b);
    }

    unsigned int shdrPrg = mParent->GetDebugShader()->GetShaderProgram();
    mParent->GetDebugShader()->UseShaderProgram();

    unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(aProj));

    unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(aView));

    unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(aModel));

    glBindVertexArray(mFaceDebugVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mFaceDebugVBO);
    glBufferData(GL_ARRAY_BUFFER, mFaceNormals.size() * sizeof(glm::vec3) * 2, mFaceNormPoints.data(), GL_DYNAMIC_DRAW);

    glLineWidth(mDebugLineWidth);
    glDrawArrays(GL_LINES, 0, mFaceNormPoints.size() * 3);
    glBindVertexArray(0);
  }

  void Mesh::DrawVertexNormals(glm::mat4 &aProj, glm::mat4 &aView, glm::mat4 &aModel)
  {
    mVertNormPoints.clear();

    for (unsigned int i = 0; i < mVertices.size(); ++i)
    {
      glm::vec3 norm = mVertices[i].mNormal;
      glm::vec3 a = mVertices[i].mPos;
      glm::vec3 b = a + mDebugLineLength * norm;

      mVertNormPoints.push_back(a);
      mVertNormPoints.push_back(b);
    }

    unsigned int shdrPrg = mParent->GetDebugShader()->GetShaderProgram();
    mParent->GetDebugShader()->UseShaderProgram();

    unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(aProj));

    unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(aView));

    unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(aModel));

    glBindVertexArray(mVertDebugVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVertDebugVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3) * 2, mVertNormPoints.data(), GL_DYNAMIC_DRAW);

    glLineWidth(mDebugLineWidth);
    glDrawArrays(GL_LINES, 0, mVertNormPoints.size() * 3);
    glBindVertexArray(0);
  }

  void Mesh::AddVertex(float aX, float aY, float aZ)
  {
    Vertex v;
    v.mPos.x = aX;
    v.mPos.y = aY;
    v.mPos.z = aZ;

    mVertices.push_back(v);
  }

  void Mesh::AddFace(GLuint aA, GLuint aB, GLuint aC)
  {
    mFaces.push_back(Face(aA, aB, aC));
  }

  void Mesh::SetUpMesh()
  {
    //// Vertex Array Object ////
    // create and bind vertex array object
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    //////////////////////////////

    //// Vertex Buffer Object ////
    // create and bind empty vertex buffer object
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);
    //////////////////////////////

    unsigned int *indices = new unsigned int[mFaces.size() * 3];

    for (unsigned int i = 0; i < mFaces.size(); ++i)
    {
      for (unsigned int j = 0; j < 3; ++j)
      {
        indices[3 * i + j] = mFaces[i].mIndices[j];
      }
    }

    //// Element Buffer Object ////
    // create and bind empty element buffer object
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mFaces.size() * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    ///////////////////////////////

    // tell OpenGL how it should interpret all vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTangent));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mBitangent));

    glBindVertexArray(0);

    BindVertNorms();
    BindFaceNorms();
  }

  void Mesh::Preprocess()
  {
    std::unordered_map<glm::vec3*, std::vector<glm::vec3>> adjNormals;

    // compute vertex normals
    for (unsigned int i = 0; i < mFaces.size(); ++i)
    {
      Face &face = mFaces[i];

      glm::vec3 v0 = mVertices[face.mIndices[0]].mPos;
      glm::vec3 v1 = mVertices[face.mIndices[1]].mPos;
      glm::vec3 v2 = mVertices[face.mIndices[2]].mPos;

      glm::vec3 normRaw = glm::cross(v1 - v0, v2 - v1);

      glm::vec3 norm = glm::normalize(normRaw);

      mFaceNormals.push_back(norm);

      for (int j = 0; j < 3; ++j)
      {
        mVertices[face.mIndices[j]].mNormal = glm::vec3(0, 0, 0);

        std::vector<glm::vec3> &verts = adjNormals[&mVertices[face.mIndices[j]].mNormal];

        auto it = std::find(verts.begin(), verts.end(), norm);

        if (it == verts.end())
        {
          glm::vec3 n = mFaceNormals[i];
          verts.push_back(n);
        }
      }
    }

    for (auto it : adjNormals)
    {
      for (auto norm : it.second)
      {
        *it.first += norm;
      }
    }

    for (unsigned int i = 0; i < mVertices.size(); ++i)
    {
      mVertices[i].mNormal = glm::normalize(mVertices[i].mNormal);
    }

    CenterMesh();
    NormalizeVertices();

    PreprocessTangentsAndBitangents();
  }

  glm::vec3 Mesh::GetFaceCentroid(Face aFace)
  {
    glm::vec3 centroid = mVertices[aFace.a].mPos;
    centroid += mVertices[aFace.b].mPos;
    centroid += mVertices[aFace.c].mPos;
    return centroid * (1.f / 3.f);
  }

  int* Mesh::GetDebugMode()
  {
    return &mDebugMode;
  }

  Material & Mesh::GetMaterial()
  {
    return mMaterial;
  }

  void Mesh::CenterMesh()
  {
    glm::vec3 centroid(0.f);

    for (auto &vert : mVertices)
    {
      centroid += vert.mPos;
    }

    centroid *= 1.f / static_cast<float>(mVertices.size());

    centroid = -centroid;

    for (auto &vert : mVertices)
    {
      vert.mPos += centroid;
    }
  }

  void Mesh::NormalizeVertices()
  {
    glm::vec3 minimum = mVertices[0].mPos;
    glm::vec3 maximum = mVertices[0].mPos;

    for (auto &vert : mVertices)
    {
      float x = vert.mPos.x;
      float y = vert.mPos.y;
      float z = vert.mPos.z;

      minimum.x = std::min(minimum.x, x);
      minimum.y = std::min(minimum.y, y);
      minimum.z = std::min(minimum.z, z);
      maximum.x = std::max(maximum.x, x);
      maximum.y = std::max(maximum.y, y);
      maximum.z = std::max(maximum.z, z);
    }

    mParent->pMin = minimum;
    mParent->pMax = maximum;

    glm::vec3 extent = maximum - minimum;

    float scalar = 1.f / std::min(std::min(extent.x, extent.y), extent.z);

    if (extent.x == 0)
    {
      scalar = 1.f / std::min(extent.y, extent.z);
    }
    else if (extent.y == 0)
    {
      scalar = 1.f / std::min(extent.x, extent.z);
    }
    else if (extent.z == 0)
    {
      scalar = 1.f / std::min(extent.x, extent.y);
    }

    for (auto &vert : mVertices)
    {
      vert.mPos *= scalar;
    }
  }

  void Mesh::BindVertNorms()
  {
    //// Vertex Array Object ////
    // create and bind vertex array object
    glGenVertexArrays(1, &mVertDebugVAO);
    glBindVertexArray(mVertDebugVAO);
    //////////////////////////////

    //// Vertex Buffer Object ////
    // create and bind empty vertex buffer object
    glGenBuffers(1, &mVertDebugVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVertDebugVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3) * 2, mVertNormPoints.data(), GL_STATIC_DRAW);
    //////////////////////////////

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
  }

  void Mesh::BindFaceNorms()
  {
    //// Vertex Array Object ////
    // create and bind vertex array object
    glGenVertexArrays(1, &mFaceDebugVAO);
    glBindVertexArray(mFaceDebugVAO);
    //////////////////////////////

    //// Vertex Buffer Object ////
    // create and bind empty vertex buffer object
    glGenBuffers(1, &mFaceDebugVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mFaceDebugVBO);
    glBufferData(GL_ARRAY_BUFFER, mFaceNormals.size() * sizeof(glm::vec3) * 2, mFaceNormPoints.data(), GL_STATIC_DRAW);
    //////////////////////////////

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
  }

  glm::vec2 Mesh::GetUVs(Vertex const& aVert)
  {
    switch (mParent->mMappingType)
    {
      // planar
    case 0:
    {
      return planarMapping(aVert.mPos);
    }

    // cylindrical
    case 1:
    {
      return cylindricalMapping(aVert.mPos, aVert.mPos, mParent->pMin, mParent->pMax);
    }

    // spherical
    case 2:
    {
      return sphericalMapping(aVert.mNormal);
    }
    }
  }

  void Mesh::PreprocessTangentsAndBitangents()
  {
    std::unordered_map<Vertex*, std::vector<glm::vec3>> adjTangents;
    std::unordered_map<Vertex*, std::vector<glm::vec3>> adjBitangents;

    // for each face on the mesh
    for (Face& f : mFaces)
    {
      /* calculate its tangent and bitangent vectors */

      // get the 3 vertices (E,F,G) of the face
      Vertex E = mVertices[f.a];
      Vertex F = mVertices[f.b];
      Vertex G = mVertices[f.c];

      // get the texture coords for each vertex
      glm::vec2 eTexCoords = GetUVs(E);
      glm::vec2 fTexCoords = GetUVs(F);
      glm::vec2 gTexCoords = GetUVs(G);

      // find the vectors P=F-E and Q=G-E
      glm::vec3 P = F - E;
      glm::vec3 Q = G - E;
      
      // find the tex coords for the vectors
      glm::vec2 sP = glm::vec2(fTexCoords.x - eTexCoords.x, fTexCoords.x - eTexCoords.y);
      glm::vec2 sQ = glm::vec2(gTexCoords.x - eTexCoords.x, gTexCoords.x - eTexCoords.y);

      glm::vec3 T;
      glm::vec3 B;

      /* solve the system of equations to get T and B */
      // get T.x and B.x
      auto matrix = MakeMatrix(sP.x, sP.y, P.x,
                               sQ.x, sQ.y, Q.x);

      auto sol = GaussianElimination(matrix);

      T.x = sol[0];
      B.x = sol[1];
      
      // get T.y and B.y
      matrix = MakeMatrix(sP.x, sP.y, P.y,
                          sQ.x, sQ.y, Q.y);

      sol = GaussianElimination(matrix);

      T.y = sol[0];
      B.y = sol[1];

      // get T.z and B.z
      matrix = MakeMatrix(sP.x, sP.y, P.z,
                          sQ.x, sQ.y, Q.z);

      sol = GaussianElimination(matrix);

      T.z = sol[0];
      B.z = sol[1];


      // store the calculated vectors on the face
      f.tangent = T;
      f.bitangent = B;

      for (int i = 0; i < 3; ++i)
      {
        mVertices[f.mIndices[i]].mTangent = glm::vec3();
        std::vector<glm::vec3> tangs = adjTangents[&mVertices[f.mIndices[i]]];

        auto it = std::find(tangs.begin(), tangs.end(), f.tangent);

        if (it == tangs.end())
        {
          tangs.push_back(f.tangent);
        }


        mVertices[f.mIndices[i]].mBitangent = glm::vec3();
        std::vector<glm::vec3> bitangs = adjBitangents[&mVertices[f.mIndices[i]]];

        auto it_2 = std::find(bitangs.begin(), bitangs.end(), f.bitangent);

        if (it_2 == bitangs.end())
        {
          bitangs.push_back(f.bitangent);
        }
      }
    }


    for (auto pair : adjTangents)
    {
      for (auto tangent : pair.second)
      {
        pair.first->mTangent += tangent;
      }
    }

    for (auto pair : adjBitangents)
    {
      for (auto bitangent : pair.second)
      {
        pair.first->mBitangent += bitangent;
      }
    }

    for (Vertex &v : mVertices)
    {
      v.mTangent = glm::normalize(v.mTangent);
      v.mBitangent = glm::normalize(v.mBitangent);
    }
  }

  float & Mesh::GetDebugLineWidth()
  {
    return mDebugLineWidth;
  }

  float & Mesh::GetDebugLineLength()
  {
    return mDebugLineLength;
  }

}
