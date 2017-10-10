#pragma once

#include <vector>
#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>

#include "../Graphics/Light.hpp"

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
    void CreateShader(const char *aName, const char *vertShaderPath, const char *fragShaderPath);
    void Update(int aWidth, int aHeight);
    void Shutdown();

    GLFWwindow* GetWindow();

    int GetWindowWidth();
    int GetWindowHeight();

    Shader* GetShader(const char *aName);

    Camera* GetCamera();

    std::vector<Model*>& GetModels();

    std::vector<Light>& GetLights();

  private:

    Editor *mEditor;

    GLFWwindow *mWindow;

    void ProcessInput();
    void Render();

    std::map<std::string, Shader*> mShaders;

    std::vector<Model*> mModels;

    std::vector<Light> mLights;

    Camera *mCamera;
  
    int mWindowWidth;
    int mWindowHeight;

    void CreateInitialShaders();
    void CreateInitialModels();
    void CreateInitialLights();

    void BindLights(unsigned int aShaderPrg);

  };
}

