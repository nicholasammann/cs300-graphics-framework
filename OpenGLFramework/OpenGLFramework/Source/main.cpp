#include "Core/Application.hpp"


int main()
{
  ELBA::Application app = ELBA::Application();
  app.Init();
  app.CreateShader("Shaders/simple.vert", "Shaders/simple.frag");

  // main loop
  while (!glfwWindowShouldClose(app.GetWindow()))
  {
    app.Update();
  }
  
  app.Shutdown();

  return 0;
}