/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.hpp
Purpose: Shader class
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#pragma once

#include <string>

#include <GL\glew.h>


namespace ELBA
{
  class Shader
  {
  public:

    // read in shader source code and build shader
    Shader(const char *aName, const char *vertPath, const char *fragPath);

    // activate the shader
    void UseShaderProgram();

    // utility uniform functions
    void SetBool(const std::string &name, bool value);
    void SetInt(const std::string &name, int value);
    void SetFloat(const std::string &name, float value);

    unsigned int GetShaderProgram() const;
    unsigned int GetVertShader() const;
    unsigned int GetFragShader() const;

    std::string GetName();
    std::string GetVertPath();
    std::string GetFragPath();

  private:

    std::string mName;
    std::string mVertPath;
    std::string mFragPath;

    unsigned int mShaderProgram;
    unsigned int mVertShader;
    unsigned int mFragShader;

    // reads in a shader from file
    const GLchar* ReadShader(const std::string &filename);
  };
}