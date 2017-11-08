#include "MathUtils.hpp"

std::vector<float> GaussianElimination(std::vector<std::vector<float>> matrix)
{
  int height = matrix.size();

  // iterate through rows
  for (int i = 0; i < height; ++i)
  {
    float maxElement = matrix[i][i];
    int maxRow = i;

    // search for the max element in this column
    for (int k = i + 1; k < height; ++k)
    {

      if (matrix[k][i] > maxElement)
      {
        maxElement = matrix[k][i];
        maxRow = k;
      }
    }

    // swap the max row with the current row
    for (int k = i; k < height + 1; ++k)
    {
      float temp = matrix[maxRow][k];
      matrix[maxRow][k] = matrix[i][k];
      matrix[i][k] = temp;
    }

    // make all the rows below the current row 0 in the current column
    for (int k = i + 1; k < height; ++k)
    {
      float c = -(matrix[k][i] / matrix[i][i]);

      for (int j = i; j < height + 1; ++j)
      {
        if (i == j)
        {
          matrix[k][j] = 0.0f;
        }
        else
        {
          matrix[k][j] += c * matrix[i][j];
        }
      }
    }
  }

  // solve the equation for the upper triangular matrix
  std::vector<float> solution;
  solution.resize(height);

  for (int i = height - 1; i >= 0; --i)
  {
    //float denom = matrix[i][i];
    //
    //if (denom == 0.0f)
    //{
    //  denom = 0.0001f;
    //}

    solution[i] = matrix[i][height] / matrix[i][i];

    //if (isnan(solution[i]))
    //{
    //  solution[i] = 0.0f;
    //}

    for (int k = i - 1; k >= 0; --k)
    {
      matrix[k][height] -= matrix[k][i] * solution[i];
    }
  }

  for (unsigned i = 0; i < solution.size(); ++i)
  {
    if (isnan(solution[i]))
    {
      solution[i] = 0.0f;
    }
  }

  return solution;
}

std::vector<std::vector<float>> MakeMatrix(float m00, float m01, float m02, 
                                           float m10, float m11, float m22)
{
  std::vector<std::vector<float>> matrix;

  matrix.resize(2);
  matrix[0].resize(3);
  matrix[1].resize(3);
  
  matrix[0][0] = m00;  matrix[0][1] = m01;  matrix[0][2] = m02;
  matrix[1][0] = m10;  matrix[1][1] = m11;  matrix[1][2] = m22;

  return matrix;
}
