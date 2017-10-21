#version 330 core
layout(location = 0) out vec4 vFragColor;

in vec4 oViewPos;
in vec4 oViewNorm;

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


vec4 dirlight_computeColor(in int lightIdx)
{
  DirLight light = DirLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = view * -light.direction;
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(oViewNorm, lightUnitVec), 0);
  vec4 diffuse = diffuseFactor * light.diffuse * Material.diffuse;


  // calculate specular color
  vec4 viewVec = -vec4(oViewPos.xyz, 0);
  vec4 halfVec = lightUnitVec + viewVec;
  float specFactor = pow(dot(oViewNorm, halfVec), Material.shininess);
  vec4 specColor = light.specular * Material.specular * max(specFactor, 0);

  vec4 color =  ambient + diffuse + specColor;

  return color;
}



vec4 spotlight_computeColor(in int lightIdx)
{
  SpotLight light = SpotLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (-light.pos) - oViewPos;
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(oViewNorm, lightUnitVec), 0);
  vec4 diffuse = diffuseFactor * light.diffuse * Material.diffuse;


  // calculate specular color
  vec4 viewVec = -vec4(oViewPos.xyz, 0);
  vec4 halfVec = lightUnitVec + viewVec;
  float specFactor = pow(dot(oViewNorm, halfVec), Material.shininess);
  vec4 specColor = light.specular * Material.specular * max(specFactor, 0);
  

  // calculate light source attenuation
  vec4 d = lightVec;
  float dL = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
  float lightAtt = min( 1.0 / (c1 + c2*dL + c3*dL*dL), 1);
  

  // calculate spotlight effect
  vec4 spotUnitDir = normalize(light.direction);
  float alpha = dot(spotUnitDir, -lightUnitVec);

  float phi = cos(radians(spotOuterAngle));
  float theta = cos(radians(spotInnerAngle));

  float spotEffect = 0.0f;

  if (alpha < phi)
  {
    spotEffect = 0.0;
  }
  else if (alpha > theta)
  {
  }
  else
  {
    spotEffect = pow(( (cos(radians(alpha)) - phi) / (theta - phi) ), spotFalloff);
  }

  vec4 color = lightAtt * ambient + lightAtt * spotEffect * (diffuse + specColor);

  return color;
}



vec4 pointlight_computeColor(in int lightIdx)
{
  PointLight light = PointLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (view * -light.pos) - oViewPos;
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(oViewNorm, lightUnitVec), 0);
  vec4 diffuse = diffuseFactor * light.diffuse * Material.diffuse;


  // calculate specular color
  vec4 viewVec = -vec4(oViewPos.xyz, 0);
  vec4 halfVec = lightUnitVec + viewVec;
  float specFactor = pow(dot(oViewNorm, halfVec), Material.shininess);
  vec4 specColor = light.specular * Material.specular * max(specFactor, 0);
  

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


vec4 computeFragmentColor()
{
  vec4 color = vec4(0, 0, 0, 0);

  for (int i = 0; i < DirLightCount; ++i)
  {
    color += dirlight_computeColor(i);
  }

  for (int i = 0; i < SpotLightCount; ++i)
  {
    color += spotlight_computeColor(i);
  }

  for (int i = 0; i < PointLightCount; ++i)
  {
    color += pointlight_computeColor(i);
  }

  // calculate local
  vec4 local = Material.emissive + Material.ambient*globalAmbient + color;

  // calculate final (with atmospheric attenuation)
  float S = compute_atmosphericAttenuation();
  
  vec4 final = S * local + (1 - S) * fogColor;

  return final;
}


void main()
{
  vFragColor = computeFragmentColor();
}