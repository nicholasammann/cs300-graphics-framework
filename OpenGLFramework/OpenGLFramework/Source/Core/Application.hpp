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

    Application(GLFWwindow *aWindow);
    ~Application();

    void Init();
    void CreateShader(const char *vertShaderPath, const char *fragShaderPath);
    void Update(int aWidth, int aHeight);
    void Shutdown();


    void EditorUpdate();


    GLFWwindow* GetWindow();

    int GetWindowWidth();
    int GetWindowHeight();

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
  
    int mWindowWidth;
    int mWindowHeight;

  };
}

