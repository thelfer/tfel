/*!
 * \file  tests/Math/st2tost2/StensorProductDerivative2.cxx
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
struct StensorProductDerivative2 final : public tfel::tests::TestCase {
  StensorProductDerivative2()
      : tfel::tests::TestCase("TFEL/Math",
                              "StensorProductDerivative2" + std::to_string(N)) {
  }  // end of StensorProductDerivative2
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
    const stensor<N> tmp = stensor<N>::Id() + 2 * eel;
    const stensor<N> S = D * eel;
    const tensor<N> M = tmp * S;
    st2tot2<N> ndM(double(0));
    const st2tot2<N, double> dM(2 * st2tot2<N, double>::tpld(S) +
                                st2tot2<N, double>::tprd(tmp, D));
    for (typename stensor<N>::size_type i = 0; i != eel.size(); ++i) {
      stensor<N> eel_e(eel);
      eel_e[i] += eps;
      const stensor<N> tmp_p = stensor<N>::Id() + 2 * eel_e;
      const stensor<N> S_p = D * eel_e;
      const tensor<N> M_p = tmp_p * S_p;
      eel_e[i] -= 2 * eps;
      const stensor<N> tmp_m = stensor<N>::Id() + 2 * eel_e;
      const stensor<N> S_m = D * eel_e;
      const tensor<N> M_m = tmp_m * S_m;
      for (typename tensor<N>::size_type j = 0; j != M.size(); ++j) {
        ndM(j, i) = (M_p(j) - M_m(j)) / (2 * eps);
      }
    }
    for (typename tensor<N>::size_type i = 0; i != M.size(); ++i) {
      for (typename stensor<N>::size_type j = 0; j != eel.size(); ++j) {
        TFEL_TESTS_ASSERT(abs(ndM(i, j) - dM(i, j)) < prec);
        if (abs(ndM(i, j) - dM(i, j)) > prec) {
          cout << "Error " << N << " (" << i << ", " << j << ") "
               << "[" << i * StensorDimeToSize<N>::value + j
               << "]: " << ndM(i, j) << " vs " << dM(i, j) << " "
               << abs(ndM(i, j) - dM(i, j)) << endl;
        }
      }
    }
    return this->result;
  }  // end of execute
};

using StensorProductDerivative2_1D = StensorProductDerivative2<1U>;
using StensorProductDerivative2_2D = StensorProductDerivative2<2U>;
using StensorProductDerivative2_3D = StensorProductDerivative2<3U>;
TFEL_TESTS_GENERATE_PROXY(StensorProductDerivative2_1D,
                          "StensorProductDerivative2-1D");
TFEL_TESTS_GENERATE_PROXY(StensorProductDerivative2_2D,
                          "StensorProductDerivative2-2D");
TFEL_TESTS_GENERATE_PROXY(StensorProductDerivative2_3D,
                          "StensorProductDerivative2-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("StensorProductDerivative2.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
