/*!
 * \file   tests/Math/types.cxx
 * \author Thomas Helfer
 * \date   11/11/2024
 */

#include "TFEL/Math/types.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/stensor.hxx"

template <bool qt>
void test() {
  using namespace tfel::math;
  using Types = tfel::config::Types<3, float, qt>;
  using stress = Types::stress;
  using StressStensor = Types::StressStensor;
  static_assert(std::same_as<typename TypeRebind<StressStensor>::time,
                             typename Types::time>);
  static_assert(std::same_as<typename ScalarTypeRebind<stress>::time,
                             typename Types::time>);
  static_assert(
      std::is_same<stress, tfel::math::qt<Stress, base_type<stress>>>::value);
  static_assert(CheckUnitCompatibility<Stress, stress>::value);
  static_assert(CheckUnitCompatibility<Stress, const stress>::value);
  static_assert(CheckUnitCompatibility<Stress, qt_ref<Stress, double>>::value);
  static_assert(
      CheckUnitCompatibility<Stress, const qt_ref<Stress, double>>::value);
}

int main() {
  test<true>();
  //  test<false>();
  return EXIT_SUCCESS;
}
