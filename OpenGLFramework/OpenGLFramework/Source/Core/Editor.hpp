#pragma once

namespace elba
{

  class Application;

  class Editor
  {
  public:

    Editor(Application *aApp);
    
    void Update();


  private:

    Application *mApp;

  };

}