#include <filesystem>
#include <fstream>

#include "MeshLoader.hpp"
#include "../Graphics/Model.hpp"
#include "../Graphics/Mesh.hpp"

namespace ELBA
{
  namespace Utils
  {
    void ParseLine(std::string line, Mesh *mesh)
    {
      // skip empty lines
      if (line.empty())
      {
        return;
      }

      // skip comments
      if (line[0] == '#')
      {
        return;
      }

      int pos = line.find(' ');
      std::string token = line.substr(0, pos);

      // polygonal face elements
      if (token == "f")
      {
        std::stringstream stream(line.substr(pos, line.length() - 1));
        std::vector<unsigned int> face;
        unsigned int num;

        while (stream >> num)
        {
          face.push_back(num);
        }

        mesh->AddFace(face[0] - 1, face[1] - 1, face[2] - 1);
      }
      // vertices
      else if (token == "v")
      {
        std::stringstream stream(line.substr(pos, line.length() - 1));
        std::vector<float> vertex;
        float num;

        while (stream >> num)
        {
          vertex.push_back(num);
        }

        mesh->AddVertex(vertex[0], vertex[1], vertex[2]);
      }
    }


    Mesh* LoadMesh(std::string const &objFile, Model *aParent)
    {
      std::ifstream input = std::ifstream(objFile);
      
      assert(input.good());
      
      if (!input.good())
        return nullptr;

      Mesh *mesh = new Mesh(aParent);

      std::string line;
      while (getline(input, line))
      {
        ParseLine(line, mesh);
      }

      mesh->Preprocess();

      return mesh;

    }
  }

}