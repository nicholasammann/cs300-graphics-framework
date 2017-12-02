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

vec2 computeBoxUV()
{
  // pick UV based on maximal extents (this is not used in CS300 assignment 2,
  // it's merely another way to generate UVs)...only allow comps in [-1, 1]
  vec3 position = clamp(modelPos.xyz, vec3(-1), vec3(1));
  
  // find largest standard basis bias
  vec3 mag = abs(position);
  vec3 biasUVs = vec3(0.5) + 0.5 * position;
  if (mag.x > mag.y && mag.x > mag.z)
  {
    // facing pos or neg x axis; use corrected y/z for UV
    return biasUVs.yz;
  }
  else if (mag.y > mag.z)
  {
    // facing pos or neg y axis; use corrected x/z for UV
    return biasUVs.xz;
  }
  else // z is the largest
  {
    // facing pos or neg z axis; use corrected x/y for UV
    return biasUVs.xy;
  }
}

void main()
{
  vec2 uv = computeBoxUV();

  vec4 finalColor = vec4(0, 0, 0, 1);

  // skybox top
  if (modelNorm.y < 0)
  {
    finalColor.rgb = texture(Top, uv).rgb;
  }
  // skybox bottom
  else if (modelNorm.y > 0)
  {
    finalColor.rgb = texture(Bottom, uv).rgb;
  }
  // skybox front
  else if (modelNorm.z > 0)
  {
    finalColor.rgb = texture(Front, uv).rgb;
  }
  // skybox back
  else if (modelNorm.z < 0)
  {
    finalColor.rgb = texture(Back, uv).rgb;
  }
  // skybox left
  else if (modelNorm.x < 0)
  {
    finalColor.rgb = texture(Left, uv).rgb;
  }
  // skybox right
  else if (modelNorm.x > 0)
  {
    finalColor.rgb = texture(Right, uv).rgb;
  }

  vFragColor = finalColor;
}