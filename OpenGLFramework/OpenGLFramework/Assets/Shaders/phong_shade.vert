#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 oViewPos;
out vec4 oViewNorm;
out vec4 oWorldNorm;
out vec4 oColor;

void main()
{
  // calculate position and normal in view space
  oViewPos = view * model * vec4(aPos, 1);
  oViewNorm = normalize(view * model * vec4(aNormal, 0));

  // calculate position and normal in world space
  oWorldNorm = projection * viewNorm;

  // final world coordinates
  gl_Position = projection * viewPos;
}