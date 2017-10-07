#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "Mesh.hpp"


namespace ELBA
{

  Mesh::Mesh() : mDebugMode(0)
  {
  }

  void Mesh::Draw(Shader *aShader)
  {
    switch (mDebugMode)
    {
    case VertNormals:
    {
      DrawVertexNormals();
      break;
    }

    case FaceNormals:
    {
      DrawFaceNormals();
      break;
    }
    }

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mFaces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  void Mesh::DrawFaceNormals()
  {
    for (unsigned int i = 0; i < mFaces.size(); ++i)
    {
      glm::vec3 norm = mFaceNormals[i];
      glm::vec3 a = GetFaceCentroid(mFaces[i]);
      glm::vec3 b = a + 5.0f * norm;

      glLineWidth(1.5);
      glColor3f(0, 0, 1.0);
      glBegin(GL_LINES);
      glVertex3f(a.x, a.y, a.z);
      glVertex3f(b.x, b.y, b.z);
      glEnd();
    }
  }

  void Mesh::DrawVertexNormals()
  {
    
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

    glBindVertexArray(0);
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

  void Mesh::BindVertNormals()
  {
    std::vector<float> mPoints;

    for (unsigned i = 0; i < mVertices.size(); ++i)
    {
      Vertex v = mVertices[i];
      glm::vec3 a = v.mPos;
      glm::vec3 b = a + v.mNormal;

      // store the beginning of the line
      mPoints.push_back(a.x);
      mPoints.push_back(a.y);
      mPoints.push_back(a.z);

      // store the end of the line
      mPoints.push_back(b.x);
      mPoints.push_back(b.y);
      mPoints.push_back(b.z);
    }

    //// Vertex Array Object ////
    // create and bind vertex array object
    glGenVertexArrays(1, &mVertNorm_VAO);
    glBindVertexArray(mVertNorm_VAO);
    //////////////////////////////

    //// Vertex Buffer Object ////
    // create and bind empty vertex buffer object
    glGenBuffers(1, &mVertNorm_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVertNorm_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mPoints), mPoints.data(), GL_STATIC_DRAW);
    //////////////////////////////

  }

  void Mesh::BindFaceNormals()
  {

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

    glm::vec3 extent = maximum - minimum;

    float scalar = 1.f / std::min(std::min(extent.x, extent.y), extent.z);

    for (auto &vert : mVertices)
    {
      vert.mPos *= scalar;
    }
  }

}
