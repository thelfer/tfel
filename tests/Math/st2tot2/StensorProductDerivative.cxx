/*!
 * \file  tests/Math/st2tost2/StensorProductDerivative.cxx
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
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
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
struct StensorProductDerivative final : public tfel::tests::TestCase {
  StensorProductDerivative()
      : tfel::tests::TestCase("TFEL/Math",
                              "StensorProductDerivative" + std::to_string(N)) {
  }  // end of StensorProductDerivative
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-3;
    const double prec = 1.e-10;
    stensor<N> t1;
    for (typename stensor<N>::size_type i = 0; i != t1.size(); ++i) {
      t1[i] = 0.3 + i;
    }
    stensor<N> t2;
    for (typename stensor<N>::size_type i = 0; i != t2.size(); ++i) {
      t2[i] = -0.45 + i;
    }
    st2tot2<N> ld = st2tot2<N>::tpld(t2);
    st2tot2<N> rd = st2tot2<N>::tprd(t1);
    st2tot2<N> nld;
    st2tot2<N> nrd;
    tensor<N> t3(t1 * t2);
    for (typename stensor<N>::size_type i = 0; i != t1.size(); ++i) {
      stensor<N> t1e(t1);
      stensor<N> t2e(t2);
      t1e[i] += eps;
      t2e[i] += eps;
      tensor<N> t3l_f(t1e * t2);
      tensor<N> t3r_f(t1 * t2e);
      t1e[i] -= 2 * eps;
      t2e[i] -= 2 * eps;
      tensor<N> t3l_b(t1e * t2);
      tensor<N> t3r_b(t1 * t2e);
      for (typename tensor<N>::size_type j = 0; j != t3.size(); ++j) {
        nld(j, i) = (t3l_f(j) - t3l_b(j)) / (2 * eps);
        nrd(j, i) = (t3r_f(j) - t3r_b(j)) / (2 * eps);
      }
    }
    for (typename tensor<N>::size_type i = 0; i != t3.size(); ++i) {
      for (typename stensor<N>::size_type j = 0; j != t1.size(); ++j) {
        TFEL_TESTS_ASSERT(abs(nld(i, j) - ld(i, j)) < prec);
        if (abs(nld(i, j) - ld(i, j)) > prec) {
          cout << "Error " << N << " nld (" << i << ", " << j << ") "
               << "[" << i * StensorDimeToSize<N>::value + j
               << "]: " << nld(i, j) << " vs " << ld(i, j) << " "
               << abs(nld(i, j) - ld(i, j)) << endl;
        }
      }
    }
    for (typename tensor<N>::size_type i = 0; i != t3.size(); ++i) {
      for (typename stensor<N>::size_type j = 0; j != t1.size(); ++j) {
        TFEL_TESTS_ASSERT(abs(nrd(i, j) - rd(i, j)) < prec);
        if (abs(nrd(i, j) - rd(i, j)) > prec) {
          cout << "Error " << N << " nrd (" << i << ", " << j << ") "
               << "[" << i * StensorDimeToSize<N>::value + j
               << "]: " << nrd(i, j) << " vs " << rd(i, j) << " "
               << abs(nrd(i, j) - rd(i, j)) << endl;
        }
      }
    }
    return this->result;
  }  // end of execute
};

using StensorProductDerivative_1D = StensorProductDerivative<1U>;
using StensorProductDerivative_2D = StensorProductDerivative<2U>;
using StensorProductDerivative_3D = StensorProductDerivative<3U>;
TFEL_TESTS_GENERATE_PROXY(StensorProductDerivative_1D,
                          "StensorProductDerivative-1D");
TFEL_TESTS_GENERATE_PROXY(StensorProductDerivative_2D,
                          "StensorProductDerivative-2D");
TFEL_TESTS_GENERATE_PROXY(StensorProductDerivative_3D,
                          "StensorProductDerivative-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("StensorProductDerivative.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
