/*!
 * \file   deprecated_views_names.cxx
 * \brief  This file checks that the former names of the scattered and strided
 * views, which are deprecated, can still be used.
 * \author Tristan Chenaille
 * \date 24/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

// the former names and the former headers are deprecated
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-W#pragma-messages"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

#include <array>
#include <cstdlib>
#include <iostream>
#include <type_traits>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Array/CoalescedView.hxx"
#include "TFEL/Math/Array/StridedCoalescedView.hxx"
#include "TFEL/Math/Array/StridedCoalescedViewsArray.hxx"
#include "TFEL/Math/Array/StridedCoalescedViewsArrayIO.hxx"

//! \brief a metafunction specialised using the former names
template <typename T>
struct IsCoalescedView : std::false_type {};

template <tfel::math::MappableMathObjectUsingCoalescedViewConcept MappedType,
          typename IndexingPolicyType>
struct IsCoalescedView<
    tfel::math::CoalescedView<MappedType, IndexingPolicyType>>
    : std::true_type {};

struct DeprecatedViewsNamesTest final : public tfel::tests::TestCase {
  DeprecatedViewsNamesTest()
      : tfel::tests::TestCase("TFEL/Math", "DeprecatedViewsNamesTest") {
  }  // end of DeprecatedViewsNamesTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    using Stensor = stensor<2u, int>;
    using IndexingPolicy = FixedSizeVectorIndexingPolicy<unsigned short, 2>;
    // the former names are aliases of the new ones
    static_assert(
        std::is_same_v<CoalescedView<Stensor>, ScatteredView<Stensor>>);
    static_assert(std::is_same_v<ConstCoalescedView<Stensor>,
                                 ConstScatteredView<Stensor>>);
    static_assert(
        std::is_same_v<StridedCoalescedView<Stensor>, StridedView<Stensor>>);
    static_assert(std::is_same_v<ConstStridedCoalescedView<Stensor>,
                                 ConstStridedView<Stensor>>);
    static_assert(
        std::is_same_v<StridedCoalescedViewsArray<Stensor, IndexingPolicy>,
                       StridedViewsArray<Stensor, IndexingPolicy>>);
    static_assert(
        std::is_same_v<
            StridedCoalescedViewsFixedSizeVector<Stensor, unsigned short, 2>,
            StridedViewsFixedSizeVector<Stensor, unsigned short, 2>>);
    static_assert(MappableMathObjectUsingCoalescedViewConcept<Stensor>);
    static_assert(MappableMutableMathObjectUsingCoalescedViewConcept<Stensor>);
    static_assert(
        MappableImmutableMathObjectUsingCoalescedViewConcept<const Stensor>);
    static_assert(IsCoalescedView<ScatteredView<Stensor>>::value);
    static_assert(!IsCoalescedView<StridedView<Stensor>>::value);
    // SoA layout: [s0_c0, s1_c0, s0_c1, s1_c1, s0_c2, s1_c2, s0_c3, s1_c3]
    int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
    std::array ptrs{&values[0], &values[2], &values[4], &values[6]};
    const CoalescedView<Stensor> s1{ptrs};
    const StridedCoalescedView<Stensor> s2{&values[1], 2};
    const StridedCoalescedViewsFixedSizeVector<Stensor, unsigned short, 2> a{
        &values[0]};
    TFEL_TESTS_ASSERT(s1[2] == 3);
    TFEL_TESTS_ASSERT(s2[2] == 30);
    TFEL_TESTS_ASSERT(a[1][3] == 40);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(DeprecatedViewsNamesTest, "DeprecatedViewsNamesTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("DeprecatedViewsNames.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
