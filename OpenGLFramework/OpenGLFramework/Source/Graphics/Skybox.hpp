#pragma once

#include <gl/glew.h>
#include <string>

enum
{
  Top,
  Bottom,
  Front,
  Back,
  Left,
  Right
};

namespace ELBA
{
  class Application;

  class Skybox
  {
  public:

    Skybox(Application *aApp, unsigned int aShaderPrg);

    void Build();

    void LoadTextures();

    void LoadOneSide(GLenum aSide, int aDir, std::string aFilename);

    void Draw();



  private:

    Application *mApp;

    unsigned int mShaderPrg;

    unsigned int mVAO;
    unsigned int mVBO;

    unsigned int mTexture;

  };

}
