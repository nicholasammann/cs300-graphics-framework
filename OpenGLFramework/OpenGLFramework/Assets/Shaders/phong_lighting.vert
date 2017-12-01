#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 litFragColor;

// texture uniforms
uniform int UseTextures;
uniform int MappingType;
uniform int DebugColors;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform int UseNormalMap;

uniform vec3 pMin;
uniform vec3 pMax;

// LIGHT UNIFORMS

const int MaxLights = 10;
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

  // attenuation coefficients
  float c1;
  float c2;
  float c3;
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

  // attenuation coefficients
  float c1;
  float c2;
  float c3;
};

uniform int PointLightCount;
uniform PointLight PointLights[MaxLights];

// other spotlight constants
uniform float spotInnerAngle;
uniform float spotOuterAngle;
uniform float spotFalloff;

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


vec4 dirlight_computeColor(in vec4 viewPos, in int lightIdx, in vec4 adiffuse, in float shininess, in vec4 trueNormal)
{
  DirLight light = DirLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (view * -light.direction);
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(trueNormal, lightUnitVec), 0);
  vec4 diffuse = diffuseFactor * light.diffuse * adiffuse;

  // calculate specular color
  vec4 viewVec = normalize(-vec4(viewPos.xyz, 0));
  vec4 reflec = 2.0 * viewVec * dot(viewVec, lightUnitVec) - lightUnitVec;
  float specFactor = pow(max(dot(reflec, viewVec), 0), shininess);
  vec4 specColor = light.specular * Material.specular * specFactor;

  vec4 color =  ambient + diffuse + specColor;

  return color;
}


vec4 spotlight_computeColor(in vec4 viewPos, in int lightIdx, in vec4 adiffuse, in float shininess, in vec4 trueNormal)
{
  SpotLight light = SpotLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (view * light.pos) - viewPos;
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(trueNormal.xyz, lightUnitVec.xyz), 0);
  vec4 diff = diffuseFactor * light.diffuse * adiffuse;

  // calculate specular color
  vec4 viewVec = normalize(-vec4(viewPos.xyz, 0));
  vec4 reflec = 2.0 * viewVec * dot(viewVec, lightUnitVec) - lightUnitVec;
  float specFactor = pow(max(dot(reflec, viewVec), 0), shininess);
  vec4 specColor = light.specular * Material.specular * specFactor;

  // calculate light source attenuation
  vec4 d = lightVec;
  float dL = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
  float lightAtt = min( 1.0 / (light.c1 + light.c2*dL + light.c3*dL*dL), 1);
  

  // calculate spotlight effect
  vec4 spotUnitDir = normalize(view * -light.direction);
  float alpha = dot(spotUnitDir.xyz, lightUnitVec.xyz);

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


vec4 pointlight_computeColor(in vec4 viewPos, in int lightIdx, in vec4 adiffuse, in float shininess, in vec4 trueNormal)
{
  PointLight light = PointLights[lightIdx];

  // calculate light vector in view space
  vec4 lightVec = (view * light.pos) - viewPos;
  vec4 lightUnitVec = normalize(lightVec);

  // calculate ambient color
  vec4 ambient = light.ambient * Material.ambient;

  // calculate diffuse color
  float diffuseFactor = max(dot(trueNormal, lightUnitVec), 0);
  vec4 diffuse = diffuseFactor * light.diffuse * adiffuse;

  // calculate specular color
  vec4 viewVec = normalize(-vec4(viewPos.xyz, 0));
  vec4 reflec = 2.0 * viewVec * dot(viewVec, lightUnitVec) - lightUnitVec;
  float specFactor = pow(max(dot(reflec, viewVec), 0), shininess);
  vec4 specColor = light.specular * Material.specular * specFactor;

  // calculate light source attenuation
  vec4 d = lightVec;
  float dL = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
  float lightAtt = min( 1.0 / (light.c1 + light.c2*dL + light.c3*dL*dL), 1);
  
  vec4 color = lightAtt * (ambient + diffuse + specColor);

  return color;
}

float compute_atmosphericAttenuation(in vec4 viewPos)
{
  vec4 ld = viewPos;
  float S = (fogFar - sqrt(ld.x*ld.x + ld.y*ld.y + ld.z*ld.z)) / (fogFar - fogNear);
  return S;
}


