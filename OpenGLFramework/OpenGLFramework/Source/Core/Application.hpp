#pragma once

#include <vector>
#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\vec3.hpp>

#include "../ImGui/imgui.h"

#include "../Graphics/Light.hpp"

namespace ELBA
{
  class Shader;
  class Model;
  class Camera;
  class Editor;

  using shdr_info = std::tuple<std::string, std::string, std::string>;
  using shader_path_vec = std::vector<shdr_info>;

  class Application
  {
  public:

    Application(GLFWwindow *aWindow);
    ~Application();

    void Init();
    void CreateShader(std::string aName, std::string vertShaderPath, std::string fragShaderPath);
    void Update(int aWidth, int aHeight);
    void Shutdown();

    GLFWwindow* GetWindow();

    int GetWindowWidth();
    int GetWindowHeight();

    Shader* GetShader(const char *aName);

    Camera* GetCamera();

    std::vector<Model*>& GetModels();

    std::map<std::string, Shader*>& GetShaderMap();

    ImVector<char*> mShaderNames;

    shader_path_vec& GetShaderPaths();

    void ReloadShaderNamesForEditor();

    LightUniforms& GetLightUniforms();

  private:

    Editor *mEditor;

    GLFWwindow *mWindow;

    void ProcessInput();
    void Render();

    std::map<std::string, Shader*> mShaders;

    shader_path_vec mShaderPaths;

    std::vector<Model*> mModels;

    LightUniforms mLightUniforms;

    Camera *mCamera;
  
    int mWindowWidth;
    int mWindowHeight;

    void CreateInitialShaders();
    void CreateInitialModels();
    void CreateInitialLights();

    void BindLights(unsigned int aShaderPrg);
    void UpdateLights();
    void RenderLightModels();

  };
}

