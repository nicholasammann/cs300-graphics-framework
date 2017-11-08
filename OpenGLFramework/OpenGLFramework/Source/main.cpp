/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: main function
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#include <iostream>

#include "Core/Application.hpp"

#include "Utils/MathUtils.hpp"


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

  glewExperimental = GL_TRUE;

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


  //// test gaussian elimination
  std::vector<std::vector<float>> matrix;
  matrix.resize(2);
  matrix[0].resize(3);
  matrix[1].resize(3);
  
  matrix[0][0] = 3.0f;  matrix[0][1] = 1.0f;  matrix[0][2] = 5.0f;
  matrix[1][0] = 1.0f;  matrix[1][1] = 5.0f;  matrix[1][2] = 4.0f;
  
  auto sol = GaussianElimination(matrix);
  
  std::cout << "Size : " << sol.size() << std::endl;
  
  std::cout << "Solution : " << sol[0] << ", " << sol[1] << std::endl;


  // main loop
  while (!glfwWindowShouldClose(app.GetWindow()))
  {
    app.Update(g_width, g_height);
  }
  
  app.Shutdown();

  return 0;
}