vec4 computeFragmentColor(in vec4 viewPos, in vec4 adiffuse, in float shininess, in vec4 trueNormal)
{
  vec4 color = vec4(0, 0, 0, 0);

  for (int i = 0; i < DirLightCount; ++i)
  {
    color += dirlight_computeColor(viewPos, i, adiffuse, shininess, trueNormal);
  }

  for (int i = 0; i < SpotLightCount; ++i)
  {
    color += spotlight_computeColor(viewPos, i, adiffuse, shininess, trueNormal);
  }

  for (int i = 0; i < PointLightCount; ++i)
  {
    color += pointlight_computeColor(viewPos, i, adiffuse, shininess, trueNormal);
  }

  // calculate local
  vec4 local = Material.emissive + Material.ambient*globalAmbient + color;

  // calculate final (with atmospheric attenuation)
  float S = compute_atmosphericAttenuation(viewPos);
  
  vec4 final = S * local + (1 - S) * fogColor;

  return final;
}


vec2 planarMapping()
{
  // find largest standard basis bias
  vec3 mag = abs(aPos.xyz);

  vec3 biasUVs = vec3(0.5) + 0.5 * aPos;

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


vec2 cylindricalMapping()
{
  vec2 uv;

  float theta = atan( aPos.y / aPos.x );

  if (aNormal.x < 0)
  {
    theta += 3.1415;
  }
  else if (aNormal.y < 0)
  {
    theta += 2 * 3.1415;
  }

  float h = (aPos.z - pMin.z)/(pMax.z - pMin.z);

  uv.x = theta / (2 * 3.1415f);
  uv.y = h;

  return uv;
}


vec2 sphericalMapping()
{
  vec2 uv;

  float theta = atan(aNormal.y / aNormal.x);

  if (aNormal.x < 0)
  {
    theta += 3.1415;
  }
  else if (aNormal.y < 0)
  {
    theta += 2 * 3.1415;
  }

  float phi = acos(aNormal.z);

  uv.x = theta / (2 * 3.1415);

  uv.y = phi / 3.1415;

  return uv;
}



void main()
{
  // deal with position and normal in world space

  vec4 viewPos = view * model * vec4(aPos, 1);
  vec4 worldPos = projection * view * model * vec4(aPos, 1);

  vec4 viewNorm = normalize(view * model * vec4(aNormal, 0));
  vec4 worldNorm = normalize(projection * view * model * vec4(aNormal, 0));

  // compute the final result of passing this vertex through the transformation
  // pipeline and yielding a coordinate in NDC space
  gl_Position = worldPos;
  
  vec4 diffuse = Material.diffuse;
  float shininess = Material.shininess;
  vec4 trueNormal = viewNorm;

  vec4 tang = view * model * vec4(normalize(aTangent), 0);
  vec4 bitang = view * model * vec4(normalize(aBitangent), 0);
  vec4 norm = view * model * vec4(normalize(aNormal), 0);

  mat3 TBN = mat3(tang.xyz, bitang.xyz, norm.xyz);

  vec2 uv;

  if (MappingType == 0)
  {
    uv = planarMapping();
  }
  else if (MappingType == 1)
  {
    uv = cylindricalMapping();
  }
  else if (MappingType == 2)
  {
    uv = sphericalMapping();
  }

  uv.x = clamp(uv.x, 0, 1);
  uv.y = clamp(uv.y, 0, 1);

  if (UseTextures == 1)
  {
    diffuse = vec4(texture(diffuseTexture, uv).rgb, 1);
    shininess = float(texture(specularTexture, uv).r);
  }

  if (UseNormalMap == 1)
  {
    vec3 sampledNorm = texture(normalTexture, uv).rgb;

    sampledNorm = 2 * (sampledNorm - vec3(0.5, 0.5, 0.5));

    vec3 viewspaceNorm = TBN * sampledNorm;

    trueNormal = normalize(vec4(viewspaceNorm, 0));
  }

  vec4 finalColor = vec4(0, 0, 0, 1);
  
  // draw normally
  if (DebugColors == 0)
  {
    finalColor = computeFragmentColor(viewPos, diffuse, shininess, trueNormal);
  }
  // use Tangent for RBG
  else if (DebugColors == 1)
  {
    finalColor.rgb = tang.xyz;
  }
  // use Bitangent for RBG
  else if (DebugColors == 2)
  {
    finalColor.rgb = bitang.xyz;
  }
  // use Normal for RBG
  else if (DebugColors == 3)
  {
    finalColor.rgb = trueNormal.xyz;
  }

  litFragColor = finalColor;
}