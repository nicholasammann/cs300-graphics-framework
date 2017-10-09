#pragma once

#include <glm/vec4.hpp>

struct Light
{
  float direction[4]; // direction the light is directed
  float ambient[4]; // ambient light cast onto objects
  float diffuse[4]; // diffuse light cast onto objects

  void SetDirection(float x, float y, float z, float w)
  {
    direction[0] = x;
    direction[1] = y;
    direction[2] = z;
    direction[3] = w;
  }

  void SetAmbient(float r, float g, float b, float a)
  {
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
    ambient[3] = a;
  }

  void SetDiffuse(float r, float g, float b, float a)
  {
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;
  }
};
