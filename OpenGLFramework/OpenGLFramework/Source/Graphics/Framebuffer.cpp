#include <GL/glew.h>

#include <iostream>

#include "Framebuffer.hpp"

namespace ELBA
{

  Framebuffer::Framebuffer()
  {
  }
  
  void Framebuffer::Build(unsigned int aShaderPrg)
  {
    glUseProgram(aShaderPrg);

    // create texture
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // create framebuffer
    glGenFramebuffers(1, &mHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
    glViewport(0, 0, 512, 512);

    // add depth buffer
    glGenRenderbuffers(1, &mRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);

    // add the render target
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRenderBuffer);

    glViewport(0, 0, 512, 512);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(0);
  }

  void Framebuffer::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, mHandle);
    glViewport(0, 0, 512, 512);
  }

  void Framebuffer::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Framebuffer::Clear()
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Framebuffer::Destroy()
  {
    Unbind();

    glDeleteRenderbuffers(1, &mRenderBuffer);
    mRenderBuffer = 0;

    glDeleteFramebuffers(1, &mHandle);
    mHandle = 0;
  }

  void Framebuffer::BindTexture(int aSlot)
  {
    glActiveTexture(GL_TEXTURE0 + aSlot);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    mTextureSlot = aSlot;
  }

  void Framebuffer::SetTextureUniform(unsigned int aShaderPrg, std::string aUniform, int aSlot)
  {
    glUseProgram(aShaderPrg);
    int loc = glGetUniformLocation(aShaderPrg, aUniform.c_str());
    glUniform1i(loc, aSlot);
  }

  void Framebuffer::UnbindTexture()
  {
    glActiveTexture(GL_TEXTURE0 + mTextureSlot);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

}