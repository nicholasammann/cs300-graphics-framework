#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;

out vec4 modelPos;
out vec4 modelNorm;

void main()
{  
  modelPos = vec4(vVertex, 1);
  modelNorm = vec4(vNormal, 0);

  gl_Position = projection * view * model * vec4(vVertex, 1);
}