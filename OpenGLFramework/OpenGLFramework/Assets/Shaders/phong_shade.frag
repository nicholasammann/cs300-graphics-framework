#version 330 core

in vec4 oViewPos;
in vec4 oViewNorm;

layout(location = 0) out vec4 vFragColor;

void main()
{
  vFragColor = litFragColor;
}