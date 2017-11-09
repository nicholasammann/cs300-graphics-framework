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

  // calculate position and normal in view space
  vec4 vPos = view * model * vec4(aPos, 1);

  oViewPos = vPos;
  oViewNorm = normalize(view * model * vec4(aNormal, 0));

  // calculate TBN matrix

  vec4 viewTan4 = view * model * vec4(aTangent, 0);
  vec4 viewBitan4 = view * model * vec4(aBitangent, 0);

  vec3 viewTangent = viewTan4.xyz;
  vec3 viewBitangent = viewBitan4.xyz;

  mat3 temp = mat3(viewTangent, viewBitangent, oViewNorm);

  TBN = temp;

  // final world coordinates
  gl_Position = projection * view * model * vec4(aPos, 1);
}