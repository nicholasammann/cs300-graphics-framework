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