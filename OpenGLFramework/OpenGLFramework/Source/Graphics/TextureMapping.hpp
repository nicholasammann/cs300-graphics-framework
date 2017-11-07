#pragma once

#include <glm/vec2.hpp>

namespace ELBA
{

  glm::vec2 planarMapping(glm::vec3 aObjectPos);
  glm::vec2 cylindricalMapping(glm::vec3 aObjectPos, glm::vec3 aObjectNorm, glm::vec3 pMin, glm::vec3 pMax);
  glm::vec2 sphericalMapping(glm::vec3 aObjectNorm);

}