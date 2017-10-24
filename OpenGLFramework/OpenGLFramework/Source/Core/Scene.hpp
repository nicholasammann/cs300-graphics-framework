#pragma once
#include "Application.hpp"

namespace ELBA
{

  enum LightType
  {
    Directional,
    Spot,
    Point
  };

  namespace Scene1
  {
    void CreateLights(Application *aApp, int aLightType);
  }

  namespace Scene2
  {
    void CreateLights(Application *aApp);
  }

  namespace Scene3
  {
    void CreateLights(Application *aApp);
  }


}