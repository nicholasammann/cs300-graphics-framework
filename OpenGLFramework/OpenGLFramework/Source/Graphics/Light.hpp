#pragma once

#include <glm/vec4.hpp>

struct Light
{
  glm::vec4 direction; // direction the light is directed
  glm::vec4 ambient; // ambient light cast onto objects
  glm::vec4 diffuse; // diffuse light cast onto objects
};
