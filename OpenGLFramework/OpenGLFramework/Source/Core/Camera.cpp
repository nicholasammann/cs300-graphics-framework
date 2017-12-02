/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: Camera class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace ELBA
{

  Camera::Camera()
  {
    mPosition = glm::vec3(0.0f, 0.0f, 10.0f);
    mTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    mDirection = glm::normalize(mTarget - mPosition);
    mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mCameraRight = glm::normalize(glm::cross(mWorldUp, mDirection));
    mCameraUp = glm::cross(mDirection, mCameraRight);
  }

  void Camera::UpdateVectors()
  {
    mDirection = normalize(mTarget - mPosition);
    mCameraRight = normalize(cross(mDirection, mWorldUp));
    mCameraUp = cross(mCameraRight, mDirection);
  }

  glm::mat4 Camera::ConstructViewMatrix()
  {
    UpdateVectors();

    glm::vec3 p = mPosition;

    glm::mat4 view;

    // column 0
    view[0][0] = mCameraRight.x;
    view[1][0] = mCameraRight.y;
    view[2][0] = mCameraRight.z;
    view[3][0] = -dot(mCameraRight, p);
    
    // column 1
    view[0][1] = mCameraUp.x;
    view[1][1] = mCameraUp.y;
    view[2][1] = mCameraUp.z;
    view[3][1] = -dot(mCameraUp, p);

    // column 2
    view[0][2] = -mDirection.x;
    view[1][2] = -mDirection.y;
    view[2][2] = -mDirection.z;
    view[3][2] = dot(mDirection, p);

    view[3][3] = 1.0f;

    return view;
  }

  glm::mat4 Camera::ConstructProjMatrix(int aWidth, int aHeight)
  {
    glm::mat4 proj;

    float rad = glm::radians(90.0f);
    float tanHalfFovy = tan(rad / 2.0f);
    float aspect = (float)aWidth / aHeight;
    float zNear = 0.1f;
    float zFar = 50.0f;

    proj[0][0] = 1.0f / (aspect * tanHalfFovy);
    proj[1][1] = 1.0f / (tanHalfFovy);
    proj[2][2] = -(zFar + zNear) / (zFar - zNear);
    proj[2][3] = -1.0f;
    proj[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);

    return proj;
  }

  void Camera::SetPosition(glm::vec3 aPos)
  {
    mPosition = aPos;
    UpdateVectors();
  }

  void Camera::SetTargetPoint(glm::vec3 aPoint)
  {
    mTarget = aPoint;
    UpdateVectors();
  }

  void Camera::SetDirection(glm::vec3 aDir)
  {
    mDirection = normalize(aDir);
    mTarget = mPosition + mDirection;
    mCameraRight = glm::normalize(glm::cross(mWorldUp, mDirection));
    mCameraUp = glm::cross(mDirection, mCameraRight);
  }

  glm::mat4 Camera::ConstructSkyboxView()
  {
    UpdateVectors();

    glm::vec3 p(0, 0, 0);

    glm::mat4 view;

    // column 0
    view[0][0] = mCameraRight.x;
    view[1][0] = mCameraRight.y;
    view[2][0] = mCameraRight.z;
    view[3][0] = -dot(mCameraRight, p);

    // column 1
    view[0][1] = mCameraUp.x;
    view[1][1] = mCameraUp.y;
    view[2][1] = mCameraUp.z;
    view[3][1] = -dot(mCameraUp, p);

    // column 2
    view[0][2] = -mDirection.x;
    view[1][2] = -mDirection.y;
    view[2][2] = -mDirection.z;
    view[3][2] = dot(mDirection, p);

    view[3][3] = 1.0f;

    return view;
  }

}