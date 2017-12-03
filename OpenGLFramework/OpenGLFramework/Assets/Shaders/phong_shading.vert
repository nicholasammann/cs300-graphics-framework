#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 oObjPos;
out vec3 oObjNorm;

out vec4 oViewPos;
out vec4 oViewNorm;

out vec3 modTang;
out vec3 modBitang;

out vec4 oViewTangent;
out vec4 oViewBitangent;

out mat3 TBN;


void main()
{
  oObjPos = aPos;
  oObjNorm = aNormal;

  vec4 tang = view * model * vec4(normalize(aTangent), 0);
  vec4 bitang = view * model * vec4(normalize(aBitangent), 0);
  vec4 norm = view * model * vec4(normalize(aNormal), 0);

  oViewTangent = tang;
  oViewBitangent = bitang;

  TBN = mat3(tang.xyz, bitang.xyz, norm.xyz);
  
  // calculate position and normal in view space
  oViewPos = view * model * vec4(aPos, 1);
  oViewNorm = view * model * vec4(aNormal, 0);

  // final world coordinates
  gl_Position = projection * view * model * vec4(aPos, 1);
}