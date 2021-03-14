#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N)
{
  float sum = 0;
  for (int i = 0; i < N; i++)
  {
    sum = sum + x[i] * x[i];
  }
  return 10 * log10(sum / N);
}

float compute_am(const float *x, unsigned int N)
{
  float sum = 0;
  for (int i = 0; i < N; i++)
  {
    sum = sum + fabs(x[i]);
  }
  return (sum / N);
}

float compute_zcr(const float *x, unsigned int N, float fm)
{
  float sum = 0;
  for (int i = 1; i < N; i++)
  {
    int delta = 0;
    if ((x[i] < 0 && x[i - 1] > 0) || (x[i] > 0 && x[i - 1] < 0))
    {
      delta = 1;
    }
    sum = sum + delta;
  }
  float zcr = (fm / 2) * (1 / (N - 1)) * sum;

  return zcr;
}