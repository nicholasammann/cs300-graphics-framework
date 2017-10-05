#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>
#include <vector>

namespace ELBA
{
  class Shader;
  class Model;
  class Camera;

  class Application
  {
  public:

    Application();
    ~Application();

    void Init();
    void CreateShader(const char *vertShaderPath, const char *fragShaderPath);
    void Update();
    void Shutdown();


    void EditorUpdate();


    GLFWwindow* GetWindow();

    Shader* GetShader();
    unsigned int GetVAO();

    Camera* GetCamera();

  private:

    GLFWwindow *mWindow;

    void ProcessInput();
    void Render();

    Shader *mShader;
    unsigned int mVAO;

    std::vector<Model*> mModels;

    Camera *mCamera;

    // callback for window resizing
    static void FramebufferSizeCallback(GLFWwindow *aWindow, int aWidth, int aHeight);

  };
}

