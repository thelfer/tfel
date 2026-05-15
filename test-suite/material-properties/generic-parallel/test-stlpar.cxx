/*!
 * \file   test-stlpar.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/04/2026
 */

#include <array>
#include <vector>
#include <iostream>

#include "MFront/GenericMaterialProperty/MaterialProperty.h"
#include "Inconel600_YoungModulus-generic-parallel-stlpar.hxx"

double fct(const double TK) {
  const double TC = TK - 273.15;
  return (-3.1636e-3 * TC * TC - 3.8654 * TC + 2.1421e+4) * 1e7;
}  // end of fct

void f1() {
  std::cout << "# f1:\n";
  auto E = std::vector<double>(4);
  const auto T = std::vector<double>{300, 500, 300, 800};
  auto output = mfront_gmp_OutputStatus{};
  auto policy = mfront_gmp_OutOfBoundsPolicy{};
  auto args = std::array<const double *, 1u>{T.data()};
  auto args_strides = std::array<mfront_gmp_size_type, 1u>{1};
  Inconel600_YoungModulus(&output, E.data(), 1, args.data(),
                          args_strides.data(), 1, 4, policy);
  for (std::size_t i = 0; i != E.size(); ++i) {
    std::cout << "- " << E[i] << " " << fct(T[i]) << "\n";
  }
}

void f2() {
  std::cout << "# f2:\n";
  auto E = std::vector<double>(4);
  const auto T = std::vector<double>{300, 500, 300, 800};
  auto output = mfront_gmp_OutputStatus{};
  auto policy = mfront_gmp_OutOfBoundsPolicy{};
  auto args = std::array<const double *, 1u>{T.data()};
  Inconel600_YoungModulus2(&output, E.data(), args.data(), 1, 4, policy);
  for (std::size_t i = 0; i != E.size(); ++i) {
    std::cout << "- " << E[i] << " " << fct(T[i]) << "\n";
  }
}

int main() {
  f1();
  f2();
  return EXIT_SUCCESS;
}
