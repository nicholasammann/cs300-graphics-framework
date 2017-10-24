/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.hpp
Purpose: Defining the different scenes
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

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