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
  using stress = typename Types::stress;
  using StressStensor = typename Types::StressStensor;
  static_assert(std::same_as<typename TypeRebind<StressStensor>::time,
                             typename Types::time>);
  static_assert(std::same_as<typename ScalarTypeRebind<stress>::time,
                             typename Types::time>);
  static_assert(
      std::is_same<stress,
                   tfel::math::qt<unit::Stress, base_type<stress>>>::value);
  static_assert(CheckUnitCompatibility<unit::Stress, stress>::value);
  static_assert(CheckUnitCompatibility<unit::Stress, const stress>::value);
  static_assert(CheckUnitCompatibility<unit::Stress,
                                       qt_ref<unit::Stress, double>>::value);
  static_assert(
      CheckUnitCompatibility<unit::Stress,
                             const qt_ref<unit::Stress, double>>::value);
}

int main() {
  test<true>();
  //  test<false>();
  return EXIT_SUCCESS;
}
