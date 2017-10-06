#include "../ImGui/imgui.h"

#include "Editor.hpp"

ELBA::Editor::Editor(Application * aApp)
{
}

void ELBA::Editor::Update()
{
  ImGui::Begin("Test Window");

  ImGui::Text("HEY FAMB");

  ImGui::End();
}
