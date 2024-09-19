#ifndef DFT_H_
#define DFT_H_

#include <complex>
#include <cstdint>
#include <vector>

using Complex = std::complex<float>;

struct Coef
{
  int k;
  Complex c;
};

struct DFT
{
  void computeCoefficient(int k, const std::vector<Complex> &seq);
  void addCoefficient(const std::vector<Complex>& seq);
  std::vector<Coef> coefficients;
};

#endif // DFT_H_
