#version 330 core
layout(location = 0) out vec4 vFragColor;

in vec4 oViewPos;
in vec4 oViewNorm;

// texture uniforms
uniform int UseTextures;
uniform int MappingType;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform vec3 pMin;
uniform vec3 pMax;


uniform mat4 view;

// LIGHT UNIFORMS

const int MaxLights = 8;
uniform vec4 globalAmbient;

// directional lights
struct DirLight
{
  vec4 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
uniform int DirLightCount;
uniform DirLight DirLights[MaxLights];

// spot lights
struct SpotLight
{
  vec4 pos;
  vec4 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
uniform int SpotLightCount;
uniform SpotLight SpotLights[MaxLights];

// point lights
struct PointLight
{
  vec4 pos;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
uniform int PointLightCount;
uniform PointLight PointLights[MaxLights];

// other spotlight constants
uniform float spotInnerAngle;
uniform float spotOuterAngle;
uniform float spotFalloff;

// attenuation coefficients
uniform float c1;
uniform float c2;
uniform float c3;

uniform vec4 fogColor;
uniform float fogNear;
uniform float fogFar;


uniform struct
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 emissive;
  float shininess;
} Material;


vec4 dirlight_computeColor(in int lightIdx, in vec4 adiffuse, in vec4 specular)
{
  DirLight light = DirLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (-light.direction);
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(oViewNorm, lightUnitVec), 0);
  vec4 diffuse = diffuseFactor * light.diffuse * adiffuse;


  // calculate specular color
  vec4 viewVec = -vec4(oViewPos.xyz, 0);
  vec4 halfVec = lightUnitVec + viewVec;
  float specFactor = pow(dot(oViewNorm, halfVec), Material.shininess);
  vec4 specColor = light.specular * specular * max(specFactor, 0);

  vec4 color =  ambient + diffuse + specColor;

  return color;
}


vec4 spotlight_computeColor(in int lightIdx, in vec4 adiffuse, in vec4 specular)
{
  SpotLight light = SpotLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (view * light.pos) - oViewPos;
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(oViewNorm, lightUnitVec), 0);
  vec4 diff = diffuseFactor * light.diffuse * adiffuse;

  // calculate specular color
  vec4 viewVec = -vec4(oViewPos.xyz, 0);
  vec4 halfVec = lightUnitVec + viewVec;
  float specFactor = pow(dot(oViewNorm, halfVec), Material.shininess);
  vec4 specColor = light.specular * specular * max(specFactor, 0);
  

  // calculate light source attenuation
  vec4 d = lightVec;
  float dL = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
  float lightAtt = min( 1.0 / (c1 + c2*dL + c3*dL*dL), 1);
  

  // calculate spotlight effect
  vec4 spotUnitDir = normalize(-light.direction);
  float alpha = dot(spotUnitDir, lightUnitVec);

  float phi = cos(radians(spotOuterAngle));
  float theta = cos(radians(spotInnerAngle));

  float spotEffect = 0.0f;

  if (alpha < phi)
  {
    spotEffect = 0.0f;
  }
  else if (alpha > theta)
  {
    spotEffect = 1.0f;
  }
  else
  {
    spotEffect = pow((alpha - phi) / (theta - phi), spotFalloff);
  }

  vec4 color = lightAtt * ambient + lightAtt * spotEffect * (diff + specColor);

  return color;
}


vec4 pointlight_computeColor(in int lightIdx, in vec4 adiffuse, in vec4 specular)
{
  PointLight light = PointLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (view * light.pos) - oViewPos;
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(oViewNorm, lightUnitVec), 0);
  vec4 diffuse = diffuseFactor * light.diffuse * adiffuse;


  // calculate specular color
  vec4 viewVec = -vec4(oViewPos.xyz, 0);
  vec4 halfVec = lightUnitVec + viewVec;
  float specFactor = pow(dot(oViewNorm, halfVec), Material.shininess);
  vec4 specColor = light.specular * specular * max(specFactor, 0);
  

  // calculate light source attenuation
  vec4 d = lightVec;
  float dL = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
  float lightAtt = min( 1.0 / (c1 + c2*dL + c3*dL*dL), 1);
  
  vec4 color = lightAtt * (ambient + diffuse + specColor);

  return color;
}

float compute_atmosphericAttenuation()
{
  vec4 ld = oViewPos;
  float S = (fogFar - sqrt(ld.x*ld.x + ld.y*ld.y + ld.z*ld.z)) / (fogFar - fogNear);
  return S;
}


vec4 computeFragmentColor(in vec4 adiffuse, in vec4 specular)
{
  vec4 color = vec4(0, 0, 0, 0);

  for (int i = 0; i < DirLightCount; ++i)
  {
    color += dirlight_computeColor(i, adiffuse, specular);
  }

  for (int i = 0; i < SpotLightCount; ++i)
  {
    color += spotlight_computeColor(i, adiffuse, specular);
  }

  for (int i = 0; i < PointLightCount; ++i)
  {
    color += pointlight_computeColor(i, adiffuse, specular);
  }

  // calculate local
  vec4 local = Material.emissive + Material.ambient*globalAmbient + color;

  // calculate final (with atmospheric attenuation)
  float S = compute_atmosphericAttenuation();
  
  vec4 final = S * local + (1 - S) * fogColor;

  return final;
}


vec2 computeBoxUV()
{
  // pick UV based on maximal extents (this is not used in CS300 assignment 2,
  // it's merely another way to generate UVs)...only allow comps in [-1, 1]
  vec3 position = clamp(oViewPos.xyz, vec3(-1), vec3(1));
  
  // find largest standard basis bias
  vec3 mag = abs(position);
  vec3 biasUVs = vec3(0.5) + 0.5 * position;
  if (mag.x > mag.y && mag.x > mag.z)
  {
    // facing pos or neg x axis; use corrected y/z for UV
    return biasUVs.yz;
  }
  else if (mag.y > mag.z)
  {
    // facing pos or neg y axis; use corrected x/z for UV
    return biasUVs.xz;
  }
  else // z is the largest
  {
    // facing pos or neg z axis; use corrected x/y for UV
    return biasUVs.xy;
  }
}


vec2 planarMapping()
{
  vec2 uv;

  uv.x = (oViewPos.x - pMin.x) / (pMax.x - pMin.x);

  uv.y = (oViewPos.y - pMin.y) / (pMax.y - pMin.y);

  return uv;
}


vec2 cylindricalMapping()
{
  vec2 uv;

  float theta = atan( oViewPos.y / oViewPos.x );

  float h = (oViewPos.z - pMin.z)/(pMax.z - pMin.z);

  uv.x = theta / 360.0f;
  uv.y = h;

  return uv;
}


vec2 sphericalMapping()
{
  vec2 uv;

  float theta = atan(oViewNorm.y / oViewNorm.x);

  float phi = acos(oViewNorm.z);

  uv.x = theta / 360.0f;
  uv.y = phi / 180.0f;

  return uv;
}


void main()
{
  vec4 diffuse = Material.diffuse;
  vec4 specular = Material.specular;
  
  if (UseTextures == 1)
  {
    vec2 uv;

    if (MappingType == 0)
    {
      uv = computeBoxUV();
    }
    else if (MappingType == 1)
    {
      uv = cylindricalMapping();
    }
    else if (MappingType == 2)
    {
      uv = sphericalMapping();
    }
    
    diffuse = vec4(texture(diffuseTexture, uv).rgb, 1);
    specular = vec4(texture(specularTexture, uv).rgb, 1);
  }

  vFragColor = computeFragmentColor(diffuse, specular);
}