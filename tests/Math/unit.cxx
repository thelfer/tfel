/*!
 * \file   tests/Math/unit.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02/08/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "TFEL/Math/Quantity/Unit.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct UnitTest final : public tfel::tests::TestCase {
  UnitTest() : tfel::tests::TestCase("TFEL/Math", "Unit") {}  // end of UnitTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math::unit;
    TFEL_TESTS_STATIC_ASSERT(exponents<NoUnit> == makeUnitExponents<>());
    TFEL_TESTS_STATIC_ASSERT(!(exponents<NoUnit> == exponents<Time>));
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<NoUnit, UnitBase<makeUnitExponents<>()>>));
    TFEL_TESTS_STATIC_ASSERT(
        (!areUnitsEqual<NoUnit, UnitBase<makeUnitExponents<1>()>>));
    TFEL_TESTS_STATIC_ASSERT((areUnitsEqual<NoUnit, StandardUnit<>>));
    TFEL_TESTS_STATIC_ASSERT((!areUnitsEqual<NoUnit, StandardUnit<1>>));
    constexpr auto e0 = UnitExponent{.numerator = 0, .denominator = 0};
    constexpr auto e1 = UnitExponent{.numerator = 1, .denominator = 2};
    constexpr auto e2 = UnitExponent{.numerator = 4, .denominator = 2};
    TFEL_TESTS_STATIC_ASSERT(!isValid(e0));
    TFEL_TESTS_STATIC_ASSERT(isValid(e1));
    TFEL_TESTS_STATIC_ASSERT(isIrreductible(e1));
    TFEL_TESTS_STATIC_ASSERT(!isIrreductible(e2));
    //
    constexpr auto ue0 =
        UnitExponents{.exponents = {e0, e1, e1, e1, e1, e1, e1}};
    constexpr auto ue1 =
        UnitExponents{.exponents = {e1, e1, e1, e1, e1, e1, e1}};
    constexpr auto ue2 =
        UnitExponents{.exponents = {e2, e1, e1, e1, e1, e1, e1}};
    constexpr auto ue3 =
        makeUnitExponents<-1, 2, -3, 4, -5, 6, -7, 3, 9, 23, 5, 7, 13>();
    TFEL_TESTS_STATIC_ASSERT(!isValid(ue0));
    TFEL_TESTS_STATIC_ASSERT(isValid(ue1));
    TFEL_TESTS_STATIC_ASSERT(!isValid(ue2));
    TFEL_TESTS_STATIC_ASSERT(isValid(ue3));
    TFEL_TESTS_STATIC_ASSERT(
        (ue3.exponents[0] == UnitExponent{.numerator = -1, .denominator = 3}));
    TFEL_TESTS_STATIC_ASSERT(
        (ue3.exponents[1] == UnitExponent{.numerator = 2, .denominator = 9}));
    TFEL_TESTS_STATIC_ASSERT(
        (ue3.exponents[2] == UnitExponent{.numerator = -3, .denominator = 23}));
    TFEL_TESTS_STATIC_ASSERT(
        (ue3.exponents[3] == UnitExponent{.numerator = 4, .denominator = 5}));
    TFEL_TESTS_STATIC_ASSERT(
        (ue3.exponents[4] == UnitExponent{.numerator = -5, .denominator = 7}));
    TFEL_TESTS_STATIC_ASSERT(
        (ue3.exponents[5] == UnitExponent{.numerator = 6, .denominator = 13}));
    TFEL_TESTS_STATIC_ASSERT(
        (ue3.exponents[6] == UnitExponent{.numerator = -7, .denominator = 1}));
  }
  void test2() {
    using namespace tfel::math::unit;
    constexpr auto e1 = UnitExponent{.numerator = 1, .denominator = 2};
    constexpr auto e2 = UnitExponent{.numerator = 2, .denominator = 3};
    constexpr auto e3 = UnitExponent{.numerator = 7, .denominator = 6};
    constexpr auto e4 = UnitExponent{.numerator = 1, .denominator = 3};
    TFEL_TESTS_STATIC_ASSERT((e3 == add(e1, e2)));
    TFEL_TESTS_STATIC_ASSERT((e4 == multiply(e1, e2)));
  }
  void test3() {
    using namespace tfel::math::unit;
    constexpr auto u0 = exponents<Time>;
    constexpr auto u1 = exponents<Mass>;
    constexpr auto u3 = makeUnitExponents<1, 0, 0, 0, 0, 0, 0, 2>();
    constexpr auto u4 = makeUnitExponents<3, 0, 0, 0, 0, 0, 0, 2>();
    constexpr auto u5 = makeUnitExponents<2, 0, 0, 0, 0, 0, 0, 3>();
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<UnitBase<add(u0, u0)>, StandardUnit<0, 0, 2>>));
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<UnitBase<add(u0, u1)>, StandardUnit<1, 0, 1>>));
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<UnitBase<add(u0, add(u0, u0))>, StandardUnit<0, 0, 3>>));
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<UnitBase<multiply(add(u0, add(u0, u0)), add(u0, u0))>,
                       StandardUnit<0, 0, 6>>));
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<UnitBase<add(u3, u4)>, StandardUnit<2>>));
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<UnitBase<subtract(u3, u4)>, StandardUnit<-1>>));
    TFEL_TESTS_STATIC_ASSERT(
        (areUnitsEqual<UnitBase<add(u3, u5)>, Unit<7, 0, 0, 0, 0, 0, 0, 6>>));
    TFEL_TESTS_STATIC_ASSERT(areAllDenominatorsOne(exponents<Length>));
    TFEL_TESTS_STATIC_ASSERT((
        std::same_as<typename UnitRebind<makeUnitExponents<1>()>::type, Mass>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::same_as<typename UnitRebind<makeUnitExponents<0, 1>()>::type,
                      Length>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::same_as<typename UnitRebind<makeUnitExponents<0, 0, 1>()>::type,
                      Time>));
    TFEL_TESTS_STATIC_ASSERT((
        std::same_as<typename UnitRebind<makeUnitExponents<0, 0, 0, 1>()>::type,
                     Ampere>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::same_as<
            typename UnitRebind<makeUnitExponents<0, 0, 0, 0, 1>()>::type,
            Temperature>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::same_as<
            typename UnitRebind<makeUnitExponents<0, 0, 0, 0, 0, 1>()>::type,
            Candela>));
    TFEL_TESTS_STATIC_ASSERT(
        (std::same_as<
            typename UnitRebind<makeUnitExponents<0, 0, 0, 0, 0, 0, 1>()>::type,
            Mole>));
  }
};  // end of struct UnitTest

TFEL_TESTS_GENERATE_PROXY(UnitTest, "UnitTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Unit.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
