/*!
 * \file  tests/Math/st2tost2/StensorSquareDerivative.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 april 2015
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
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/st2tot2.hxx"

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
struct StensorSquareDerivative final : public tfel::tests::TestCase {
  StensorSquareDerivative()
      : tfel::tests::TestCase("TFEL/Math",
                              "StensorSquareDerivative" + std::to_string(N)) {
  }  // end of StensorSquareDerivative
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-3;
    const double prec = 1.e-10;
    const double veel[6] = {0.00077453824166446, 0.00077207083708966,
                            0.0024525337568425,  -1.7790370858361e-08,
                            3.613971630283e-10,  -1.7873236537153e-08};
    const stensor<N> eel(veel);
    st2tost2<N> D(double(0));
    D(0, 0) = D(1, 1) = D(2, 2) = 2.80;
    if (N > 1) {
      D(3, 3) = 4.16;
      D(0, 1) = D(0, 2) = D(1, 0) = D(2, 0) = D(1, 2) = D(2, 1) = 1.20;
    }
    if (N == 3) {
      D(4, 4) = D(5, 5) = 4.16;
    }
    const stensor<N> eel2 = square(eel);
    const st2tost2<N, double> deel2(st2tost2<N, double>::dsquare(eel));
    const st2tost2<N, double> deel2_D(st2tost2<N, double>::dsquare(eel, D));
    st2tost2<N, double> ndeel2(double(0));
    for (typename stensor<N>::size_type i = 0; i != eel.size(); ++i) {
      stensor<N> eel_e(eel);
      eel_e[i] += eps;
      const stensor<N> eel2_p = syme(eel_e * eel_e);
      eel_e[i] -= 2 * eps;
      const stensor<N> eel2_m = syme(eel_e * eel_e);
      for (typename stensor<N>::size_type j = 0; j != eel2.size(); ++j) {
        ndeel2(j, i) = (eel2_p(j) - eel2_m(j)) / (2 * eps);
      }
    }
    const st2tost2<N, double> ndeel2_D(ndeel2 * D);
    for (typename tensor<N>::size_type i = 0; i != eel.size(); ++i) {
      for (typename stensor<N>::size_type j = 0; j != eel.size(); ++j) {
        TFEL_TESTS_ASSERT(abs(ndeel2(i, j) - deel2(i, j)) < prec);
        if (abs(ndeel2(i, j) - deel2(i, j)) > prec) {
          cout << "Error " << N << " (" << i << ", " << j << ") "
               << "[" << i * StensorDimeToSize<N>::value + j
               << "]: " << ndeel2(i, j) << " vs " << deel2(i, j) << " "
               << abs(ndeel2(i, j) - deel2(i, j)) << endl;
        }
        TFEL_TESTS_ASSERT(abs(ndeel2_D(i, j) - deel2_D(i, j)) < prec);
        if (abs(ndeel2_D(i, j) - deel2_D(i, j)) > prec) {
          cout << "Error " << N << " (" << i << ", " << j << ") "
               << "[" << i * StensorDimeToSize<N>::value + j
               << "]: " << ndeel2_D(i, j) << " vs " << deel2_D(i, j) << " "
               << abs(ndeel2_D(i, j) - deel2_D(i, j)) << endl;
        }
      }
    }
    return this->result;
  }  // end of execute
};

using StensorSquareDerivative_1D = StensorSquareDerivative<1U>;
using StensorSquareDerivative_2D = StensorSquareDerivative<2U>;
using StensorSquareDerivative_3D = StensorSquareDerivative<3U>;
TFEL_TESTS_GENERATE_PROXY(StensorSquareDerivative_1D,
                          "StensorSquareDerivative-1D");
TFEL_TESTS_GENERATE_PROXY(StensorSquareDerivative_2D,
                          "StensorSquareDerivative-2D");
TFEL_TESTS_GENERATE_PROXY(StensorSquareDerivative_3D,
                          "StensorSquareDerivative-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("StensorSquareDerivative.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
