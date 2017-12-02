#include <GL/glew.h>

#include <iostream>

#include "../Core/Application.hpp"

#include "Framebuffer.hpp"

#include "CubeMap.hpp"

enum TexSlots
{
  Top = 3,
  Bottom,
  Front,
  Back,
  Left,
  Right
};


namespace ELBA
{

  CubeMap::CubeMap(Application *aApp) : mApplication(aApp)
  {
    mCamera = new Camera();

    mTop = new Framebuffer();
    mTop->Build(Front);

    mBottom = new Framebuffer();
    mBottom->Build(Bottom);

    mFront = new Framebuffer();
    mFront->Build(Front);

    mBack = new Framebuffer();
    mBack->Build(Back);

    mLeft = new Framebuffer();
    mLeft->Build(Left);

    mRight = new Framebuffer();
    mRight->Build(Right);
  }

  void CubeMap::UpdateTextures(glm::vec3 aPos)
  {
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    mCamera->SetPosition(aPos);
    
    mCamera->SetTargetPoint(aPos + glm::vec3(0, 1, 0));
    mTop->Bind();
    mTop->Clear();
    mApplication->Render(mCamera, 512, 512, false);
    mTop->Unbind();

    mCamera->SetTargetPoint(aPos + glm::vec3(0, -1, 0));
    mBottom->Bind();
    mBottom->Clear();
    mApplication->Render(mCamera, 512, 512, false);
    mBottom->Unbind();
    
    mCamera->SetTargetPoint(aPos + glm::vec3(0, 0, 1));
    mFront->Bind();
    mFront->Clear();
    mApplication->Render(mCamera, 512, 512, false);
    mFront->Unbind();
    
    mCamera->SetTargetPoint(aPos + glm::vec3(0, 0, -1));
    mBack->Bind();
    mBack->Clear();
    mApplication->Render(mCamera, 512, 512, false);
    mBack->Unbind();
    
    mCamera->SetTargetPoint(aPos + glm::vec3(-1, 0, 0));
    mLeft->Bind();
    mLeft->Clear();
    mApplication->Render(mCamera, 512, 512, false);
    mLeft->Unbind();
    
    mCamera->SetTargetPoint(aPos + glm::vec3(1, 0, 0));
    mRight->Bind();
    mRight->Clear();
    mApplication->Render(mCamera, 512, 512, false);
    mRight->Unbind();

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
