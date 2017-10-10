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
      
      ImGui::PushID(k);

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

      ImGui::PopID();


      // for each mesh on the model
      auto meshes = models[k]->GetMeshes();
      
      for (unsigned int i = 0; i < meshes.size(); ++i)
      {
        ImGui::PushID(k);
        std::string meshLabel = "Submesh " + std::to_string(i);

        if (ImGui::TreeNode(meshLabel.c_str()))
        {

          ImGui::PushID(k);
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
          ImGui::PopID();
          

          ImGui::PushID(k);

          if (ImGui::TreeNode("Material"))
          {
            ImGui::ColorEdit4("Ambient", meshes[i]->GetMaterial().ambient);

            ImGui::ColorEdit4("Diffuse", meshes[i]->GetMaterial().diffuse);

            ImGui::TreePop();
          }

          ImGui::PopID();

          ImGui::TreePop();
        }
        ImGui::PopID();

      }
    }
    ImGui::PopID();
  }

  

  if (ImGui::CollapsingHeader("Lights"))
  {

    for (unsigned int i = 0; i < mApp->GetLights().size(); ++i)
    {
      std::string lightLabel = "Light " + std::to_string(i) + '\0';
      
      if (ImGui::TreeNode(lightLabel.c_str()))
      {
        Light &light = mApp->GetLights()[i];

        ImGui::PushID(i);

        ImGui::DragFloat4("Direction", light.direction);

        if (ImGui::TreeNode("Ambient"))
        {
          ImGui::ColorPicker4("Ambient", light.ambient);
          ImGui::TreePop();
        }

        if (ImGui::TreeNode("Diffuse"))
        {
          ImGui::ColorPicker4("Diffuse", light.diffuse);
          ImGui::TreePop();
        }

        ImGui::PopID();

        ImGui::TreePop();
      }
    }
  }

  ImGui::End();
}
