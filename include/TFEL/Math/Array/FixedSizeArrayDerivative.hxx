/*!
 * \file   TFEL/Math/Array/FixedSizeArrayDerivative.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FIXEDSIZEARRAYDERIVATIVE_HXX
#define LIB_TFEL_MATH_FIXEDSIZEARRAYDERIVATIVE_HXX

#include <type_traits>
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/FixedSizeArrayPolicies.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Array/ConstView.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"

namespace tfel::math {

  /*!
   * \brief an array policy resulting from the derivation of an math object with
   * another math object.
   * \tparam ArrayPolicy1: array policy of the derivated array
   * \tparam ArrayPolicy2: array policy of the derivated array
   */
  template <typename ArrayPolicy1, typename ArrayPolicy2>
  struct FixedSizeArrayDerivativePolicy
      : StandardArrayPolicyAliases<
            result_type<typename ArrayPolicy1::value_type,
                        typename ArrayPolicy2::value_type,
                        OpDiv>> {
    static_assert(ArrayPolicy1::IndexingPolicy::hasFixedSizes);
    static_assert(ArrayPolicy2::IndexingPolicy::hasFixedSizes);
    //! \brief indexing policy
    using IndexingPolicy = FixedSizeIndexingPoliciesCartesianProduct<
        typename ArrayPolicy1::IndexingPolicy,
        typename ArrayPolicy2::IndexingPolicy>;
  }; // end of struct FixedSizeArrayDerivativePolicy

  /*!
   * \brief a tag associated with the FixedSizeArrayDerivativeConcept
   */
  template <typename ArrayTag1, typename ArrayTag2>
  struct FixedSizeArrayDerivativeTag {};
  /*!
   * \brief a traits class associated to characterize the result of the
   * derivation of an array with respect to another array.
   */
  template <typename ArrayDerivativeType>
  struct FixedSizeArrayDerivativeTraits {
    //! \brief derivatived array
    using type1 = tfel::meta::InvalidType;
    //! \brief array used to derivate the first array
    using type2 = tfel::meta::InvalidType;
  };  // end of struct FixedSizeArrayDerivativeTraits

  /*!
   * \brief a tag associated with the FixedSizeArrayDerivativeConcept
   */
  template <typename ArrayDerivativeType>
  struct FixedSizeArrayDerivativeConcept {
    //! \brief concept tag
    using ConceptTag = FixedSizeArrayDerivativeTag<
        typename ComputeObjectTag<typename FixedSizeArrayDerivativeTraits<
            ArrayDerivativeType>::type1>::type,
        typename ComputeObjectTag<typename FixedSizeArrayDerivativeTraits<
            ArrayDerivativeType>::type2>::type>;
  };  // end of struct FixedSizeArrayDerivativeConcept

  /*!
   * \brief a general implementation of an object representing the
   * derivation of an object of type `Array1` with respect to an object of
   * type `Array2`
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   */
  template <typename Array1, typename Array2>
  struct FixedSizeArrayDerivative;

  /*!
   * \brief paratial specialisation of the `FixedSizeArrayDerivativeTraits`
   * class
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   */
  template <typename Array1, typename Array2>
  struct FixedSizeArrayDerivativeTraits<
      FixedSizeArrayDerivative<Array1, Array2>> {
    //! \brief derivatived array
    using type1 = Array1;
    //! \brief array used to derivate the first array
    using type2 = Array1;
  }; // end of struct FixedSizeArrayDerivativeTraits

  template <typename Array1, typename Array2>
  struct FixedSizeArrayDerivative
      : FixedSizeArrayDerivativeConcept<
            FixedSizeArrayDerivative<Array1, Array2>>,
        GenericFixedSizeArray<
            FixedSizeArrayDerivative<Array1, Array2>,
            FixedSizeArrayDerivativePolicy<typename Array1::array_policy,
                                           typename Array2::array_policy>> {
    static_assert(getSpaceDimension<Array1>() == getSpaceDimension<Array2>());
    //! \brief a simple alias
    using GenericFixedSizeArrayBase = GenericFixedSizeArray<
        FixedSizeArrayDerivative,
        FixedSizeArrayDerivativePolicy<typename Array1::array_policy,
                                       typename Array2::array_policy>>;
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(FixedSizeArrayDerivative,
                                               GenericFixedSizeArrayBase);
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
  };  // end of struct DerivativeArray

  /*!
   * \brief partial specialisation of the `MathObjectTraits` class.
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   */
  template <typename Array1, typename Array2>
  struct MathObjectTraits<FixedSizeArrayDerivative<Array1, Array2>>
      : MathObjectTraitsFromArrayPolicy<
            typename FixedSizeArrayDerivative<Array1, Array2>::array_policy> {
    //! \brief space dimension of the first array
    static constexpr auto array1_dime = getSpaceDimension<Array1>();
    //! \brief space dimension of the second array
    static constexpr auto array2_dime = getSpaceDimension<Array2>();
    //! \brief space dimension
    static constexpr unsigned short dime =
        (array1_dime == array2_dime) ? array1_dime : 0;
  };  // end of MathObjectTraits

  /*!
   * \brief paratial specialisation of the `FixedSizeArrayDerivativeTraits`
   * class for expression
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   * \tparam Operation: operation represented by the expression
   */
  template <typename Array1, typename Array2, typename Operation>
  struct FixedSizeArrayDerivativeTraits<
      Expr<FixedSizeArrayDerivative<Array1, Array2>, Operation>> {
    //! \brief derivatived array
    using type1 = Array1;
    //! \brief array used to derivate the first array
    using type2 = Array1;
  };  // end of struct FixedSizeArrayDerivativeTraits

  /*!
   * \brief paratial specialisation of the `FixedSizeArrayDerivativeTraits`
   * class for a view
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   */
  template <typename Array1, typename Array2>
  struct FixedSizeArrayDerivativeTraits<
      View<FixedSizeArrayDerivative<Array1, Array2>>> {
    //! \brief derivatived array
    using type1 = Array1;
    //! \brief array used to derivate the first array
    using type2 = Array1;
  };  // end of struct FixedSizeArrayDerivativeTraits

  /*!
   * \brief paratial specialisation of the `FixedSizeArrayDerivativeTraits`
   * class for a const view
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   */
  template <typename Array1, typename Array2>
  struct FixedSizeArrayDerivativeTraits<
      ConstView<FixedSizeArrayDerivative<Array1, Array2>>> {
    //! \brief derivatived array
    using type1 = Array1;
    //! \brief array used to derivate the first array
    using type2 = Array1;
  };  // end of struct FixedSizeArrayDerivativeTraits

}  // end of namespace tfel::math

