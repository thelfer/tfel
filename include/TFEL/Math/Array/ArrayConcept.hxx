/*!
 * \file   include/TFEL/Math/Array/ArrayConcept.hxx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_CONCEPT_HXX
#define LIB_TFEL_MATH_ARRAY_CONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"

namespace tfel::math {

  /*!
   * \class ArrayTag
   * \brief Helper class to characterise arrays.
   */
  struct ArrayTag {};  // end of ArrayTag

  /*!
   * \class ArrayConceptBase
   * \brief A class used to model the concept of arrays.
   */
  template <typename T>
  struct ArrayConceptBase {
    using ConceptTag = ArrayTag;
  };

  /*!
   * \brief definition of the ArrayConcept concept
   * a class matching the array concept must expose the `ArrayTag` and have
   * access operators.
   */
  template <typename ArrayType>
  concept ArrayConcept =
      (std::is_same_v<typename std::decay_t<ArrayType>::ConceptTag,
                      ArrayTag>)&&  //
      (requires(const ArrayType t, const index_type<ArrayType> i) {
        t[i];
      }) &&  //
      (requires(const ArrayType t, const index_type<ArrayType> i) { t(i); });

  //! paratial specialisation for arrays
  template <typename ArrayType>
  struct ConceptRebind<ArrayTag, ArrayType> {
    using type = ArrayConceptBase<ArrayType>;
  };

  //! \brief partial specialisation of ComputeUnaryResult_ for arrays
  template <typename A>
  class ComputeUnaryResult_<ArrayTag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    using ArrayTypeA = EvaluationResult<A>;

   public:
    using Result = typename UnaryResultType<ArrayTypeA, OpNeg>::type;
    using Handle = std::conditional_t<isInvalid<Result>(),
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  /*!
   * \brief an helper function which returns if the given type implements
   * the `ArrayConcept`.
   * \tparam ArrayType: type tested
   */
  template <typename ArrayType>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsArrayConcept() {
    // return tfel::meta::implements<ArrayType, ArrayConceptBase>;
    return ArrayConcept<ArrayType>;
  }  // end of implementsArrayConcept

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_CONCEPT_HXX */
