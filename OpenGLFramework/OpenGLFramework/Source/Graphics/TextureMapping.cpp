#include <glm/vec3.hpp>

#include "TextureMapping.hpp"

using namespace glm;

namespace ELBA
{

  vec2 planarMapping(vec3 aObjectPos)
  {
    // find largest standard basis bias
    vec3 mag;
    mag.x = abs(aObjectPos.x);
    mag.y = abs(aObjectPos.y);
    mag.z = abs(aObjectPos.z);

    vec3 biasUVs = vec3(0.5f, 0.5f, 0.5f) + 0.5f * aObjectPos;

    if (mag.x > mag.y && mag.x > mag.z)
    {
      // facing pos or neg x axis; use corrected y/z for UV
      return vec2(biasUVs.y, biasUVs.z);
    }
    else if (mag.y > mag.z)
    {
      // facing pos or neg y axis; use corrected x/z for UV
      return vec2(biasUVs.x, biasUVs.z);
    }
    else // z is the largest
    {
      // facing pos or neg z axis; use corrected x/y for UV
      return vec2(biasUVs.x, biasUVs.y);
    }
  }


  vec2 cylindricalMapping(vec3 aObjectPos, vec3 aObjectNorm, vec3 pMin, vec3 pMax)
  {
    vec2 uv;

    float theta = atan(aObjectPos.y / aObjectPos.x);

    if (aObjectNorm.x < 0)
    {
      theta += 3.1415;
    }
    else if (aObjectNorm.y < 0)
    {
      theta += 2 * 3.1415;
    }

    float h = (aObjectNorm.z - pMin.z) / (pMax.z - pMin.z);

    uv.x = theta / (2 * 3.1415f);
    uv.y = h;

    return uv;
  }


  vec2 sphericalMapping(vec3 aObjectNorm)
  {
    vec2 uv;

    float theta = atan(aObjectNorm.y / aObjectNorm.x);

    if (aObjectNorm.x < 0)
    {
      theta += 3.1415;
    }
    else if (aObjectNorm.y < 0)
    {
      theta += 2 * 3.1415;
    }

    float phi = acos(aObjectNorm.z);

    uv.x = theta / (2 * 3.1415);

    uv.y = phi / 3.1415;

    return uv;
  }

}