namespace tfel::typetraits {

  /*!
   * \brief partial specialisation of the `IsAssignable` class.
   * \tparam Array11: type of the first array of the first array derivative
   * \tparam Array12: type of the second array of the first array derivative
   * \tparam Array21: type of the first array of the second array derivative
   * \tparam Array22: type of the second array of the second array derivative
   */
  template <typename Array11,
            typename Array12,
            typename Array21,
            typename Array22>
  struct IsAssignableTo<
      tfel::math::FixedSizeArrayDerivative<Array11, Array12>,
      tfel::math::FixedSizeArrayDerivative<Array21, Array22>> {
    //! \brief first condition to be met
    static constexpr bool cond1 = isAssignableTo<
        tfel::math::numeric_type<
            tfel::math::FixedSizeArrayDerivative<Array11, Array12>>,
        tfel::math::numeric_type<
            tfel::math::FixedSizeArrayDerivative<Array21, Array22>>>();
    //! \brief second condition to be met
    static constexpr bool cond2 =
        tfel::math::getSpaceDimension<
            tfel::math::FixedSizeArrayDerivative<Array11, Array12>>() ==
        tfel::math::getSpaceDimension<
            tfel::math::FixedSizeArrayDerivative<Array21, Array22>>();
    //! \brief third condition to be met
    static constexpr bool cond3 =
        std::is_same_v<typename tfel::math::ComputeObjectTag<Array11>::type,
                       typename tfel::math::ComputeObjectTag<Array21>::type>;
    //! \brief fourth condition to be met
    static constexpr bool cond4 =
        std::is_same_v<typename tfel::math::ComputeObjectTag<Array12>::type,
                       typename tfel::math::ComputeObjectTag<Array22>::type>;
    //! \brief result
    static constexpr bool cond = cond1 && cond2 && cond3 && cond4;
  };  // end of struct IsAssignableTo

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_FIXEDSIZEARRAYDERIVATIVE_HXX */
