/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Editor.hpp
Purpose: Editor
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#pragma once

namespace ELBA
{

  class Application;

  class Editor
  {
  public:

    Editor(Application *aApp);
    
    void Update();

    void PrintToConsole(std::string message);

  private:

    Application *mApp;

    char mLoadBuffer[256];

    std::vector<std::string> mConsoleLog;

    int mNumLights;

  };

}