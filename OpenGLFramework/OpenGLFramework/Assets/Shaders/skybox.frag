#version 330 core

layout(location = 0) out vec4 vFragColor;

uniform sampler2D Top;
uniform sampler2D Bottom;
uniform sampler2D Front;
uniform sampler2D Back;
uniform sampler2D Left;
uniform sampler2D Right;

in vec4 modelPos;
in vec4 modelNorm;

vec3 computeBoxUV()
{
  // pick UV based on maximal extents (this is not used in CS300 assignment 2,
  // it's merely another way to generate UVs)...only allow comps in [-1, 1]
  vec3 position = clamp(modelPos.xyz, vec3(-1), vec3(1));

  vec3 color = vec3(0, 0, 0);

  // find largest standard basis bias
  vec3 mag = abs(position);

  vec2 uv;

  if (mag.x > mag.y && mag.x > mag.z)
  {
    // skybox left
    if (modelNorm.x > 0)
    {
      uv.x = ((modelPos.z / modelPos.x) + 1.0f) / 2.0f;
      uv.y = ((modelPos.y / modelPos.x) + 1.0f) / 2.0f;
      color = texture(Left, uv).rgb;
    }
    // skybox right
    else if (modelNorm.x < 0)
    {
      uv.x = ((modelPos.z / modelPos.x) + 1.0f) / 2.0f;
      uv.y = ((-modelPos.y / modelPos.x) + 1.0f) / 2.0f;
      color = texture(Right, uv).rgb;
    }
  }
  else if (mag.y > mag.z)
  {
    // skybox top
    if (modelNorm.y < 0)
    {
      uv.x = ((modelPos.x / modelPos.y) + 1.0f) / 2.0f;
      uv.y = ((-modelPos.z / modelPos.y) + 1.0f) / 2.0f;
      color = texture(Top, uv).rgb;
    }
    // skybox bottom
    else if (modelNorm.y > 0)
    {
      uv.x = ((-modelPos.x / modelPos.y) + 1.0f) / 2.0f;
      uv.y = ((-modelPos.z / modelPos.y) + 1.0f) / 2.0f;
      color = texture(Bottom, uv).rgb;
    }
  }
  else
  {
    // skybox front
    if (modelNorm.z < 0)
    {
      uv.x = ((-modelPos.x / modelPos.z) + 1.0f) / 2.0f;
      uv.y = ((-modelPos.y / modelPos.z) + 1.0f) / 2.0f;
      color = texture(Front, uv).rgb;
    }
    // skybox back
    else if (modelNorm.z > 0)
    {
      uv.x = ((-modelPos.x / modelPos.z) + 1.0f) / 2.0f;
      uv.y = ((modelPos.y / modelPos.z) + 1.0f) / 2.0f;
      color = texture(Back, uv).rgb;
    }
  }

  return color;
}

void main()
{
  vec4 finalColor = vec4(computeBoxUV(), 1);
  
  vFragColor = finalColor;
}