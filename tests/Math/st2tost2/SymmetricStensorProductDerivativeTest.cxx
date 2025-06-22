/*!
 * \file   SymmetricStensorProductDerivativeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 avril 2017
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
#include "TFEL/Math/ST2toST2/SymmetricStensorProductDerivative.hxx"

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
struct SymmetricStensorProductDerivativeTest final
    : public tfel::tests::TestCase {
  SymmetricStensorProductDerivativeTest()
      : tfel::tests::TestCase(
            "TFEL/Math",
            "SymmetricStensorProductDerivativeTest" + std::to_string(N)) {
  }  // end of SymmetricStensorProductDerivativeTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using size_type = typename stensor<N>::size_type;
    const double eps = 1.e-3;
    const double prec = 1.e-10;
    stensor<N> t1;
    for (size_type i = 0; i != t1.size(); ++i) {
      t1[i] = 0.3 + i;
    }
    stensor<N> t2;
    for (size_type i = 0; i != t2.size(); ++i) {
      t2[i] = -0.45 + i;
    }
    const auto t3 = symmetric_product_aba(t1, t2);
    const tensor<N> ut4 = t1 * t2 * t1;
    const stensor<N> t4 = syme(ut4);
    const auto d = symmetric_product_derivative_daba_da(t1, t2);
    for (size_type i = 0; i != t3.size(); ++i) {
      TFEL_TESTS_ASSERT(std::abs(t4(i) - t3(i)) < prec);
    }
    st2tost2<N> nd;
    for (size_type i = 0; i != t1.size(); ++i) {
      stensor<N> t1e(t1);
      t1e[i] += eps;
      stensor<N> t3e_p = symmetric_product_aba(t1e, t2);
      t1e[i] -= 2 * eps;
      stensor<N> t3e_m = symmetric_product_aba(t1e, t2);
      for (typename tensor<N>::size_type j = 0; j != t3.size(); ++j) {
        nd(j, i) = (t3e_p(j) - t3e_m(j)) / (2 * eps);
      }
    }
    for (size_type i = 0; i != t3.size(); ++i) {
      for (size_type j = 0; j != t1.size(); ++j) {
        TFEL_TESTS_ASSERT(std::abs(nd(i, j) - d(i, j)) < prec);
        if (abs(nd(i, j) - d(i, j)) > prec) {
          std::cout << "Error " << N << " nd (" << i << ", " << j << ") "
                    << "[" << i * StensorDimeToSize<N>::value + j
                    << "]: " << nd(i, j) << " vs " << d(i, j) << " "
                    << std::abs(nd(i, j) - d(i, j)) << std::endl;
        }
      }
    }
    return this->result;
  }  // end of execute
};

using SymmetricStensorProductDerivativeTest_1D =
    SymmetricStensorProductDerivativeTest<1u>;
using SymmetricStensorProductDerivativeTest_2D =
    SymmetricStensorProductDerivativeTest<2u>;
using SymmetricStensorProductDerivativeTest_3D =
    SymmetricStensorProductDerivativeTest<3u>;
TFEL_TESTS_GENERATE_PROXY(SymmetricStensorProductDerivativeTest_1D,
                          "SymmetricStensorProductDerivativeTest-1D");
TFEL_TESTS_GENERATE_PROXY(SymmetricStensorProductDerivativeTest_2D,
                          "SymmetricStensorProductDerivativeTest-2D");
TFEL_TESTS_GENERATE_PROXY(SymmetricStensorProductDerivativeTest_3D,
                          "SymmetricStensorProductDerivativeTest-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("SymmetricStensorProductDerivative.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
