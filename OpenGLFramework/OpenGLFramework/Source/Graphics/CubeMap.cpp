#include <GL/glew.h>

#include <iostream>

#include "../Core/Application.hpp"

#include "Framebuffer.hpp"

#include "CubeMap.hpp"

enum TexSlots
{
  Top = 4,
  Bottom,
  Front,
  Back,
  Left,
  Right
};


namespace ELBA
{

  CubeMap::CubeMap(Application *aApp, unsigned int aShaderPrg) : mApplication(aApp)
  {
    mTop = new Framebuffer();
    mTop->Build(aShaderPrg);

    mBottom = new Framebuffer();
    mBottom->Build(aShaderPrg);

    mFront = new Framebuffer();
    mFront->Build(aShaderPrg);

    mBack = new Framebuffer();
    mBack->Build(aShaderPrg);

    mLeft = new Framebuffer();
    mLeft->Build(aShaderPrg);

    mRight = new Framebuffer();
    mRight->Build(aShaderPrg);
  }

  void CubeMap::UpdateTextures(glm::vec3 aPos)
  {
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    Camera *camera = mApplication->GetCamera();

    glm::vec3 oldPos = camera->mPosition;
    glm::vec3 oldDir = camera->mDirection;

    camera->SetPosition(aPos);

    camera->SetDirection(glm::vec3(0, 10, 0.1));
    mTop->Bind();
    mTop->Clear();
    mApplication->Render(camera, 512, 512, false);
    mTop->Unbind();

    camera->SetDirection(glm::vec3(0, -10, 0.1));
    mBottom->Bind();
    mBottom->Clear();
    mApplication->Render(camera, 512, 512, false);
    mBottom->Unbind();
    
    camera->SetDirection(glm::vec3(0, 0, 10));
    mFront->Bind();
    mFront->Clear();
    mApplication->Render(camera, 512, 512, false);
    mFront->Unbind();
    
    camera->SetDirection(glm::vec3(0, 0, -10));
    mBack->Bind();
    mBack->Clear();
    mApplication->Render(camera, 512, 512, false);
    mBack->Unbind();
    
    camera->SetDirection(glm::vec3(-10, 0, 0));
    mLeft->Bind();
    mLeft->Clear();
    mApplication->Render(camera, 512, 512, false);
    mLeft->Unbind();
    
    camera->SetDirection(glm::vec3(10, 0, 0));
    mRight->Bind();
    mRight->Clear();
    mApplication->Render(camera, 512, 512, false);
    mRight->Unbind();

    camera->SetPosition(oldPos);
    //camera->SetDirection(oldDir);

    glDisable(GL_CULL_FACE);
  }


  void CubeMap::SetTextureUniforms(unsigned int aShaderPrg)
  {
    mTop->SetTextureUniform(aShaderPrg, "CubeMapTop", Top);
    mTop->BindTexture(Top);
    
    mBottom->SetTextureUniform(aShaderPrg, "CubeMapBottom", Bottom);
    mBottom->BindTexture(Bottom);
    
    mFront->SetTextureUniform(aShaderPrg, "CubeMapFront", Front);
    mFront->BindTexture(Front);
    
    mBack->SetTextureUniform(aShaderPrg, "CubeMapBack", Back);
    mBack->BindTexture(Back);
    
    mLeft->SetTextureUniform(aShaderPrg, "CubeMapLeft", Left);
    mLeft->BindTexture(Left);
    
    mRight->SetTextureUniform(aShaderPrg, "CubeMapRight", Right);
    mRight->BindTexture(Right);
  }

  void CubeMap::UnbindTextures()
  {
    mTop->UnbindTexture();
    mBottom->UnbindTexture();

    mFront->UnbindTexture();
    mBack->UnbindTexture();

    mLeft->UnbindTexture();
    mRight->UnbindTexture();
  }

}
