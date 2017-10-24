/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.cpp
Purpose: Defining the different scenes
Language: C++, VC 15.0
Platform: VC 15.0
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/23/17
--------------------------------------------------------*/

#include "Scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

      aApp->GetLightUniforms().spotFalloff = 1.0f;
      aApp->GetLightUniforms().spotInnerAngle = 15.0f;
      aApp->GetLightUniforms().spotOuterAngle = 30.0f;

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
          light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
          light.SetSpecular(1.0f, 0.4f, 1.0f, 1);

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
          light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
          light.SetSpecular(1.0f, 0.4f, 1.0f, 1);

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
          light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
          light.SetSpecular(1.0f, 0.4f, 1.0f, 1);

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

      aApp->GetLightUniforms().spotFalloff = 1.0f;
      aApp->GetLightUniforms().spotInnerAngle = 15.0f;
      aApp->GetLightUniforms().spotOuterAngle = 30.0f;

      for (int i = 0; i < 4; ++i)
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
          light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
          light.SetSpecular(1.0f, 0.4f, 1.0f, 1);
        }
        else
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(0.02f, 0.95f, 1.0f, 1);
          light.SetSpecular(0.02f, 0.95f, 1.0f, 1);
        }

        Material &mat = mod->GetMeshes()[0]->GetMaterial();
        mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

        lunis.PointLights.push_back(light);
      }

      for (int i = 0; i < 4; ++i)
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
          light.SetDiffuse(0.02f, 0.95f, 1.0f, 1);
          light.SetSpecular(0.02f, 0.95f, 1.0f, 1);
        }
        else
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
          light.SetSpecular(1.0f, 0.4f, 1.0f, 1);
        }

        Material &mat = mod->GetMeshes()[0]->GetMaterial();
        mat.SetAmbient(light.ambient[0], light.ambient[1], light.ambient[2], light.ambient[3]);

        lunis.SpotLights.push_back(light);
      }

      for (int i = 0; i < 2; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
        mod->SetShader("Light Model");
        mod->GetTransform().mScale = lightSize;

        DirLight light;
        light.model = mod;

        if (i % 2)
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(1.0f, 0.4f, 1.0f, 1);
          light.SetSpecular(1.0f, 0.4f, 1.0f, 1);
        }
        else
        {
          light.SetAmbient(0.0f, 0.0f, 0.0f, 1);
          light.SetDiffuse(0.02f, 0.95f, 1.0f, 1);
          light.SetSpecular(0.02f, 0.95f, 1.0f, 1);
        }

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

      aApp->GetLightUniforms().spotFalloff = 0.3f;
      aApp->GetLightUniforms().spotInnerAngle = 20.0f;
      aApp->GetLightUniforms().spotOuterAngle = 22.0f;

      // stop the app from changing the position and direction of our lights
      aApp->mRotateLights = false;

      for (int i = 0; i < 8; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
        mod->SetShader("Light Model");
        mod->GetTransform().mScale = lightSize;

        SpotLight light;
        light.model = mod;

        if (i % 2)
        {
          light.SetPos(-4, -1, -4 + i, 1);
          light.SetDirection(1, -0.5f, 0, 0);
        }
        else
        {
          light.SetPos(4, -1, 4 - i, 1);
          light.SetDirection(-1, -0.5f, 0, 0);
        }

        light.SetAmbient(0, 0, 0, 1);
        light.SetDiffuse(0.0f, 0.2f, (float)i/10.0f, 1);
        light.SetSpecular(0.0f, 0.2f, (float)i/10.0f, 1);

        Material &mat = mod->GetMeshes()[0]->GetMaterial();
        mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

        lunis.SpotLights.push_back(light);
      }

      float offset = (2 * 3.1416f) / 8;

      for (int i = 0; i < 8; ++i)
      {
        Model *mod = new Model(aApp, "../OpenGLFramework/Assets/Models/sphere.obj", "Sphere");
        mod->SetShader("Light Model");
        mod->GetTransform().mScale = lightSize;
      
        PointLight light;
        light.model = mod;
        light.SetPos(0, 0, 5, 1);
        light.SetAmbient(0, 0, 0, 1);
      

        if (i % 2)
        {
          light.SetDiffuse(0, (float)i / 50.0f, 0.2f, 1);
          light.SetSpecular(0, (float)i / 50.0f, 0.2f, 1);
        }
        else
        {
          light.SetDiffuse(0, 0.2f, (float)i / 50.0f, 1);
          light.SetSpecular(0, 0.2f, (float)i / 50.0f, 1);
        }

        Material &mat = mod->GetMeshes()[0]->GetMaterial();
        mat.SetAmbient(light.diffuse[0], light.diffuse[1], light.diffuse[2], light.diffuse[3]);

        vec4 initPos = vec4(3, 2, 0, 1);

        // initial rotation
        mat4 initRot = glm::rotate(mat4(), static_cast<float>(i * offset), vec3(0, 1, 0));

        // apply matrices to initial position
        vec4 pos = initRot * initPos;

        light.SetPos(pos.x, pos.y, pos.z, 1);
      
        lunis.PointLights.push_back(light);
      }
    }
  }

}