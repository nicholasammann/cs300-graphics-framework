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

out mat4 TBN;

void main()
{
  oObjPos = aPos;
  oObjNorm = aNormal;

  // calculate position and normal in view space
  vec4 vPos = view * model * vec4(aPos, 1);

  oViewPos = vPos;
  vec4 viewNorm = normalize(view * model * vec4(aNormal, 0));
  oViewNorm = viewNorm;

  // invert
  // model space

  mat4 temp = mat4(vec4(aTangent, 0), vec4(aBitangent, 0), vec4(aNormal, 0), vec4(0, 0, 0, 1));

  temp = transpose(temp);

  TBN = view * model * temp;

  // final world coordinates
  gl_Position = projection * view * model * vec4(aPos, 1);
}