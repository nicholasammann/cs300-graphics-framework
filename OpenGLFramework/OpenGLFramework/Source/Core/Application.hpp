#pragma once

/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Application.hpp
Purpose: Main Window
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

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
  class CubeMap;

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

    float* GetLightSpeed();

    bool mRotateLights;
    bool mRotateCamera;

    void UpdateCamera();

    void Render(Camera *aCamera, int aWidth, int aHeight, bool aUseClearColor);

  private:

    Model *mSkybox;

    Editor *mEditor;

    GLFWwindow *mWindow;

    void ProcessInput();

    std::map<std::string, Shader*> mShaders;

    shader_path_vec mShaderPaths;

    std::vector<Model*> mModels;

    float mLightSpeed;
    LightUniforms mLightUniforms;

    glm::vec4 mBackgroundColor;

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

