#include "Scene.hpp"

#include "../Graphics/Light.hpp"

namespace ELBA
{
  vec3 lightSize = vec3(0.25f, 0.25f, 0.25f);


  namespace Scene1
  {
    void CreateLights(Application *aApp, int aLightType)
    {
      auto &lunis = aApp->GetLightUniforms();

      lunis.DirLights.clear();
      lunis.SpotLights.clear();
      lunis.PointLights.clear();

      switch (aLightType)
      {
      case Directional:
      {
        for (int i = 0; i < 8; ++i)
        {
          Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
          mod->SetShader("Light Model");
          mod->GetTransform().mScale = lightSize;

          DirLight light;
          light.model = mod;
          light.SetModelPos(-2.0f + i / 2.0f, 0, 5, 1);
          light.SetDirection(0, 0, 0, 0);
          light.SetAmbient(0, 0, 0, 1);
          light.SetDiffuse(0, 0, 1, 1);
          light.SetSpecular(0, 0, 1, 1);

          Material &mat = mod->GetMeshes()[0]->GetMaterial();
          mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

          lunis.DirLights.push_back(light);
        }
        break;
      }

      case Spot:
      {
        for (int i = 0; i < 8; ++i)
        {
          Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
          mod->SetShader("Light Model");
          mod->GetTransform().mScale = lightSize;

          SpotLight light;
          light.model = mod;
          light.SetPos(-2.0f + i / 2.0f, 0, 5, 1);
          light.SetDirection(0, 0, 0, 0);
          light.SetAmbient(0, 0, 0, 1);
          light.SetDiffuse(0, 0, 1, 1);
          light.SetSpecular(0, 0, 1, 1);

          Material &mat = mod->GetMeshes()[0]->GetMaterial();
          mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

          lunis.SpotLights.push_back(light);
        }
        break;
      }

      case Point:
      {
        for (int i = 0; i < 8; ++i)
        {
          Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
          mod->SetShader("Light Model");
          mod->GetTransform().mScale = lightSize;

          PointLight light;
          light.model = mod;
          light.SetPos(-2.0f + i / 2.0f, 0, 5, 1);
          light.SetAmbient(0, 0, 0, 1);
          light.SetDiffuse(0, 0, 1, 1);
          light.SetSpecular(0, 0, 1, 1);

          Material &mat = mod->GetMeshes()[0]->GetMaterial();
          mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

          lunis.PointLights.push_back(light);
        }
        break;
      }
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
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
        mod->SetShader("Light Model");
        mod->GetTransform().mScale = lightSize;

        PointLight light;
        light.model = mod;
        light.SetPos(-2.0f + i / 2.0f, 0, 5, 1);

        if (i % 2)
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(1.0f, 1.0f, 1.0f, 1);
          light.SetSpecular(1.0f, 1.0f, 1.0f, 1);
        }
        else
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(1.0f, 1.0f, 1.0f, 1);
          light.SetSpecular(1.0f, 1.0f, 1.0f, 1);
        }

        Material &mat = mod->GetMeshes()[0]->GetMaterial();
        mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

        lunis.PointLights.push_back(light);
      }

      for (int i = 0; i < 3; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
        mod->SetShader("Light Model");
        mod->GetTransform().mScale = lightSize;

        SpotLight light;
        light.model = mod;
        light.SetPos(-2.0f + (i + 3) / 2.0f, 0, 5, 1);
        light.SetDirection(0, 0, -1, 0);

        if (i % 2)
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(1.0f, 0, 0, 1);
          light.SetSpecular(1.0f, 0, 0, 1);
        }
        else
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(1.0f, 0, 0, 1);
          light.SetSpecular(1.0f, 0, 0, 1);
        }

        Material &mat = mod->GetMeshes()[0]->GetMaterial();
        mat.SetAmbient(light.ambient[0], light.ambient[1], light.ambient[2], light.ambient[3]);

        lunis.SpotLights.push_back(light);
      }

      for (int i = 0; i < 3; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
        mod->SetShader("Light Model");
        mod->GetTransform().mScale = lightSize;

        DirLight light;
        light.model = mod;

        light.SetDirection(0, 0, -1, 0);
        light.SetAmbient(0, 0, 0, 1);
        light.SetDiffuse(0, 1, 0, 1);
        light.SetSpecular(0, 1, 0, 1);

        Material &mat = mod->GetMeshes()[0]->GetMaterial();
        mat.SetAmbient(light.ambient[0], light.ambient[1], light.ambient[2], light.ambient[3]);

        lunis.DirLights.push_back(light);
      }
    }
  }


  namespace Scene3
  {
    void CreateLights(Application *aApp)
    {
      auto &lunis = aApp->GetLightUniforms();

      lunis.DirLights.clear();
      lunis.SpotLights.clear();
      lunis.PointLights.clear();
    }
  }

}