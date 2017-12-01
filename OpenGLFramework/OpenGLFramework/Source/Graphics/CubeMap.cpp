#include <GL/glew.h>

#include "../Core/Application.hpp"

#include "Framebuffer.hpp"

#include "CubeMap.hpp"


namespace ELBA
{

  CubeMap::CubeMap(Application *aApp) : mApplication(aApp)
  {
    mCamera = new Camera();

    mTop = new Framebuffer();
    mTop->Build();

    mBottom = new Framebuffer();
    mBottom->Build();

    mFront = new Framebuffer();
    mFront->Build();

    mBack = new Framebuffer();
    mBack->Build();

    mLeft = new Framebuffer();
    mLeft->Build();

    mRight = new Framebuffer();
    mRight->Build();
  }

  void CubeMap::UpdateTextures(glm::vec3 aPos)
  {
    mCamera->SetPosition(aPos);
    mCamera->SetTargetPoint(aPos + glm::vec3(0, 1, 0));
    mTop->Bind();
    mApplication->Render(mCamera, 512, 512);
    mTop->Unbind();

    
    mCamera->SetPosition(aPos);
    mCamera->SetTargetPoint(aPos + glm::vec3(0, -1, 0));
    mBottom->Bind();
    mApplication->Render(mCamera, 512, 512);
    mBottom->Unbind();


    mCamera->SetPosition(aPos);
    mCamera->SetTargetPoint(aPos + glm::vec3(0, 0, 1));
    mFront->Bind();
    mApplication->Render(mCamera, 512, 512);
    mFront->Unbind();


    mCamera->SetPosition(aPos);
    mCamera->SetTargetPoint(aPos + glm::vec3(0, 0, -1));
    mBack->Bind();
    mApplication->Render(mCamera, 512, 512);
    mBack->Unbind();


    mCamera->SetPosition(aPos);
    mCamera->SetTargetPoint(aPos + glm::vec3(-1, 0, 0));
    mLeft->Bind();
    mApplication->Render(mCamera, 512, 512);
    mLeft->Unbind();


    mCamera->SetPosition(aPos);
    mCamera->SetTargetPoint(aPos + glm::vec3(1, 0, 0));
    mRight->Bind();
    mApplication->Render(mCamera, 512, 512);
    mRight->Unbind();
  }

}
