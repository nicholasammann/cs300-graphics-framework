/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Application.cpp
Purpose: Main Window
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
 --------------------------------------------------------*/

#include <iostream>
#include <filesystem>
#include <exception>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"

#include "../Graphics/CubeMap.hpp"
#include "../Graphics/Model.hpp"
#include "../Graphics/NormalMap.hpp"
#include "../Graphics/Shader.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/Skybox.hpp"

#include "Application.hpp"
#include "Camera.hpp"
#include "Editor.hpp"
#include "Scene.hpp"

namespace ELBA
{
  Application::Application(GLFWwindow *aWindow)
    : mWindow(aWindow), mCamera(new Camera()),
      mEditor(new Editor(this)), mLightSpeed(1.5f),
      mBackgroundColor(0.15f, 0.15f, 0.15f, 1.0f),
      mRotateLights(true), mRotateCamera(false)
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

    UpdateLights();

    UpdateCamera();

    mEditor->Update();
    
    for (auto m : mModels)
    {
      if (m->mReflection || m->mRefraction)
      {
        m->UpdateEnvironmentMap();
      }
    }

    Render(mCamera, mWindowWidth, mWindowHeight, true);

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
    float camSpeed = 0.05f;

    if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
      camSpeed = 0.075f;
    }

    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(mWindow, true);
    }
    
    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
      mCamera->mPosition += camSpeed * mCamera->mDirection;
      mCamera->UpdateVectors();
    }

    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
      mCamera->mPosition -= camSpeed * mCamera->mDirection;
      mCamera->UpdateVectors();
    }

    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
      mCamera->mPosition -= camSpeed * mCamera->mCameraRight;
      mCamera->UpdateVectors();
    }

    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
      mCamera->mPosition += camSpeed * mCamera->mCameraRight;
      mCamera->UpdateVectors();
    }

    if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_PRESS)
    {
      mCamera->mPosition -= camSpeed * mCamera->mCameraUp;
      mCamera->UpdateVectors();
    }

    if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_PRESS)
    {
      mCamera->mPosition += camSpeed * mCamera->mCameraUp;
      mCamera->UpdateVectors();
    }
  }

  void Application::Render(Camera *aCamera, int aWidth, int aHeight, bool aUseClearColor)
  {
    aWidth = mWindowWidth;
    aHeight = mWindowHeight;

    // rendering commands
    if (aUseClearColor)
    {
      glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
      glViewport(0, 0, aWidth, aHeight);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSkybox->Draw();

    for (Model *m : mModels)
    {
      Shader *shdr = m->GetShader();

      if (!shdr)
      {
        shdr = this->GetShader("Simple");
      }

      unsigned int shdrPrg = shdr->GetShaderProgram();
      shdr->UseShaderProgram();

      BindLights(shdrPrg);

      //std::cout << "Pos: " << aCamera->mPosition.x << ", " << aCamera->mPosition.y << ", " << aCamera->mPosition.z << std::endl;

      glm::mat4 view = aCamera->ConstructViewMatrix();
      unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      glm::mat4 projection = aCamera->ConstructProjMatrix(aWidth, aHeight);
      unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

      glm::mat4 model = m->ConstructModelMatrix();
      unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      m->Draw(projection, view, model);
    }

    RenderLightModels();
  }

  void Application::CreateInitialShaders()
  {
    std::string p = "Assets/Shaders/";

    CreateShader("Simple", p+"simple.vert", p+"simple.frag");
    mShaderPaths.emplace_back(shdr_info("Simple", p+"simple.vert", p+"simple.frag"));

    CreateShader("Debug", p + "debug.vert", p + "debug.frag");
    mShaderPaths.emplace_back(shdr_info("Debug", p + "debug.vert", p + "debug.frag"));

    CreateShader("Phong Lighting", p+"phong_lighting.vert", p+"phong_lighting.frag");
    mShaderPaths.emplace_back(shdr_info("Phong Lighting", p+"phong_lighting.vert", p+"phong_lighting.frag"));

    CreateShader("Phong Shading", p + "phong_shading.vert", p + "phong_shading.frag");
    mShaderPaths.emplace_back(shdr_info("Phong Shading", p + "phong_shading.vert", p + "phong_shading.frag"));

    CreateShader("Blinn", p + "blinn.vert", p + "blinn.frag");
    mShaderPaths.emplace_back(shdr_info("Blinn", p + "blinn.vert", p + "blinn.frag"));

    CreateShader("Light Model", p + "light_model.vert", p + "light_model.frag");
    mShaderPaths.emplace_back(shdr_info("Light Model", p + "light_model.vert", p + "light_model.frag"));

    CreateShader("Skybox", p + "skybox.vert", p + "skybox.frag");
    mShaderPaths.emplace_back(shdr_info("Skybox", p + "skybox.vert", p + "skybox.frag"));
  }

  void Application::CreateInitialModels()
  {
    Model *mod = new Model(this, "../OpenGLFramework/Assets/Models/cube.obj", "Cube");
    mod->SetShader("Phong Shading");
    mod->GetTransform().mWorldPos.z = 0.0f;
    
    Texture *diffTex = new Texture("../OpenGLFramework/Assets/Textures/metal_roof_diff_512x512.tga");
    mod->mDiffuseTexture = diffTex;

    Texture *specTex = new Texture("../OpenGLFramework/Assets/Textures/metal_roof_spec_512x512.tga");
    mod->mSpecularTexture = specTex;
    
    mod->mMappingType = 0;
    mod->mUsingTextures = false;

    NormalMap *normTex = new NormalMap("../OpenGLFramework/Assets/Textures/metal_roof_spec_512x512.tga");
    mod->mNormalTexture = normTex;
    mod->mUsingNormalMap = false;

    mModels.push_back(mod);

    //Model *plane = new Model(this, "../OpenGLFramework/Assets/Models/plane.obj", "Plane");
    //plane->SetShader("Blinn");
    //plane->GetTransform().mWorldPos.y = -2.0f;
    //plane->GetTransform().mScale.x = 10.0f;
    //plane->GetTransform().mScale.z = 10.0f;
    //mModels.push_back(plane);

    int skyboxShader = GetShader("Skybox")->GetShaderProgram();

    mSkybox = new Skybox(this, skyboxShader);
    mSkybox->Build();
  }

  void Application::CreateInitialLights()
  {
    mLightUniforms.globalAmbient[0] = 0.2f;
    mLightUniforms.globalAmbient[1] = 0.2f;
    mLightUniforms.globalAmbient[2] = 0.2f;
    mLightUniforms.globalAmbient[3] = 1.0f;

    mLightUniforms.spotInnerAngle = 15.0f;
    mLightUniforms.spotOuterAngle = 30.0f;
    mLightUniforms.spotFalloff = 1.0f;

    mLightUniforms.fogColor[0] = mBackgroundColor.r;
    mLightUniforms.fogColor[1] = mBackgroundColor.g;
    mLightUniforms.fogColor[2] = mBackgroundColor.b;
    mLightUniforms.fogColor[3] = mBackgroundColor.a;

    mLightUniforms.fogNear = 12.5f;
    mLightUniforms.fogFar = 25.0f;

    Scene1::CreateLights(this, 0);
  }

  void Application::BindLights(unsigned int aShaderPrg)
  {

    GLuint loc = glGetUniformLocation(aShaderPrg, "globalAmbient");
    glUniform4fv(loc, 1, mLightUniforms.globalAmbient);

    // directional lights
    loc = glGetUniformLocation(aShaderPrg, "DirLightCount");
    glUniform1i(loc, mLightUniforms.DirLights.size());

    auto &dirs = mLightUniforms.DirLights;

    for (unsigned int i = 0; i < dirs.size(); ++i)
    {
      std::string lightName = "DirLights[" + std::to_string(i) + "].";

      std::string dir = lightName + "direction";
      std::string amb = lightName + "ambient";
      std::string dif = lightName + "diffuse";
      std::string spc = lightName + "specular";

      unsigned int loc = glGetUniformLocation(aShaderPrg, dir.c_str());
      glUniform4fv(loc, 1, dirs[i].direction);

      loc = glGetUniformLocation(aShaderPrg, amb.c_str());
      glUniform4fv(loc, 1, dirs[i].ambient);

      loc = glGetUniformLocation(aShaderPrg, dif.c_str());
      glUniform4fv(loc, 1, dirs[i].diffuse);

      loc = glGetUniformLocation(aShaderPrg, spc.c_str());
      glUniform4fv(loc, 1, dirs[i].specular);
    }

    // spot lights
    loc = glGetUniformLocation(aShaderPrg, "SpotLightCount");
    glUniform1i(loc, mLightUniforms.SpotLights.size());

    auto &spots = mLightUniforms.SpotLights;

    for (unsigned int i = 0; i < spots.size(); ++i)
    {
      std::string lightName = "SpotLights[" + std::to_string(i) + "].";

      std::string pos = lightName + "pos";
      std::string dir = lightName + "direction";
      std::string amb = lightName + "ambient";
      std::string dif = lightName + "diffuse";
      std::string spc = lightName + "specular";
      std::string dc1 = lightName + "c1";
      std::string dc2 = lightName + "c2";
      std::string dc3 = lightName + "c3";

      loc = glGetUniformLocation(aShaderPrg, pos.c_str());
      glUniform4fv(loc, 1, spots[i].pos);

      loc = glGetUniformLocation(aShaderPrg, dir.c_str());
      glUniform4fv(loc, 1, spots[i].direction);

      loc = glGetUniformLocation(aShaderPrg, amb.c_str());
      glUniform4fv(loc, 1, spots[i].ambient);

      loc = glGetUniformLocation(aShaderPrg, dif.c_str());
      glUniform4fv(loc, 1, spots[i].diffuse);

      loc = glGetUniformLocation(aShaderPrg, spc.c_str());
      glUniform4fv(loc, 1, spots[i].specular);

      loc = glGetUniformLocation(aShaderPrg, dc1.c_str());
      glUniform1f(loc, spots[i].c1);

      loc = glGetUniformLocation(aShaderPrg, dc2.c_str());
      glUniform1f(loc, spots[i].c2);

      loc = glGetUniformLocation(aShaderPrg, dc3.c_str());
      glUniform1f(loc, spots[i].c3);
    }


    // point lights
    loc = glGetUniformLocation(aShaderPrg, "PointLightCount");
    glUniform1i(loc, mLightUniforms.PointLights.size());

    auto &points = mLightUniforms.PointLights;

    for (unsigned int i = 0; i < points.size(); ++i)
    {
      std::string lightName = "PointLights[" + std::to_string(i) + "].";

      std::string pos = lightName + "pos";
      std::string amb = lightName + "ambient";
      std::string dif = lightName + "diffuse";
      std::string spc = lightName + "specular";
      std::string dc1 = lightName + "c1";
      std::string dc2 = lightName + "c2";
      std::string dc3 = lightName + "c3";

      loc = glGetUniformLocation(aShaderPrg, pos.c_str());
      glUniform4fv(loc, 1, points[i].pos);

      loc = glGetUniformLocation(aShaderPrg, amb.c_str());
      glUniform4fv(loc, 1, points[i].ambient);

      loc = glGetUniformLocation(aShaderPrg, dif.c_str());
      glUniform4fv(loc, 1, points[i].diffuse);

      loc = glGetUniformLocation(aShaderPrg, spc.c_str());
      glUniform4fv(loc, 1, points[i].specular);

      loc = glGetUniformLocation(aShaderPrg, dc1.c_str());
      glUniform1f(loc, points[i].c1);

      loc = glGetUniformLocation(aShaderPrg, dc2.c_str());
      glUniform1f(loc, points[i].c2);

      loc = glGetUniformLocation(aShaderPrg, dc3.c_str());
      glUniform1f(loc, points[i].c3);
    }


    loc = glGetUniformLocation(aShaderPrg, "spotInnerAngle");
    glUniform1f(loc, mLightUniforms.spotInnerAngle);

    loc = glGetUniformLocation(aShaderPrg, "spotOuterAngle");
    glUniform1f(loc, mLightUniforms.spotOuterAngle);

    loc = glGetUniformLocation(aShaderPrg, "spotFalloff");
    glUniform1f(loc, mLightUniforms.spotFalloff);

    loc = glGetUniformLocation(aShaderPrg, "fogColor");
    glUniform4fv(loc, 1, mLightUniforms.fogColor);

    loc = glGetUniformLocation(aShaderPrg, "fogNear");
    glUniform1f(loc, mLightUniforms.fogNear);

    loc = glGetUniformLocation(aShaderPrg, "fogFar");
    glUniform1f(loc, mLightUniforms.fogFar);

  }

  void Application::UpdateLights()
  {
    int totalLights = mLightUniforms.SpotLights.size();
    totalLights += mLightUniforms.PointLights.size();
    totalLights += mLightUniforms.DirLights.size();

    float offset = (2 * 3.1416f) / totalLights;

    int counter = 0;

    auto &objPos = mModels[0]->GetTransform().mWorldPos;

    vec4 initPos = vec4(3, 0, 0, 1);

    // for each spot light
    for (auto &s : mLightUniforms.SpotLights)
    { 
      // initial rotation
      mat4 initRot = rotate(mat4(), static_cast<float>(counter * offset), vec3(0, 1, 0));

      // rotation based on time
      float angle = mLightSpeed * static_cast<float>(glfwGetTime());
      mat4 rot = rotate(mat4(), angle, vec3(0, 1, 0));

      // apply matrices to initial position
      vec4 pos = rot * initRot * initPos;

      vec3 mpos = mModels[0]->GetTransform().mWorldPos;
      vec3 dir = mpos - vec3(pos.x, pos.y, pos.z);

      if (mRotateLights)
      {
        // update the position and direction
        s.SetDirection(dir.x, dir.y, dir.z, 0.0f);
        s.SetPos(pos.x, pos.y, pos.z, 1);
      }

      s.model->GetMeshes()[0]->GetMaterial().SetAmbient(s.diffuse[0], s.diffuse[1], s.diffuse[2], s.diffuse[3]);

      counter++;
    }


    // for each point light
    for (auto &s : mLightUniforms.PointLights)
    {
      // initial rotation
      mat4 initRot = rotate(mat4(), static_cast<float>(counter * offset), vec3(0, 1, 0));

      // rotation based on time
      float angle = mLightSpeed * static_cast<float>(glfwGetTime());
      mat4 rot = rotate(mat4(), angle, vec3(0, 1, 0));

      // apply matrices to initial position
      vec4 pos = rot * initRot * initPos;

      vec3 mpos = mModels[0]->GetTransform().mWorldPos;
      vec3 dir = mpos - vec3(pos.x, pos.y, pos.z);

      if (mRotateLights)
      {
        // update the position
        s.SetPos(pos.x, pos.y, pos.z, 1);
      }

      s.model->GetMeshes()[0]->GetMaterial().SetAmbient(s.diffuse[0], s.diffuse[1], s.diffuse[2], s.diffuse[3]);

      counter++;
    }


    // for each directional light
    for (auto &s : mLightUniforms.DirLights)
    {
      // initial rotation
      mat4 initRot = rotate(mat4(), static_cast<float>(counter * offset), vec3(0, 1, 0));

      // rotation based on time
      float angle = mLightSpeed * static_cast<float>(glfwGetTime());
      mat4 rot = rotate(mat4(), angle, vec3(0, 1, 0));

      // apply matrices to initial position
      vec4 pos = rot * initRot * initPos;

      vec3 mpos = mModels[0]->GetTransform().mWorldPos;
      vec3 dir = mpos - vec3(pos.x, pos.y, pos.z);

      if (mRotateLights)
      {
        // update the model position & direction
        s.SetModelPos(pos.x, pos.y, pos.z, 1);
        s.SetDirection(dir.x, dir.y, dir.z, 0);
      }


      s.model->GetMeshes()[0]->GetMaterial().SetAmbient(s.diffuse[0], s.diffuse[1], s.diffuse[2], s.diffuse[3]);

      counter++;
    }

  }

  void Application::RenderLightModels()
  {
    for (auto &light : mLightUniforms.SpotLights)
    {
      auto &m = light.model;

      auto shdrPrg = m->GetShader()->GetShaderProgram();
      m->GetShader()->UseShaderProgram();
      
      glm::mat4 view = mCamera->ConstructViewMatrix();
      unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      glm::mat4 projection = mCamera->ConstructProjMatrix(mWindowWidth, mWindowHeight);
      unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

      glm::mat4 model = m->ConstructModelMatrix();
      unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      m->Draw(projection, view, model);
    }

    for (auto &light : mLightUniforms.PointLights)
    {
      auto &m = light.model;

      auto shdrPrg = m->GetShader()->GetShaderProgram();
      m->GetShader()->UseShaderProgram();

      glm::mat4 view = mCamera->ConstructViewMatrix();
      unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      glm::mat4 projection = mCamera->ConstructProjMatrix(mWindowWidth, mWindowHeight);
      unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

      glm::mat4 model = m->ConstructModelMatrix();
      unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      m->Draw(projection, view, model);
    }

    for (auto &light : mLightUniforms.DirLights)
    {
      auto &m = light.model;

      auto shdrPrg = m->GetShader()->GetShaderProgram();
      m->GetShader()->UseShaderProgram();

      glm::mat4 view = mCamera->ConstructViewMatrix();
      unsigned int viewLoc = glGetUniformLocation(shdrPrg, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      glm::mat4 projection = mCamera->ConstructProjMatrix(mWindowWidth, mWindowHeight);
      unsigned int projLoc = glGetUniformLocation(shdrPrg, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

      glm::mat4 model = m->ConstructModelMatrix();
      unsigned int modelLoc = glGetUniformLocation(shdrPrg, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      m->Draw(projection, view, model);
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

  LightUniforms & Application::GetLightUniforms()
  {
    return mLightUniforms;
  }

  float* Application::GetLightSpeed()
  {
    return &mLightSpeed;
  }

  void Application::UpdateCamera()
  {
    vec4 initPos = vec4(10, 0.75f * cos(glfwGetTime()), 0, 1);

    // rotation based on time
    float angle = 0.75f * static_cast<float>(glfwGetTime());
    mat4 rot = rotate(mat4(), angle, vec3(0, 1, 0));

    // apply matrices to initial position
    vec4 pos = rot * initPos;

    vec3 mpos = mModels[0]->GetTransform().mWorldPos;
    vec3 dir = mpos - vec3(pos.x, pos.y, pos.z);

    if (mRotateCamera)
    {
      mCamera->SetPosition(pos);
      mCamera->SetTargetPoint(mModels[0]->GetTransform().mWorldPos);
    }
    else
    {
      //mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
      //mCamera->SetTargetPoint(mModels[0]->GetTransform().mWorldPos);
    }
  }
}