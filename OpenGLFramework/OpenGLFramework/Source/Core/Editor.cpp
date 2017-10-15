#include <string>

#include "../ImGui/imgui.h"


#include "Application.hpp" 
#include "Editor.hpp"


namespace elba
{

  Editor::Editor(Application * aApp) : mApp(aApp)
  {
  }

  void Editor::Update()
  {
    ImGui::Begin("Start");
    ImGui::End();
  }

}