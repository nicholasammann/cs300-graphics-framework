/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MeshLoader.hpp
Purpose: MeshLoader function definitions
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include "../Graphics/Mesh.hpp"

namespace ELBA
{
  class Model;

  namespace Utils
  {
    void ParseLine(std::string line, Mesh *mesh);

    Mesh* LoadMesh(std::string const &objFile, Model *aParent, bool invertNormals);
  }
}