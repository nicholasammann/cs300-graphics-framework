#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include "../Graphics/Mesh.hpp"

namespace ELBA
{
  namespace Utils
  {
    void ParseLine(std::string line, Mesh *mesh);

    Mesh* LoadMesh(std::string const &objFile);
  }
}