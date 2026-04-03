/*!
 * \file  tests/Math/t2tost2/rightCauchyGreenTensorDerivative.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 04 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <string>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

template <unsigned short N>
struct RightCauchyGreenTensorDerivative final : public tfel::tests::TestCase {
  RightCauchyGreenTensorDerivative()
      : tfel::tests::TestCase(
            "TFEL/Math",
            "RightCauchyGreenTensorDerivative" +
                std::to_string(static_cast<unsigned int>(N))) {
  }  // end of RightCauchyGreenTensorDerivative
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-7;
    const double prec = 1.e-8;
    tensor<N> F;
    for (typename tensor<N>::size_type i = 0; i != F.size(); ++i) {
      if (i < 3) {
        F[i] = 1 + 0.03 * i * i;
      } else {
        F[i] = 0.03 * i;
      }
    }
    stensor<N> C = computeRightCauchyGreenTensor(F);
    t2tost2<N> dC = t2tost2<N>::dCdF(F);
    t2tost2<N> ndC;
    for (typename tensor<N>::size_type i = 0; i != F.size(); ++i) {
      tensor<N> Fe(F);
      Fe[i] += eps;
      stensor<N> Cfe = computeRightCauchyGreenTensor(Fe);
      Fe[i] -= 2 * eps;
      stensor<N> Cbe = computeRightCauchyGreenTensor(Fe);
      for (typename tensor<N>::size_type j = 0; j != C.size(); ++j) {
        ndC(j, i) = (Cfe(j) - Cbe(j)) / (2 * eps);
      }
    }
    for (typename tensor<N>::size_type i = 0; i != C.size(); ++i) {
      for (typename tensor<N>::size_type j = 0; j != F.size(); ++j) {
        TFEL_TESTS_ASSERT(abs(ndC(i, j) - dC(i, j)) < prec);
        if (abs(ndC(i, j) - dC(i, j)) > prec) {
          cout << "error (" << N << "): " << i << " " << j << " " << ndC(i, j)
               << " " << dC(i, j) << " " << abs(ndC(i, j) - dC(i, j)) << endl;
        }
      }
    }
    return this->result;
  }  // end of execute
};

using RightCauchyGreenTensorDerivative_1D =
    RightCauchyGreenTensorDerivative<1U>;
using RightCauchyGreenTensorDerivative_2D =
    RightCauchyGreenTensorDerivative<2U>;
using RightCauchyGreenTensorDerivative_3D =
    RightCauchyGreenTensorDerivative<3U>;
TFEL_TESTS_GENERATE_PROXY(RightCauchyGreenTensorDerivative_1D,
                          "RightCauchyGreenTensorDerivative-1D");
TFEL_TESTS_GENERATE_PROXY(RightCauchyGreenTensorDerivative_2D,
                          "RightCauchyGreenTensorDerivative-2D");
TFEL_TESTS_GENERATE_PROXY(RightCauchyGreenTensorDerivative_3D,
                          "RightCauchyGreenTensorDerivative-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("RightCauchyGreenTensorDerivative.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
