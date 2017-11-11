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

out mat3 TBN;

void main()
{
  oObjPos = aPos;
  oObjNorm = aNormal;

  // invert
  // model space
  vec3 tang = normalize(vec3(view * model * vec4(aTangent, 0)).xyz);
  vec3 bitang = normalize(vec3(view * model * vec4(aBitangent, 0)).xyz);
  vec3 norm = normalize(vec3(view * model * vec4(aNormal, 0)).xyz);

  TBN = transpose(mat3(tang, bitang, norm));
  
  // calculate position and normal in view space
  vec4 vPos = view * model * vec4(aPos, 1);

  oViewPos = vPos;
  oViewNorm = view * model * vec4(aNormal, 0);

  // final world coordinates
  gl_Position = projection * view * model * vec4(aPos, 1);
}