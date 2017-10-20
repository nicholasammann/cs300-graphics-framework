#include <iostream>
#include <filesystem>
#include <exception>

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

    CreateInitialShaders();

    ReloadShaderNamesForEditor();

    CreateInitialModels();
    CreateInitialLights();
  }

  void Application::CreateShader(std::string aName, std::string aVertShaderPath, std::string aFragShaderPath)
  {
    // find out if we have already loaded the shader
    auto it = mShaders.find(aName);

    // shader has already been loaded
    if (it != mShaders.end())
    {
      // erase it so we can reload it
      mShaders.erase(aName);
    }

    try
    {
      // load the new shader and store it in the map
      mShaders[aName] = new Shader(aName.c_str(), aVertShaderPath.c_str(), aFragShaderPath.c_str());
    }
    catch (std::string &error)
    {
      // print out error to console
      mEditor->PrintToConsole(error);
    }
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

  std::vector<Light>& Application::GetLights()
  {
    return mLights;
  }

  std::map<std::string, Shader*>& Application::GetShaderMap()
  {
    return mShaders;
  }

  shader_path_vec & Application::GetShaderPaths()
  {
    return mShaderPaths;
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
        shdr = this->GetShader("Simple");
      }

      unsigned int shdrPrg = shdr->GetShaderProgram();

      BindLights(shdrPrg);

      glm::mat4 view = mCamera->ConstructViewMatrix();
      unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      glm::mat4 projection = mCamera->ConstructProjMatrix(mWindowWidth, mWindowHeight);
      unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

      glm::mat4 model = m->ConstructModelMatrix();
      unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      m->Draw(shdr, projection, view, model);
    }
  }

  void Application::CreateInitialShaders()
  {
    std::string p = "Assets/Shaders/";

    CreateShader("Simple", p+"simple.vert", p+"simple.frag");
    mShaderPaths.emplace_back(shdr_info("Simple", p+"simple.vert", p+"simple.frag"));

    CreateShader("Phong Lighting", p+"phong_light.vert", p+"phong_light.frag");
    mShaderPaths.emplace_back(shdr_info("Phong Lighting", p+"phong_light.vert", p+"phong_light.frag"));
    
    CreateShader("Debug", p+"debug.vert", p+"debug.frag");
    mShaderPaths.emplace_back(shdr_info("Debug", p+"debug.vert", p+"debug.frag"));
  }

  void Application::CreateInitialModels()
  {
    Model *mod = new Model(this, "../OpenGLFramework/Assets/CS300/cube.obj", "Cube");
    mod->SetShader("Shader");

    mModels.push_back(mod);
  }

  void Application::CreateInitialLights()
  {
    Light light;
    light.SetDirection(-1, -1, 0, 1);
    light.SetAmbient(1, 0, 0, 1);
    light.SetDiffuse(1, 0, 0, 1);

    mLights.push_back(light);

    light.SetDirection(1, 1, 0, 1);
    light.SetAmbient(0, 1, 0, 1);
    light.SetDiffuse(0, 1, 0, 1);
    //mLights.push_back(light);
    //mLights.push_back(light);
    //mLights.push_back(light);
    //mLights.push_back(light);
    //mLights.push_back(light);
    //mLights.push_back(light);
    //mLights.push_back(light);

  }

  void Application::BindLights(unsigned int aShaderPrg)
  {
    for (unsigned int i = 0; i < mLights.size(); ++i)
    {
      std::string lightName = "Lights[" + std::to_string(i) +"].";

      std::string dir = lightName + "direction";
      std::string amb = lightName + "ambient";
      std::string dif = lightName + "diffuse";

      unsigned int loc = glGetUniformLocation(aShaderPrg, dir.c_str());
      glProgramUniform4fv(aShaderPrg, loc, 1, mLights[i].direction);

      loc = glGetUniformLocation(aShaderPrg, amb.c_str());
      glProgramUniform4fv(aShaderPrg, loc, 1, mLights[i].ambient);

      loc = glGetUniformLocation(aShaderPrg, dif.c_str());
      glProgramUniform4fv(aShaderPrg, loc, 1, mLights[i].diffuse);

      // bind light count
      unsigned int countLoc = glGetUniformLocation(aShaderPrg, "LightCount");
      glProgramUniform1i(aShaderPrg, countLoc, mLights.size());
    }
  }
  void Application::ReloadShaderNamesForEditor()
  {
    mShaderNames.clear();

    // load names for all shaders
    for (auto &s : mShaders)
    {
      char *name = new char[s.second->GetName().size()];
      strcpy(name, s.second->GetName().c_str());
      mShaderNames.push_back(name);
    }
  }
}