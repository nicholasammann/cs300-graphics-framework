#pragma once

#include <GLFW/glfw3.h>

namespace elba
{
  class Editor;

  class Application
  {
  public:

    Application(GLFWwindow *aWindow);
    ~Application();

    void Init();
    void Update(int aWidth, int aHeight);
    void Shutdown();

    GLFWwindow* GetWindow();

    int GetWindowWidth();
    int GetWindowHeight();

  private:

    Editor *mEditor;

    GLFWwindow *mWindow;
  
    int mWindowWidth;
    int mWindowHeight;

  };
}

