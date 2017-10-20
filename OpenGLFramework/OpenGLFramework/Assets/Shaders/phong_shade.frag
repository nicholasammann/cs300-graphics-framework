#version 330 core

in vec4 oViewPos;
in vec4 oViewNorm;
in vec4 oWorldNorm;
in vec4 oColor;

layout(location = 0) out vec4 vFragColor;

void main()
{
  vFragColor = litFragColor;
}