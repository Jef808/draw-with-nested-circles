#include "dft.h"

constexpr float TWO_PI = 2.0f * M_PIf;

void DFT::computeCoefficient(int k, const std::vector<Complex>& seq)
{
    Complex sum{0.0f, 0.0f};

    const float N = static_cast<float>(seq.size());
    const float w = TWO_PI * k / N;

    for (uint32_t n = 0; n < seq.size(); ++n)
    {
        sum += seq[n] * std::exp(std::complex<float>(0.0f, -w * static_cast<float>(n)));
    }

    coefficients.push_back({k, sum / N});
}

void DFT::addCoefficient(const std::vector<Complex>& seq)
{
    static int idx = 0.0;

    computeCoefficient(idx, seq);
    idx = idx > 0 ? -idx : -idx + 1;
}
