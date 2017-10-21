#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace ELBA
{

  Camera::Camera()
  {
    mPosition = glm::vec3(0.0f, 0.0f, 20.0f);
    mTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    mDirection = glm::normalize(mTarget - mPosition);
    mWorldUp = glm::vec3(0.0f, 0.1f, 0.0f);
    mCameraRight = glm::normalize(glm::cross(mWorldUp, mDirection));
    mCameraUp = glm::cross(mDirection, mCameraRight);
  }

  glm::mat4 Camera::ConstructViewMatrix()
  {
    glm::vec3 forward(normalize(mTarget - mPosition));
    glm::vec3 right(normalize(cross(forward, mWorldUp)));
    glm::vec3 up(cross(right, forward));

    glm::vec3 p = mPosition;

    glm::mat4 view;

    // column 0
    view[0][0] = right.x;            
    view[1][0] = right.y;            
    view[2][0] = right.z;
    view[3][0] = -dot(right, p);
    
    // column 1
    view[0][1] = up.x;
    view[1][1] = up.y;
    view[2][1] = up.z;
    view[3][1] = -dot(up, p);

    // column 2
    view[0][2] = -forward.x;
    view[1][2] = -forward.y;
    view[2][2] = -forward.z;
    view[3][2] = dot(forward, p);

    view[3][3] = 1.0f;


    return view;
  }

  glm::mat4 Camera::ConstructProjMatrix(int aWidth, int aHeight)
  {
    glm::mat4 proj;

    float rad = glm::radians(45.0f);
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

}