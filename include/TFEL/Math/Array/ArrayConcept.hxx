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

  template <typename T>
  struct ArrayConcept {
    //! \brief type alias
    typedef ArrayTag ConceptTag;

   protected:
    ArrayConcept() = default;
    ArrayConcept(ArrayConcept&&) = default;
    ArrayConcept(const ArrayConcept&) = default;
    ArrayConcept& operator=(const ArrayConcept&) = default;
    ~ArrayConcept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements
   * the `ArrayConcept`.
   * \tparam ArrayType: type tested
   */
  template <typename ArrayType>
  constexpr bool implementsArrayConcept() {
    return tfel::meta::implements<ArrayType, ArrayConcept>();
  }  // end of implementsArrayConcept

  //! paratial specialisation for arrays
  template <typename Type>
  struct ConceptRebind<ArrayTag, Type> {
    using type = ArrayConcept<Type>;
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

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_CONCEPT_HXX */
