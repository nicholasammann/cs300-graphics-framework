#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model.hpp"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"


namespace ELBA
{
  Application::Application(GLFWwindow *aWindow)
    : mWindow(aWindow), mShader(nullptr), mVAO(0), mCamera(new Camera())
  {
    
  }

  Application::~Application()
  {
  }

  void Application::Init()
  {
    glEnable(GL_DEPTH_TEST);

    ImGui_ImplGlfwGL3_Init(mWindow, true);

    mModels.push_back(new Model("../OpenGLFramework/Assets/CS300/cube.obj"));
  }

  void Application::CreateShader(const char * aVertShaderPath, const char * aFragShaderPath)
  {
    if (mShader)
    {
      delete mShader;
    }

    mShader = new Shader(aVertShaderPath, aFragShaderPath);
  }

  void Application::Update(int aWidth, int aHeight)
  {
    mWindowWidth = aWidth;
    mWindowHeight = aHeight;

    ImGui_ImplGlfwGL3_NewFrame();

    // input
    ProcessInput();

    EditorUpdate();

    // rendering commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

  void Application::EditorUpdate()
  {
    ImGui::Begin("Test Window");

    ImGui::Text("HEY FAMB");

    ImGui::End();
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

  Shader * Application::GetShader()
  {
    return mShader;
  }

  unsigned int Application::GetVAO()
  {
    return mVAO;
  }

  Camera * Application::GetCamera()
  {
    return mCamera;
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
    mShader->UseShaderProgram();

    int colorLoc = glGetUniformLocation(mShader->GetShaderProgram(), "Color");
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);


    glm::vec3 pos(0, 0, 0);
    glm::vec3 scale(1.0f, 1.0f, 1.0f);
    glm::vec3 rotate(0, 0, 0);

    glm::mat4 model;
    model = glm::scale(model, scale);
    model = glm::rotate(model, rotate.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotate.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, rotate.z, glm::vec3(0, 0, 1));
    model = glm::translate(model, pos);
    unsigned int modelLoc = glGetUniformLocation(mShader->GetShaderProgram(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    Camera *cam = mCamera;

    cam->mPosition.x = static_cast<float>(sin(glfwGetTime()) * 10);
    cam->mPosition.z = static_cast<float>(cos(glfwGetTime()) * 10);

    glm::mat4 view;
    view = glm::lookAt(cam->mPosition, cam->mTarget, cam->mCameraUp);
    unsigned int viewLoc = glGetUniformLocation(mShader->GetShaderProgram(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(mWindowWidth) / mWindowHeight, 0.1f, 100.0f);
    unsigned int projLoc = glGetUniformLocation(mShader->GetShaderProgram(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    for (Model *m : mModels)
    {
      m->Draw(mShader);
    }
  }

}