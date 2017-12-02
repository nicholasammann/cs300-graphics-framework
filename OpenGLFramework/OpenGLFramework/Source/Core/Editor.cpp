/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Editor.cpp
Purpose: Editor
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#include <string>

#include "../ImGui/imgui.h"

#include "../Graphics/Model.hpp"

#include "Application.hpp" 
#include "Editor.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/NormalMap.hpp"

#include "Scene.hpp"

vec3 lightSize = vec3(0.25f, 0.25f, 0.25f);

ELBA::Editor::Editor(Application * aApp) : mApp(aApp), mLoadBuffer{ '\0' }, mNumLights(8)
{
}

void ELBA::Editor::Update()
{
  ImGui::Begin("Test Window");

  // for each model in the scene
  auto models = mApp->GetModels();
  for (unsigned int k = 0; k < models.size(); ++k)
  {
    std::string name = models[k]->GetName();

    ImGui::PushID(k);

    if (ImGui::CollapsingHeader(name.c_str()))
    {

      if (ImGui::TreeNode("Transform"))
      {
        ImGui::PushItemWidth(150.0f);

        // do the things for the mesh thing
        Transform &trans = models[k]->GetTransform();

        // position
        float pos[3];
        pos[0] = trans.mWorldPos.x;
        pos[1] = trans.mWorldPos.y;
        pos[2] = trans.mWorldPos.z;
        ImGui::DragFloat3("Position", pos, 0.1f);
        trans.mWorldPos.x = pos[0];
        trans.mWorldPos.y = pos[1];
        trans.mWorldPos.z = pos[2];

        // rotation
        float rot[3];
        rot[0] = trans.mWorldRot.x;
        rot[1] = trans.mWorldRot.y;
        rot[2] = trans.mWorldRot.z;
        ImGui::DragFloat3("Rotation", rot, 0.1f);
        trans.mWorldRot.x = rot[0];
        trans.mWorldRot.y = rot[1];
        trans.mWorldRot.z = rot[2];

        float scale[3];
        scale[0] = trans.mScale.x;
        scale[1] = trans.mScale.y;
        scale[2] = trans.mScale.z;
        ImGui::DragFloat3("Scale", scale, 0.1f);
        trans.mScale.x = scale[0];
        trans.mScale.y = scale[1];
        trans.mScale.z = scale[2];

        ImGui::PopItemWidth();

        ImGui::TreePop();
      }


      if (ImGui::TreeNode("Shader"))
      {
        // display name of current shader
        // ImGui::Text("Current Shader : ");
        // ImGui::SameLine();
        // ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), models[k]->GetShaderName().c_str());
        
        // build list of shader names

        //for (auto s : mApp->GetShaderMap())
        //{
        //  char *name = new char[512];
        //  strcpy(name, s.second->GetName().c_str());
        //  mApp.push_back(name);
        //}

        ImGui::PushItemWidth(130.0f);
        ImGui::Combo("Current",
                     &models[k]->mCurrentShaderSelect, 
                     mApp->mShaderNames.Data,
                     mApp->mShaderNames.size(),
                     mApp->mShaderNames.size());
        ImGui::PopItemWidth();

        if (models[k]->mCurrentShaderSelect != models[k]->mPrevShaderSelect)
        {
          std::string nS = mApp->mShaderNames[models[k]->mCurrentShaderSelect];
          models[k]->SetShader(nS);
        }

        // THIS IS FOR RELOADING SHADERS INDIVIDUALLY - NOT FUNCTIONAL

        //if (ImGui::Button("Reload Shader"))
        //{
        //  std::string nS = mApp->mShaderNames[models[k]->mCurrentShaderSelect];
        //
        //  Shader *oS = mApp->GetShader(nS.c_str());
        //
        //  std::string vPath = oS->GetVertPath();
        //  std::string fPath = oS->GetFragPath();
        //
        //  mApp->CreateShader(nS.c_str(), vPath.c_str(), fPath.c_str());
        //
        //  models[k]->SetShader(nS);
        //}

        ImGui::TreePop();
      }


      if (models[k]->mDiffuseTexture || models[k]->mSpecularTexture || models[k]->mNormalTexture)
      {
        if (ImGui::TreeNode("Textures"))
        {

          static const char *mapTypes[3] = { "Planar", "Cylindrical", "Spherical" };

          ImGui::PushItemWidth(130.0f);
          ImGui::Combo("Texture Mapping", &models[k]->mMappingType, mapTypes, 3);
          ImGui::PopItemWidth();

          if (models[k]->mDiffuseTexture || models[k]->mSpecularTexture)
          {
            ImGui::PushItemWidth(75.0f);
            ImGui::Checkbox("Display Textures", &models[k]->mUsingTextures);
            ImGui::PopItemWidth();
          }

          if (models[k]->mNormalTexture)
          {
            ImGui::PushItemWidth(75.0f);
            ImGui::Checkbox("Use Normal Map", &models[k]->mUsingNormalMap);
            ImGui::PopItemWidth();
          }

          ImGui::TreePop();
        }
      }


      if (ImGui::TreeNode("Environment Mappping"))
      {
        ImGui::Checkbox("Use Reflection", &models[k]->mReflection);
        ImGui::Checkbox("Use Refraction", &models[k]->mRefraction);

        ImGui::TreePop();
      }


      // for each mesh on the model
      auto meshes = models[k]->GetMeshes();
      
      //for (unsigned int i = 0; i < meshes.size(); ++i)
      //{
      //  std::string meshLabel = "Submesh " + std::to_string(i);
      //
      //  if (ImGui::TreeNode(meshLabel.c_str()))
      //  {

          if (ImGui::TreeNode("Debug Options"))
          {

            ImGui::PushItemWidth(130.0f);

            static const char *colorModes[] = { "Standard", "Tangent", "Bitangent", "Normal" };
            ImGui::Combo("Color Mode", meshes[0]->GetDebugColorMode(), colorModes, sizeof(colorModes) / sizeof(char*));

            static const char *lineModes[] = { "None", "Vertex Normals", "Face Normals", "TBN Frame" };
            ImGui::Combo("Line Mode", meshes[0]->GetDebugLineMode(), lineModes, sizeof(lineModes) / sizeof(char*));

            ImGui::SliderFloat("Line Width", &meshes[0]->GetDebugLineWidth(), 0.01f, 3.0f);

            ImGui::SliderFloat("Line Length", &meshes[0]->GetDebugLineLength(), 0.001f, 3.0f);

            ImGui::PopItemWidth();

            ImGui::TreePop();
          }

          if (ImGui::TreeNode("Material"))
          {
            ImGui::PushItemWidth(200.0f);
            ImGui::ColorEdit4("Ambient", meshes[0]->GetMaterial().ambient);

            ImGui::ColorEdit4("Diffuse", meshes[0]->GetMaterial().diffuse);

            ImGui::ColorEdit4("Specular", meshes[0]->GetMaterial().specular);
           
            ImGui::ColorEdit4("Emissive", meshes[0]->GetMaterial().emissive);
            ImGui::PopItemWidth();

            ImGui::PushItemWidth(100.0f);
            ImGui::DragFloat("Shininess", &meshes[0]->GetMaterial().shininess, 0.01f, 0.0f, 50.0f);
            ImGui::PopItemWidth();

            ImGui::TreePop();
          }

      //   ImGui::TreePop();
      // }
      //}
    }

    ImGui::PopID();
  }


  if (ImGui::CollapsingHeader("Lights"))
  {

    auto& lunis = mApp->GetLightUniforms();

    if (ImGui::TreeNode("Constants"))
    {
      if (ImGui::TreeNode("Spotlight Constants"))
      {
        ImGui::PushItemWidth(150.0f);
        ImGui::SliderFloat("Inner Angle", &lunis.spotInnerAngle, 0.0f, 90.0f);
        ImGui::SliderFloat("Outer Angle", &lunis.spotOuterAngle, 0.0f, 90.0f);
        ImGui::DragFloat("Falloff", &lunis.spotFalloff, 0.01f, 0.0f, 10.0f);
        ImGui::PopItemWidth();

        ImGui::TreePop();
      }


      if (ImGui::TreeNode("Global Colors"))
      {
        ImGui::PushItemWidth(200.0f);
        ImGui::ColorEdit4("Ambient", lunis.globalAmbient);
        ImGui::ColorEdit4("Fog", lunis.fogColor);
        ImGui::PopItemWidth();

        ImGui::TreePop();
      }


      if (ImGui::TreeNode("Misc."))
      {
        ImGui::PushItemWidth(50.0f);
        ImGui::DragFloat("Rotation Speed", mApp->GetLightSpeed(), 0.1f, 0.0f, 0.0f, "%.1f");
        ImGui::PopItemWidth();

        ImGui::TreePop();
      }


      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Directional Lights"))
    {
      ImGui::PushID("DirLight");

      for (unsigned int i = 0; i < lunis.DirLights.size(); ++i)
      {
        std::string lightLabel = "Dir Light " + std::to_string(i) + '\0';

        if (ImGui::TreeNode(lightLabel.c_str()))
        {
          ImGui::PushID(i);

          ImGui::PushItemWidth(200.0f);
          DirLight &light = lunis.DirLights[i];
          ImGui::DragFloat3("Direction", light.direction, 0.01f, -1.0f, 1.0f);
          ImGui::ColorEdit4("Ambient", light.ambient);
          ImGui::ColorEdit4("Diffuse", light.diffuse);
          ImGui::ColorEdit4("Specular", light.specular);
          ImGui::PopItemWidth();

          ImGui::PopID();
          ImGui::TreePop();
        }
      }
      ImGui::PopID();
      ImGui::TreePop();
    }


    if (ImGui::TreeNode("Spot Lights"))
    {
      ImGui::PushID("SpotLight");

      for (unsigned int i = 0; i < lunis.SpotLights.size(); ++i)
      {
        std::string lightLabel = "Spot Light " + std::to_string(i) + '\0';

        if (ImGui::TreeNode(lightLabel.c_str()))
        {
          ImGui::PushID(i);

          SpotLight &light = lunis.SpotLights[i];

          ImGui::PushItemWidth(200.0f);
          ImGui::DragFloat3("Position", light.pos, 0.01f);
          light.SetPos(light.pos[0], light.pos[1], light.pos[2], light.pos[3]);

          ImGui::DragFloat3("Direction", light.direction, 0.001f, -1.0f, 1.0f);
          ImGui::ColorEdit4("Ambient", light.ambient);
          ImGui::ColorEdit4("Diffuse", light.diffuse);
          ImGui::ColorEdit4("Specular", light.specular);
          ImGui::PopItemWidth();

          if (ImGui::TreeNode("Attenuation Constants"))
          {
            ImGui::PushItemWidth(75.0f);
            ImGui::DragFloat("C1", &light.c1, 0.001f, 0.0f, 5.0f);
            ImGui::DragFloat("C2", &light.c2, 0.001f, 0.0f, 5.0f);
            ImGui::DragFloat("C3", &light.c3, 0.001f, 0.0f, 5.0f);
            ImGui::PopItemWidth();

            ImGui::TreePop();
          }

          ImGui::PopID();
          ImGui::TreePop();
        }
      }
      ImGui::PopID();
      ImGui::TreePop();
    }


    if (ImGui::TreeNode("Point Lights"))
    {
      ImGui::PushID("PointLight");

      for (unsigned int i = 0; i < lunis.PointLights.size(); ++i)
      {
        std::string lightLabel = "Point Light " + std::to_string(i) + '\0';

        if (ImGui::TreeNode(lightLabel.c_str()))
        {
          ImGui::PushID(i);

          PointLight &light = lunis.PointLights[i];
          light.SetPos(light.pos[0], light.pos[1], light.pos[2], light.pos[3]);

          ImGui::PushItemWidth(200.0f);
          ImGui::DragFloat3("Position", light.pos, 0.01f);
          ImGui::ColorEdit4("Ambient", light.ambient);
          ImGui::ColorEdit4("Diffuse", light.diffuse);
          ImGui::ColorEdit4("Specular", light.specular);
          ImGui::PopItemWidth();

          if (ImGui::TreeNode("Attenuation Constants"))
          {
            ImGui::PushItemWidth(75.0f);
            ImGui::DragFloat("C1", &light.c1, 0.001f, 0.0f, 5.0f);
            ImGui::DragFloat("C2", &light.c2, 0.001f, 0.0f, 5.0f);
            ImGui::DragFloat("C3", &light.c3, 0.001f, 0.0f, 5.0f);
            ImGui::PopItemWidth();

            ImGui::TreePop();
          }

          ImGui::PopID();
          ImGui::TreePop();
        }
      }
      ImGui::PopID();
      ImGui::TreePop();
    }
  }



  if (ImGui::CollapsingHeader("Scene"))
  {
    static int mCurrentScene = 0;
    static int mLightType = 0;

    static const char *lightTypes[] = { "Directional", "Spotlight", "Point" };

    static const char *scenes[] = { "Scene 1", "Scene 2", "Scene 3" };

    ImGui::PushItemWidth(130.0f);
    if (ImGui::Combo("Current", &mCurrentScene, scenes, 3))
    {
      switch (mCurrentScene)
      {
      case 0:
      {
        Scene1::CreateLights(mApp, mLightType);
        break;
      }

      case 1:
      {
        Scene2::CreateLights(mApp);
        break;
      }

      case 2:
      {
        Scene3::CreateLights(mApp);
        break;
      }
      }
    }


    ImGui::Checkbox("Toggle Camera Rotation", &mApp->mRotateCamera);

    if (mCurrentScene == 0 || mCurrentScene == 1)
    {
      ImGui::Checkbox("Toggle Light Rotation", &mApp->mRotateLights);
    }
    


    if (mCurrentScene == 0)
    {
      if (ImGui::Combo("Light Type", &mLightType, lightTypes, 3))
      {
        switch (mCurrentScene)
        {
        case 0:
        {
          Scene1::CreateLights(mApp, mLightType);
          mNumLights = 8;
          break;
        }

        case 1:
        {
          Scene2::CreateLights(mApp);
          break;
        }

        case 2:
        {
          Scene3::CreateLights(mApp);
          break;
        }
        }
      }

      if (ImGui::SliderInt("Light Count", &mNumLights, 0, 20))
      {
        ImGui::PopItemWidth();

        auto &lunis = mApp->GetLightUniforms();

        switch (mLightType)
        {
        case Directional:
        {
          while ((unsigned int)mNumLights > lunis.DirLights.size())
          {
            Model *mod = new Model(mApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
            mod->SetShader("Light Model");
            mod->GetTransform().mScale = lightSize;

            DirLight light;
            light.model = mod;
            light.SetModelPos(0, 0, 5, 1);
            light.SetDirection(0, 0, 0, 0);
            light.SetAmbient(0, 0, 0, 1);
            light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
            light.SetSpecular(1.0f, 0.4f, 1.0f, 1);

            Material &mat = mod->GetMeshes()[0]->GetMaterial();
            mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

            lunis.DirLights.push_back(light);
          }

          while ((unsigned int)mNumLights < lunis.DirLights.size())
          {
            lunis.DirLights.pop_back();
          }

          break;
        }

        case Spot:
        {
          while ((unsigned int)mNumLights > lunis.SpotLights.size())
          {
            Model *mod = new Model(mApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
            mod->SetShader("Light Model");
            mod->GetTransform().mScale = lightSize;

            SpotLight light;
            light.model = mod;
            light.SetPos(0, 0, 5, 1);
            light.SetDirection(0, 0, 0, 0);
            light.SetAmbient(0, 0, 0, 1);
            light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
            light.SetSpecular(1.0f, 0.4f, 1.0f, 1);

            Material &mat = mod->GetMeshes()[0]->GetMaterial();
            mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

            lunis.SpotLights.push_back(light);
          }

          while ((unsigned int)mNumLights < lunis.SpotLights.size())
          {
            lunis.SpotLights.pop_back();
          }
          break;
        }

        case Point:
        {
          while ((unsigned int)mNumLights > lunis.PointLights.size())
          {
            Model *mod = new Model(mApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
            mod->SetShader("Light Model");
            mod->GetTransform().mScale = lightSize;

            PointLight light;
            light.model = mod;
            light.SetPos(0, 0, 5, 1);
            light.SetAmbient(0, 0, 0, 1);
            light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
            light.SetSpecular(1.0f, 0.4f, 1.0f, 1);

            Material &mat = mod->GetMeshes()[0]->GetMaterial();
            mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

            lunis.PointLights.push_back(light);
          }

          while ((unsigned int)mNumLights < lunis.PointLights.size())
          {
            lunis.PointLights.pop_back();
          }
          break;
        }
        }
      }

    }

  }


  if (ImGui::CollapsingHeader("Actions"))
  {
    if (ImGui::Button("Load Model"))
    {
      std::string path = "../OpenGLFramework/Assets/Models/";
      path += mLoadBuffer;

      Model *mod = new Model(mApp, path.data(), mLoadBuffer);
      mod->SetShader("Phong Shading");

      Texture *diffTex = new Texture("../OpenGLFramework/Assets/Textures/metal_roof_diff_512x512.tga");
      mod->mDiffuseTexture = diffTex;

      Texture *specTex = new Texture("../OpenGLFramework/Assets/Textures/metal_roof_spec_512x512.tga");
      mod->mSpecularTexture = specTex;
      
      mod->mMappingType = 1;
      mod->mUsingTextures = 1;

      NormalMap *normTex = new NormalMap("../OpenGLFramework/Assets/Textures/metal_roof_spec_512x512.tga");
      mod->mNormalTexture = normTex;
      mod->mUsingNormalMap = true;

      mApp->GetModels().clear();
      mApp->GetModels().push_back(mod);

      //Model *plane = new Model(mApp, "../OpenGLFramework/Assets/Models/plane.obj", "Plane");
      //plane->SetShader("Blinn");
      //plane->GetTransform().mWorldPos.y = -2.0f;
      //plane->GetTransform().mScale.x = 10.0f;
      //plane->GetTransform().mScale.z = 10.0f;
      //mApp->GetModels().push_back(plane);
    }

    ImGui::SameLine();

    ImGui::PushItemWidth(150.0f);
    ImGui::InputText("  ", mLoadBuffer, sizeof(mLoadBuffer) / sizeof(char));
    ImGui::PopItemWidth();

    if (ImGui::Button("Reload Shaders"))
    {
      std::string name;
      std::string vertPath;
      std::string fragPath;

      //std::vector<std::tuple<std::string, std::string, std::string>> shaderStrs;
      //
      //for (auto it_s = mApp->GetShaderMap().begin(); it_s != mApp->GetShaderMap().end(); it_s++)
      //{
      //  name = it_s->second->GetName();
      //  vertPath = it_s->second->GetVertPath();
      //  fragPath = it_s->second->GetFragPath();
      //
      //  shaderStrs.emplace_back(name, vertPath, fragPath);
      //}

      // erase all current shaders
      mApp->GetShaderMap().clear();

      for (auto t : mApp->GetShaderPaths())
      {
        name = std::get<0>(t);
        vertPath = std::get<1>(t);
        fragPath = std::get<2>(t);
        mApp->CreateShader(name.c_str(), vertPath.c_str(), fragPath.c_str());
      }
      
      for (auto m : mApp->GetModels())
      {
        name = m->GetShaderName();
        m->SetShader(name);

        m->SetDebugShader();
      }

      mApp->ReloadShaderNamesForEditor();

    }

    if (ImGui::Button("Clear Console"))
    {
      mConsoleLog.clear();
    }

    
  }

  ImGui::End();

  // Console

  ImGui::Begin("Debug Console");

  for (std::string line : mConsoleLog)
  {
    ImGui::TextWrapped(line.c_str());
  }

  float scrollMax = ImGui::GetScrollMaxY();

  ImGui::SetScrollY(scrollMax);

  ImGui::End();
  
}

void ELBA::Editor::PrintToConsole(std::string message)
{
  mConsoleLog.push_back(message);
}
