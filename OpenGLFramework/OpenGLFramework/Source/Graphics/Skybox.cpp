
#include <glm/gtc/type_ptr.hpp>

#include "../Utils/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Utils/stb_image_write.h"
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#include "../Core/Application.hpp"
#include "../Core/Camera.hpp"

#include <iostream>

#include "Skybox.hpp"


namespace ELBA
{
  float verts[] = {
    -10.0f,  10.0f, -10.0f,
    -10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,

    -10.0f, -10.0f,  10.0f,
    -10.0f, -10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f,  10.0f,
    -10.0f, -10.0f,  10.0f,

    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f,  10.0f,
    10.0f,  10.0f,  10.0f,
    10.0f,  10.0f,  10.0f,
    10.0f,  10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,

    -10.0f, -10.0f,  10.0f,
    -10.0f,  10.0f,  10.0f,
    10.0f,  10.0f,  10.0f,
    10.0f,  10.0f,  10.0f,
    10.0f, -10.0f,  10.0f,
    -10.0f, -10.0f,  10.0f,

    -10.0f,  10.0f, -10.0f,
    10.0f,  10.0f, -10.0f,
    10.0f,  10.0f,  10.0f,
    10.0f,  10.0f,  10.0f,
    -10.0f,  10.0f,  10.0f,
    -10.0f,  10.0f, -10.0f,

    -10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f,  10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f,  10.0f,
    10.0f, -10.0f,  10.0f
  };


  Skybox::Skybox(Application *aApp, unsigned int aShaderPrg) : mApp(aApp), mShaderPrg(aShaderPrg)
  {
  }

  void Skybox::Build()
  {
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    LoadTextures();
  }

  void Skybox::LoadTextures()
  {
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &mTexture);

    LoadOneSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Top,    "../OpenGLFramework/Assets/Textures/skybox/ashcanyon_up.tga");
    LoadOneSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Bottom, "../OpenGLFramework/Assets/Textures/skybox/ashcanyon_dn.tga");
    LoadOneSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Front,  "../OpenGLFramework/Assets/Textures/skybox/ashcanyon_ft.tga");
    LoadOneSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Back,   "../OpenGLFramework/Assets/Textures/skybox/ashcanyon_bk.tga");
    LoadOneSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Left,   "../OpenGLFramework/Assets/Textures/skybox/ashcanyon_lf.tga");
    LoadOneSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, Right,  "../OpenGLFramework/Assets/Textures/skybox/ashcanyon_rt.tga");

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void Skybox::LoadOneSide(GLenum aSide, int aDir, std::string aFilename)
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    int width, height, n;

    unsigned char *image = stbi_load(aFilename.c_str(), &width, &height, &n, 3);

    int result = stbi_write_png("/skyboxTex.png", width, height, 3, image, width * 3 * sizeof(unsigned char));

    glTexImage2D(aSide, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    if (!image) {
      std::cout << aFilename.c_str() << std::endl;
    }
    
    delete image;
  }

  void Skybox::Draw()
  {
    glDepthMask(GL_FALSE);
    glUseProgram(mShaderPrg);

    Camera *cam = mApp->GetCamera();

    glm::mat4 view = cam->ConstructSkyboxView();
    unsigned int viewLoc = glGetUniformLocation(mShaderPrg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = cam->ConstructProjMatrix(mApp->GetWindowWidth(), mApp->GetWindowHeight());
    unsigned int projLoc = glGetUniformLocation(mShaderPrg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int loc = glGetUniformLocation(mShaderPrg, "skyboxTexture");
    glUniform1i(loc, GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
  }


}
