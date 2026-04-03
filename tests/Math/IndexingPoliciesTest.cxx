/*!
 * \file  IndexingPoliciesTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   07 janv. 2015
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
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Array/FixedSizeIndexingPolicies.hxx"

namespace tfel::math {

  //   template <typename MathObject1, typename MathObject2 unsigned short Mn,
  //   unsigned short Mm, unsigned short In, unsigned short Im, > struct
  //   DerivateTypeViewFromTinyMatrix;
  //
  //   template <typename MathObject1, typename MathObject2 unsigned short Mn,
  //   unsigned short Mm, unsigned short In, unsigned short Im, > struct
  //   MathObjectTraits
  //       : MathObjectTraitsBase<result_type<numeric_type<MathObject1>,
  //                                          numeric_type<MathObject2>,
  //                                          OpDiv>,
  //                              std::common_type_t<index_type<MathObject1>,
  //                                                 index_type<MathObject2>>> {
  //     static_assert(getSpaceDimension<MathObject1>() ==
  //                   getSpaceDimension<MathObject2>());
  //     //! \brief space dimension
  //     static constexpr unsigned short dime =
  //     getSpaceDimension<MathObject1>();
  //   };  // end of struct MathObjectTraits
  //
  //   template <typename MathObject1, typename MathObject2 unsigned short Mn,
  //   unsigned short Mm, unsigned short In, unsigned short Im, > struct
  //   DerivateTypeViewFromTinyMatrix : View<> {
  //     //! \brief a simple alias
  // using container =
  //     //! \brief
  //     DerivateTypeViewFromTinyMatrix(const) protected
  //         : tmatrix<N, M, numeric_type<DerivateTypeViewFromTinyMatrix>>& m;
  //   };  // end of struct DerivateTypeViewFromTinyMatrix

}  // end of namespace tfel::math

struct IndexingPoliciesTest final : public tfel::tests::TestCase {
  IndexingPoliciesTest()
      : tfel::tests::TestCase("TFEL/Math", "IndexingPoliciesTest") {
  }  // end of IndexingPoliciesTest
  tfel::tests::TestResult execute() override {
    this->test1();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using namespace tfel::math;
    using IndexingPolicy1 = FixedSizeVectorIndexingPolicy<unsigned short, 3>;
    using IndexingPolicy2 = FixedSizeVectorIndexingPolicy<unsigned short, 2>;
    using IndexingPolicy =
        FixedSizeIndexingPoliciesCartesianProduct<IndexingPolicy1,
                                                  IndexingPolicy2>;
    constexpr IndexingPolicy p;
    TFEL_TESTS_STATIC_ASSERT(IndexingPolicy::arity == 2);
    TFEL_TESTS_STATIC_ASSERT(IndexingPolicy::hasFixedSizes);
    TFEL_TESTS_STATIC_ASSERT(IndexingPolicy::areDataContiguous);
    TFEL_TESTS_STATIC_ASSERT(p.size() == 6);
    TFEL_TESTS_STATIC_ASSERT(p.size(0) == 3);
    TFEL_TESTS_STATIC_ASSERT(p.size(1) == 2);
    TFEL_TESTS_STATIC_ASSERT(p.getUnderlyingArrayMinimalSize() == 6);
    TFEL_TESTS_STATIC_ASSERT(p.getIndex(0, 0) == 0);
    TFEL_TESTS_STATIC_ASSERT(p.getIndex(0, 1) == 1);
    TFEL_TESTS_STATIC_ASSERT(p.getIndex(1, 0) == 2);
    TFEL_TESTS_STATIC_ASSERT(p.getIndex(1, 1) == 3);
  }  // end of test1
};

TFEL_TESTS_GENERATE_PROXY(IndexingPoliciesTest, "IndexingPoliciesTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("IndexingPoliciesTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
