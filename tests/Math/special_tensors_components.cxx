/*!
 * \file  tests/Math/special_tensors_components.cxx
 * \brief This file checks that the overloads of `Id`, `IxI`, `K`, `M`, `J`
 * and `transpose_derivative` which take indices return the components of the
 * objects returned by the overloads without argument, with the base type
 * \author Tristan Chenaille
 * \date 15/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>
#include <type_traits>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tot2.hxx"

/*!
 * \return true if every overload taking indices returns the component of the
 * object returned by the overload without argument for the base type
 * \tparam N: space dimension
 * \tparam T: numeric type
 */
template <unsigned short N, typename T>
constexpr bool check() {
  using namespace tfel::math;
  using base = base_type<T>;
  using Stensor = stensor<N, T>;
  using Tensor = tensor<N, T>;
  using Stensor4 = st2tost2<N, T>;
  using Tensor4 = t2tot2<N, T>;
  using Matrix = tmatrix<3u, 3u, T>;
  // a is the value returned by an overload: it must have the base type
  constexpr auto differ = [](const auto a, const base b) {
    static_assert(std::is_same_v<decltype(a), const base>);
    return tfel::math::abs(a - b) > base{};
  };
  // objects built once: this keeps the evaluation at compile time short
  constexpr auto s_id = stensor<N, base>::Id();
  constexpr auto s4_id = st2tost2<N, base>::Id();
  constexpr auto s4_ixi = st2tost2<N, base>::IxI();
  constexpr auto s4_k = st2tost2<N, base>::K();
  constexpr auto s4_m = st2tost2<N, base>::M();
  constexpr auto s4_j = st2tost2<N, base>::J();
  constexpr auto t_id = tensor<N, base>::Id();
  constexpr auto t4_id = t2tot2<N, base>::Id();
  constexpr auto t4_ixi = t2tot2<N, base>::IxI();
  constexpr auto t4_k = t2tot2<N, base>::K();
  constexpr auto t4_dt = t2tot2<N, base>::transpose_derivative();
  constexpr auto m_id = tmatrix<3u, 3u, base>::Id();
  constexpr unsigned short ss = StensorDimeToSize<N>::value;
  constexpr unsigned short ts = TensorDimeToSize<N>::value;
  for (unsigned short i = 0; i != ss; ++i) {
    if (differ(Stensor::Id(i), s_id[i])) {
      return false;
    }
    for (unsigned short j = 0; j != ss; ++j) {
      if (differ(Stensor4::Id(i, j), s4_id(i, j)) ||
          differ(Stensor4::IxI(i, j), s4_ixi(i, j)) ||
          differ(Stensor4::K(i, j), s4_k(i, j)) ||
          differ(Stensor4::M(i, j), s4_m(i, j)) ||
          differ(Stensor4::J(i, j), s4_j(i, j))) {
        return false;
      }
    }
  }
  for (unsigned short i = 0; i != ts; ++i) {
    if (differ(Tensor::Id(i), t_id[i])) {
      return false;
    }
    for (unsigned short j = 0; j != ts; ++j) {
      if (differ(Tensor4::Id(i, j), t4_id(i, j)) ||
          differ(Tensor4::IxI(i, j), t4_ixi(i, j)) ||
          differ(Tensor4::K(i, j), t4_k(i, j)) ||
          differ(Tensor4::transpose_derivative(i, j), t4_dt(i, j))) {
        return false;
      }
    }
  }
  for (unsigned short i = 0; i != 3; ++i) {
    for (unsigned short j = 0; j != 3; ++j) {
      if (differ(Matrix::Id(i, j), m_id(i, j))) {
        return false;
      }
    }
  }
  return true;
}  // end of check

struct SpecialTensorsComponentsTest final : public tfel::tests::TestCase {
  SpecialTensorsComponentsTest()
      : tfel::tests::TestCase("TFEL/Math", "SpecialTensorsComponentsTest") {
  }  // end of SpecialTensorsComponentsTest

  tfel::tests::TestResult execute() override {
    TFEL_TESTS_STATIC_ASSERT((check<1u, double>()));
    TFEL_TESTS_STATIC_ASSERT((check<2u, double>()));
    TFEL_TESTS_STATIC_ASSERT((check<3u, double>()));
    TFEL_TESTS_STATIC_ASSERT((check<3u, float>()));
    // a quantity: the components have no unit
    using stress = tfel::math::qt<tfel::math::unit::Stress, double>;
    TFEL_TESTS_STATIC_ASSERT((check<3u, stress>()));
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(SpecialTensorsComponentsTest,
                          "SpecialTensorsComponentsTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("special_tensors_components.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
