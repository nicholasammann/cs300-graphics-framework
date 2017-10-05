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
  Application::Application()
    : mShader(nullptr), mVAO(0), mCamera(new Camera())
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(800, 600, "Open GL Framework", nullptr, nullptr);
    glfwMakeContextCurrent(mWindow);

    if (!mWindow)
    {
      std::cout << "Failed to create GLFW window." << std::endl;
      glfwTerminate();
      return;
    }

    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
      std::cout << "glewInit failed." << std::endl;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
  }

  Application::~Application()
  {
  }

  void Application::Init()
  {
    glEnable(GL_DEPTH_TEST);

    ImGui_ImplGlfwGL3_Init(mWindow, true);

    Model *crysis = new Model("../OpenGLFramework/Assets/CrysisModel/nanosuit.obj");
    mModels.push_back(crysis);
  }

  void Application::CreateShader(const char * aVertShaderPath, const char * aFragShaderPath)
  {
    if (mShader)
    {
      delete mShader;
    }

    mShader = new Shader(aVertShaderPath, aFragShaderPath);
  }

  void Application::Update()
  {
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

  void Application::FramebufferSizeCallback(GLFWwindow * aWindow, int aWidth, int aHeight)
  {
    glViewport(0, 0, aWidth, aHeight);
  }

  void Application::Render()
  {
    mShader->UseShaderProgram();

    glm::vec3 pos(0, -6.0f, 0);
    glm::vec3 scale(0.2f, 0.2f, 0.2f);
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

    cam->mPosition.x = sin(glfwGetTime()) * 10;
    cam->mPosition.z = cos(glfwGetTime()) * 10;

    glm::mat4 view;
    view = glm::lookAt(cam->mPosition, cam->mTarget, cam->mCameraUp);
    unsigned int viewLoc = glGetUniformLocation(mShader->GetShaderProgram(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(800) / 600, 0.1f, 100.0f);
    unsigned int projLoc = glGetUniformLocation(mShader->GetShaderProgram(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    for (Model *m : mModels)
    {
      m->Draw(mShader);
    }
  }

}