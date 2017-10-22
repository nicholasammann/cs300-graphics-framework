#pragma once
#include "Application.hpp"
#include "../Graphics/Light.hpp"

namespace ELBA
{

  vec3 lightSize = vec3(0.3f, 0.3f, 0.3f);

  namespace Scene1
  {
    void CreateLights(Application *aApp)
    {
      auto &lunis = aApp->GetLightUniforms();

      lunis.DirLights.clear();
      lunis.SpotLights.clear();
      lunis.PointLights.clear();

      for (int i = 0; i < 8; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/CS300/sphere.obj", "Sphere");
        mod->SetShader("Simple");
        mod->GetTransform().mScale = lightSize;

        PointLight light;
        light.model = mod;
        light.SetPos(-2.0f + i/2.0f, 0, 5, 1);
        light.SetAmbient(0, 0.6f, 0, 1);
        light.SetDiffuse(0, 0.6f, 0, 1);
        light.SetSpecular(0, 0.6f, 0, 1);

        lunis.PointLights.push_back(light);
      }
    }
  }

  namespace Scene2
  {
    void CreateLights(Application *aApp)
    {
      auto &lunis = aApp->GetLightUniforms();

      lunis.DirLights.clear();
      lunis.SpotLights.clear();
      lunis.PointLights.clear();

      for (int i = 0; i < 3; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/CS300/sphere.obj", "Sphere");
        mod->SetShader("Simple");
        mod->GetTransform().mScale = lightSize;

        PointLight light;
        light.model = mod;
        light.SetPos(-2.0f + i / 2.0f, 0, 5, 1);
        light.SetAmbient(0, 0.6f, 0, 1);
        light.SetDiffuse(0, 0.6f, 0, 1);
        light.SetSpecular(0, 0.6f, 0, 1);

        lunis.PointLights.push_back(light);
      }

      for (int i = 0; i < 3; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/CS300/sphere.obj", "Sphere");
        mod->SetShader("Simple");
        mod->GetTransform().mScale = lightSize;

        SpotLight light;
        light.model = mod;
        light.SetPos(-2.0f + (i+3) / 2.0f, 0, 5, 1);
        light.SetDirection(0, 0, -1, 0);
        light.SetAmbient(0.6f, 0, 0, 1);
        light.SetDiffuse(0.6f, 0, 0, 1);
        light.SetSpecular(0.6f, 0, 0, 1);

        lunis.SpotLights.push_back(light);
      }

      for (int i = 0; i < 3; ++i)
      {
        DirLight light;
        light.SetDirection(0, 0, -1, 0);
        light.SetAmbient(0.6f, 0, 0, 1);
        light.SetDiffuse(0.6f, 0, 0, 1);
        light.SetSpecular(0.6f, 0, 0, 1);

        lunis.DirLights.push_back(light);
      }
    }
  }


  namespace Scene3
  {
    void CreateLights(Application *aApp)
    {

    }
  }

}