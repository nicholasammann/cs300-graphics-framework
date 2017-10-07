#include <string>

#include "../ImGui/imgui.h"

#include "../Graphics/Model.hpp"

#include "Application.hpp" 
#include "Editor.hpp"

ELBA::Editor::Editor(Application * aApp) : mApp(aApp)
{
}

void ELBA::Editor::Update()
{
  //ImGui::ShowTestWindow();

  ImGui::Begin("Test Window");

  if (ImGui::CollapsingHeader("Object Selection"))
  {
    // for each model in the scene
    auto models = mApp->GetModels();
    for (auto mod : models)
    {
      std::string name = mod->GetName();

      if (ImGui::TreeNode(name.c_str()))
      {

        if (ImGui::TreeNode("Transform"))
        {
          // do the things for the mesh thing
          Transform &trans = mod->GetTransform();

          // position
          float pos[3];
          pos[0] = trans.mWorldPos.x;
          pos[1] = trans.mWorldPos.y;
          pos[2] = trans.mWorldPos.z;
          ImGui::DragFloat3("Position", pos, 0.1);
          trans.mWorldPos.x = pos[0];
          trans.mWorldPos.y = pos[1];
          trans.mWorldPos.z = pos[2];

          // rotation
          float rot[3];
          rot[0] = trans.mWorldRot.x;
          rot[1] = trans.mWorldRot.y;
          rot[2] = trans.mWorldRot.z;
          ImGui::DragFloat3("Rotation", rot, 0.1);
          trans.mWorldRot.x = rot[0];
          trans.mWorldRot.y = rot[1];
          trans.mWorldRot.z = rot[2];

          float scale[3];
          scale[0] = trans.mScale.x;
          scale[1] = trans.mScale.y;
          scale[2] = trans.mScale.z;
          ImGui::DragFloat3("Scale", scale, 0.1);
          trans.mScale.x = scale[0];
          trans.mScale.y = scale[1];
          trans.mScale.z = scale[2];

          ImGui::TreePop();
        }
        
        // for each mesh on the model
        auto meshes = mod->GetMeshes();
        
        for (unsigned int i = 0; i < meshes.size(); ++i)
        {
          std::string meshLabel = "Submesh " + std::to_string(i);

          if (ImGui::TreeNode(meshLabel.c_str()))
          {
            const char *modes[] = { "None", "Vertex Normals", "Face Normals" };

            ImGui::PushItemWidth(130.0f);

            ImGui::Combo("Debug Mode", meshes[i]->GetDebugMode(), modes, sizeof(modes) / sizeof(char*));

            ImGui::PopItemWidth();

            ImGui::TreePop();
          }

        }

        ImGui::TreePop();
      }
      

    }

  }

  if (ImGui::CollapsingHeader("Model Loading"))
  {

    if (ImGui::Button("Load"))
    {
      // Load the model
    }

    ImGui::SameLine();

    char buffer[128] = { '\0' };
    ImGui::InputText("Model", buffer, sizeof(buffer)/sizeof(char));
  }

  if (ImGui::CollapsingHeader("Material"))
  {
    float ambient[4] = { 0.0f };
    ImGui::ColorEdit4("Ambient", ambient);

    float diffuse[4] = { 0.0f };
    ImGui::ColorEdit4("Diffuse", diffuse);
  }

  if (ImGui::CollapsingHeader("Lights"))
  {
    if (ImGui::TreeNode("Light 0"))
    {

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Light 1"))
    {

      ImGui::TreePop();
    }
  }

  ImGui::End();
}
