#pragma once

#include <glm/vec4.hpp>

using namespace glm;

struct DirLight
{
  float direction[4];
  float ambient[4];
  float diffuse[4];
  float specular[4];

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

  void SetSpecular(float r, float g, float b, float a)
  {
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
  }
};


struct SpotLight
{
  float pos[4];
  float direction[4];
  float ambient[4];
  float diffuse[4];
  float specular[4];

  void SetPos(float x, float y, float z, float w)
  {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = w;
  }

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

  void SetSpecular(float r, float g, float b, float a)
  {
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
  }
};


struct PointLight
{
  float pos[4];
  float ambient[4];
  float diffuse[4];
  float specular[4];

  void SetPos(float x, float y, float z, float w)
  {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = w;
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

  void SetSpecular(float r, float g, float b, float a)
  {
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
  }
};


struct LightUniforms
{
  float globalAmbient[4];

  std::vector<DirLight> DirLights;

  std::vector<SpotLight> SpotLights;

  std::vector<PointLight> PointLights;

  float spotInnerAngle;
  float spotOuterAngle;
  float spotFalloff;

  float c1;
  float c2;
  float c3;

  float fogColor[4];
  float fogNear;
  float fogFar;

};
