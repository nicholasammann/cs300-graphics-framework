#pragma once

#include "../Core/Camera.hpp"

namespace ELBA
{
  class Application;
  class Framebuffer;

  class CubeMap
  {
  public:
    CubeMap(Application *aApp, unsigned int aShaderPrg);

    void UpdateTextures(glm::vec3 aPos);

    void SetTextureUniforms(unsigned int aShaderPrg);

    void UnbindTextures();

  private:
    Application *mApplication;

    Framebuffer *mTop;
    Framebuffer *mBottom;
    Framebuffer *mFront;
    Framebuffer *mBack;
    Framebuffer *mLeft;
    Framebuffer *mRight;

  };
}
