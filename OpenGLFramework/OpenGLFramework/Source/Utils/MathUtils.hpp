#pragma once

#include <vector>

std::vector<float> GaussianElimination(std::vector<std::vector<float>> matrix);

std::vector<std::vector<float>> MakeMatrix(float m00, float m01, float m02,
                                           float m10, float m11, float m22);