#pragma once

#include "../Core/Camera.hpp"

namespace ELBA
{
  class Application;
  class Framebuffer;

  class CubeMap
  {
  public:
    CubeMap(Application *aApp);

    void UpdateTextures(glm::vec3 aPos);

  private:
    Application *mApplication;

    Camera *mCamera;

    Framebuffer *mTop;
    Framebuffer *mBottom;
    Framebuffer *mFront;
    Framebuffer *mBack;
    Framebuffer *mLeft;
    Framebuffer *mRight;

  };
}
