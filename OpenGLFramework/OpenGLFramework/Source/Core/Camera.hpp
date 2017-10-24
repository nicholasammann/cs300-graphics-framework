/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.hpp
Purpose: Camera class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


namespace ELBA
{

  class Camera
  {
  public:
    Camera();

    void UpdateVectors();

    glm::mat4 ConstructViewMatrix();

    glm::mat4 ConstructProjMatrix(int aWidth, int aHeight);

    void SetPosition(glm::vec3 aPos);

    void SetTargetPoint(glm::vec3 aPoint);

    glm::vec3 mPosition;
    glm::vec3 mTarget;
    glm::vec3 mDirection;
    glm::vec3 mWorldUp;
    glm::vec3 mCameraUp;
    glm::vec3 mCameraRight;

  };

}