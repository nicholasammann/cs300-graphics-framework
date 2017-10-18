#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 oViewPos;
out vec4 oViewNorm;

void main()
{
  // calculate position and normal in view space
  vec4 viewPos = view * model * vec4(aPos, 1);
  vec4 viewNorm = normalize(view * model * vec4(aNormal, 0));
  
  // calculate position and normal in world space
  vec4 worldPos = projection * viewPos;
  vec4 worldNorm = projection * viewNorm;

  // final world coordinates
  gl_Position = worldPos;
}