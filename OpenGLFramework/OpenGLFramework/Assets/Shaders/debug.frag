#version 330 core
layout(location = 0) out vec4 vFragColor;

uniform vec4 FinalColor;

void main()
{    
    vFragColor = FinalColor;
}