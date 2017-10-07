#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"

#include "../Graphics/Shader.hpp"
#include "../Graphics/Model.hpp"

#include "Application.hpp"
#include "Camera.hpp"
#include "Editor.hpp"

namespace ELBA
{
  Application::Application(GLFWwindow *aWindow)
    : mWindow(aWindow), mCamera(new Camera()),
      mEditor(new Editor(this))
  {
    
  }

  Application::~Application()
  {
  }

  void Application::Init()
  {
    glEnable(GL_DEPTH_TEST);

    ImGui_ImplGlfwGL3_Init(mWindow, true);

    CreateShader("Simple", "Assets/Shaders/simple.vert", "Assets/Shaders/simple.frag");

    Model *mod = new Model(this, "../OpenGLFramework/Assets/CS300/cube.obj", "Cube");
    mod->SetShader("Simple");

    mModels.push_back(mod);
  }

  void Application::CreateShader(const char *aName, const char * aVertShaderPath, const char * aFragShaderPath)
  {
    // find out if we have already loaded the shader
    auto it = mShaders.find(aName);

    // shader has already been loaded
    if (it != mShaders.end())
    {
      // erase it so we can reload it
      mShaders.erase(aName);
    }

    // load the new shader and store it in the map
    mShaders[aName] = new Shader(aName, aVertShaderPath, aFragShaderPath);
  }

  void Application::Update(int aWidth, int aHeight)
  {
    mWindowWidth = aWidth;
    mWindowHeight = aHeight;

    ImGui_ImplGlfwGL3_NewFrame();

    // input
    ProcessInput();

    mEditor->Update();

    Render();

    ImGui::Render();

    // check and call events and swap buffers
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }

  void Application::Shutdown()
  {
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
  }

  GLFWwindow * Application::GetWindow()
  {
    return mWindow;
  }

  int Application::GetWindowWidth()
  {
    return mWindowWidth;
  }

  int Application::GetWindowHeight()
  {
    return mWindowHeight;
  }

  Shader * Application::GetShader(const char *aName)
  {
    // find out if we have loaded the shader
    auto it = mShaders.find(aName);

    // if not, return nullptr
    if (it == mShaders.end())
    {
      return nullptr;
    }

    // otherwise return the pointer
    return it->second;
  }

  Camera * Application::GetCamera()
  {
    return mCamera;
  }

  std::vector<Model*>& Application::GetModels()
  {
    return mModels;
  }

  void Application::ProcessInput()
  {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(mWindow, true);
    }
  }

  void Application::Render()
  {
    // rendering commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Model *m : mModels)
    {
      Shader *shdr = m->GetShader();

      if (!shdr)
      {
        // TODO: change this to load a default shader
        continue;
      }

      unsigned int shdrPrg = shdr->GetShaderProgram();

      int colorLoc = glGetUniformLocation(shdrPrg, "Color");
      glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

      Camera *cam = mCamera;

      glm::mat4 view;
      view = glm::lookAt(cam->mPosition, cam->mTarget, cam->mCameraUp);
      unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      glm::mat4 projection;
      projection = glm::perspective(glm::radians(45.0f), static_cast<float>(mWindowWidth) / mWindowHeight, 0.1f, 100.0f);
      unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

      Transform &tr = m->GetTransform();
      
      glm::mat4 model;
      model = glm::scale(model, tr.mScale);
      model = glm::rotate(model, tr.mWorldRot.x, glm::vec3(1, 0, 0));
      model = glm::rotate(model, tr.mWorldRot.y, glm::vec3(0, 1, 0));
      model = glm::rotate(model, tr.mWorldRot.z, glm::vec3(0, 0, 1));
      model = glm::translate(model, tr.mWorldPos);
      unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      m->Draw(shdr);
    }
  }

}