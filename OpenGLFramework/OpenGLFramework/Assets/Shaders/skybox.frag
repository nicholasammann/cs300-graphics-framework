#version 330 core

in vec3 texcoords;

uniform samplerCube skyboxTexture;

out vec4 finalColor;

void main() 
{
  finalColor = texture(skyboxTexture, texcoords);
}

