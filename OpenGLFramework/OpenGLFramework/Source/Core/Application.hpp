#pragma once

#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>

namespace ELBA
{
  class Shader;
  class Model;
  class Camera;
  class Editor;

  class Application
  {
  public:

    Application(GLFWwindow *aWindow);
    ~Application();

    void Init();
    void CreateShader(const char *vertShaderPath, const char *fragShaderPath);
    void Update(int aWidth, int aHeight);
    void Shutdown();

    GLFWwindow* GetWindow();

    int GetWindowWidth();
    int GetWindowHeight();

    Shader* GetShader();
    unsigned int GetVAO();

    Camera* GetCamera();

    std::vector<Model*>& GetModels();

  private:

    Editor *mEditor;

    GLFWwindow *mWindow;

    void ProcessInput();
    void Render();

    Shader *mShader;

    std::vector<Model*> mModels;

    Camera *mCamera;
  
    int mWindowWidth;
    int mWindowHeight;

  };
}

