#include <iostream>

#include "Core/Application.hpp"

int g_width = 800;
int g_height = 600;

void window_resize_callback(GLFWwindow * aWindow, int aWidth, int aHeight)
{
  glViewport(0, 0, aWidth, aHeight);
  g_width = aWidth;
  g_height = aHeight;
}

GLFWwindow* InitializeGraphics()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Open GL Framework", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if (!window)
  {
    std::cout << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return nullptr;
  }

  GLenum err = glewInit();

  if (GLEW_OK != err)
  {
    std::cout << "glewInit failed." << std::endl;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, window_resize_callback);

  return window;
}


int main()
{
  GLFWwindow *win = InitializeGraphics();

  if (!win)
  {
    return -1;
  }

  ELBA::Application app = ELBA::Application(win);

  app.Init();
  app.CreateShader("Assets/Shaders/simple.vert", "Assets/Shaders/simple.frag");

  // main loop
  while (!glfwWindowShouldClose(app.GetWindow()))
  {
    app.Update(g_width, g_height);
  }
  
  app.Shutdown();

  return 0;
}