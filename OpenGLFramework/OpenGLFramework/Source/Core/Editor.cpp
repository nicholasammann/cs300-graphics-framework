#include <string>

#include "../ImGui/imgui.h"

#include "../Graphics/Model.hpp"

#include "Application.hpp" 
#include "Editor.hpp"

ELBA::Editor::Editor(Application * aApp) : mApp(aApp), mLoadBuffer{ '\0' }
{
}

void ELBA::Editor::Update()
{
  ImGui::Begin("Test Window");
  
  if (ImGui::CollapsingHeader("Model Loading"))
  {

    if (ImGui::Button("Load"))
    {
      std::string path = "../OpenGLFramework/Assets/CS300/";
      path += mLoadBuffer;

      Model *mod = new Model(mApp, path.data(), mLoadBuffer);
      mod->SetShader("Shader");

      mApp->GetModels().push_back(mod);
    }

    ImGui::SameLine();

    ImGui::InputText("Model", mLoadBuffer, sizeof(mLoadBuffer) / sizeof(char));
  }

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

        ImGui::TreePop();
      }


      if (ImGui::TreeNode("Shaders"))
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

        ImGui::Text("Current: ");
        ImGui::SameLine();
        ImGui::Combo(" ", 
                     &models[k]->mCurrentShaderSelect, 
                     mApp->mShaderNames.Data,
                     mApp->mShaderNames.size(),
                     mApp->mShaderNames.size());

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


      // for each mesh on the model
      auto meshes = models[k]->GetMeshes();
      
      for (unsigned int i = 0; i < meshes.size(); ++i)
      {
        std::string meshLabel = "Submesh " + std::to_string(i);

        if (ImGui::TreeNode(meshLabel.c_str()))
        {

          if (ImGui::TreeNode("Debug"))
          {
            const char *modes[] = { "None", "Vertex Normals", "Face Normals" };

            ImGui::PushItemWidth(130.0f);

            ImGui::Combo("Mode", meshes[i]->GetDebugMode(), modes, sizeof(modes) / sizeof(char*));

            ImGui::PopItemWidth();

            ImGui::SliderFloat("Line Width", &meshes[i]->GetDebugLineWidth(), 1.0f, 3.0f);

            ImGui::SliderFloat("Line Length", &meshes[i]->GetDebugLineLength(), 0.1f, 3.0f);

            ImGui::TreePop();
          }

          if (ImGui::TreeNode("Material"))
          {
            ImGui::ColorEdit4("Ambient", meshes[i]->GetMaterial().ambient);

            ImGui::ColorEdit4("Diffuse", meshes[i]->GetMaterial().diffuse);

            ImGui::ColorEdit4("Specular", meshes[i]->GetMaterial().specular);
           
            ImGui::ColorEdit4("Emissive", meshes[i]->GetMaterial().emissive);

            ImGui::DragFloat("Shininess", &meshes[i]->GetMaterial().shininess, 0.01f, 0.0f, 50.0f);

            ImGui::TreePop();
          }

          ImGui::TreePop();
        }
      }
    }

    ImGui::PopID();
  }

  

  if (ImGui::CollapsingHeader("Lights"))
  {

    auto& lunis = mApp->GetLightUniforms();

    ImGui::ColorEdit4("Global Ambient", lunis.globalAmbient);

    if (ImGui::TreeNode("Spotlight Constants"))
    {
      ImGui::SliderFloat("Inner Angle", &lunis.spotInnerAngle, 0.0f, 90.0f);
      ImGui::SliderFloat("Outer Angle", &lunis.spotOuterAngle, 0.0f, 90.0f);
      ImGui::DragFloat("Falloff", &lunis.spotFalloff, 0.01f, 0.0f, 10.0f);
      ImGui::TreePop();
    }
    

    if (ImGui::TreeNode("Attenuation Constants"))
    {
      ImGui::DragFloat("C1", &lunis.c1, 0.001f, 0.0f, 5.0f);
      ImGui::DragFloat("C2", &lunis.c2, 0.001f, 0.0f, 5.0f);
      ImGui::DragFloat("C3", &lunis.c3, 0.001f, 0.0f, 5.0f);
      ImGui::TreePop();
    }
    
    ImGui::ColorEdit4("Fog Color", lunis.fogColor);



    if (ImGui::TreeNode("Directional Lights"))
    {
      ImGui::PushID("DirLight");

      for (unsigned int i = 0; i < lunis.DirLights.size(); ++i)
      {
        std::string lightLabel = "Dir Light " + std::to_string(i) + '\0';

        if (ImGui::TreeNode(lightLabel.c_str()))
        {
          ImGui::PushID(i);

          DirLight &light = lunis.DirLights[i];
          ImGui::DragFloat3("Direction", light.direction, 0.001f, -1.0f, 1.0f);
          ImGui::ColorEdit4("Ambient", light.ambient);
          ImGui::ColorEdit4("Diffuse", light.diffuse);
          ImGui::ColorEdit4("Specular", light.specular);
          
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
          ImGui::DragFloat3("Position", light.pos, 0.001f);
          ImGui::DragFloat3("Direction", light.direction, 0.001f, -1.0f, 1.0f);
          ImGui::ColorEdit4("Ambient", light.ambient);
          ImGui::ColorEdit4("Diffuse", light.diffuse);
          ImGui::ColorEdit4("Specular", light.specular);

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
          ImGui::DragFloat3("Position", light.pos, 0.1f);
          ImGui::ColorEdit4("Ambient", light.ambient);
          ImGui::ColorEdit4("Diffuse", light.diffuse);
          ImGui::ColorEdit4("Specular", light.specular);

          ImGui::PopID();
          ImGui::TreePop();
        }
      }
      ImGui::PopID();
      ImGui::TreePop();
    }


  }


  if (ImGui::CollapsingHeader("Actions"))
  {
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
