/*!
 * \file   TFEL/Math/Array/FixedSizeArrayDerivative.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FIXEDSIZEARRAYDERIVATIVE_HXX
#define LIB_TFEL_MATH_FIXEDSIZEARRAYDERIVATIVE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/Array/FixedSizeArrayPolicies.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
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
      : StandardArrayPolicy<result_type<typename ArrayPolicy1::value_type,
                                        typename ArrayPolicy2::value_type,
                                        OpDiv>,
                            FixedSizeIndexingPoliciesCartesianProduct<
                                typename ArrayPolicy1::IndexingPolicy,
                                typename ArrayPolicy2::IndexingPolicy>> {
    static_assert(ArrayPolicy1::IndexingPolicy::hasFixedSizes);
    static_assert(ArrayPolicy2::IndexingPolicy::hasFixedSizes);
  };  // end of struct FixedSizeArrayDerivativePolicy

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
   * \tparam ArrayDerivativeType: type of the array derivative
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
   * \brief partial specialisation of the `ConceptRebind` class for array
   * derivative.
   * \tparam Array1Tag: tag of function
   * \tparam Array2Tag: tag of variable
   * \tparam ArrayDerivativeType: type of the array derivative
   */
  template <typename Array1Tag,
            typename Array2Tag,
            typename ArrayDerivativeType>
  struct ConceptRebind<FixedSizeArrayDerivativeTag<Array1Tag, Array2Tag>,
                       ArrayDerivativeType> {
    //! \brief a simple alias
    using type = FixedSizeArrayDerivativeConcept<ArrayDerivativeType>;
  };
  /*!
   * \brief a class representing the derivative of an array with respect to
   * another array.
   * \tparam Array1: derivated array
   * \tparam Array2: array used to derivate the first array.
   */
  template <typename Array1, typename Array2>
  struct FixedSizeArrayDerivative
      : FixedSizeArrayDerivativeConcept<
            FixedSizeArrayDerivative<Array1, Array2>>,
        GenericFixedSizeArray<
            FixedSizeArrayDerivative<Array1, Array2>,
            FixedSizeArrayDerivativePolicy<typename Array1::array_policy,
                                           typename Array2::array_policy>> {
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
   * \brief partial specialisation of the `FixedSizeArrayDerivativeTraits`
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
    using type2 = Array2;
  };  // end of struct FixedSizeArrayDerivativeTraits
  /*!
   * \brief partial specialisation of the `FixedSizeArrayDerivativeTraits`
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
    using type2 = Array2;
  };  // end of struct FixedSizeArrayDerivativeTraits

  /*!
   * \brief partial specialisation of the `FixedSizeArrayDerivativeTraits`
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
    using type2 = Array2;
  };  // end of struct FixedSizeArrayDerivativeTraits
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
   * \brief partial specialisation for unary operations
   */
  template <typename Array1, typename Array2>
  class UnaryResultType<FixedSizeArrayDerivative<Array1, Array2>, OpNeg> {
    //! \brief result of the negation of the derivated type
    using NegatedArray = unary_result_type<Array1, OpNeg>;

   public:
    //! \brief result of the negation operation
    using type =
        std::conditional_t<isInvalid<NegatedArray>(),
                           tfel::meta::InvalidType,
                           FixedSizeArrayDerivative<NegatedArray, Array2>>;
  };  // end of class UnaryResultType
  /*!
   * \brief partial specialisation for the addition of two array derivatives.
   * \see   ComputeBinaryOperationResult
   * \tparam Array11: type of the first array for the first array derivative
   * \tparam Array12: type of the second array for the first array derivative
   * \tparam Array21: type of the first array for the second array derivative
   * \tparam Array22: type of the second array for the second array derivative
   */
  template <typename Array11,
            typename Array12,
            typename Array21,
            typename Array22>
  class ComputeBinaryOperationResult<
      FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array11>::type,
                                  typename ComputeObjectTag<Array12>::type>,
      FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array21>::type,
                                  typename ComputeObjectTag<Array22>::type>,
      FixedSizeArrayDerivative<Array11, Array12>,
      FixedSizeArrayDerivative<Array21, Array22>,
      OpPlus> {
    //! \brief check if the two derivatives are compatible
    static constexpr auto areCompatible =
        std::is_same_v<typename ComputeObjectTag<Array11>::type,
                       typename ComputeObjectTag<Array21>::type> &&
        std::is_same_v<typename ComputeObjectTag<Array12>::type,
                       typename ComputeObjectTag<Array22>::type>;
    /*!
     * \brief numeric type of the resulting array
     * Here we make a tricky choice and it is not totally clean.
     */
    using NumType = result_type<
        result_type<numeric_type<Array11>, numeric_type<Array21>, OpPlus>,
        result_type<numeric_type<Array12>, numeric_type<Array22>, OpPlus>,
        OpDiv>;

   public:
    //! \brief result
    using type = std::conditional_t<
        areCompatible && isInvalid<NumType>(),
        tfel::meta::InvalidType,
        FixedSizeArrayDerivative<result_type<Array11, Array21, OpPlus>,
                                 result_type<Array11, Array21, OpPlus>>>;
  };
  /*!
   * \brief partial specialisation for the addition of two array derivatives.
   * \see   ComputeBinaryOperationResult
   * \tparam Array11: type of the first array for the first array derivative
   * \tparam Array12: type of the second array for the first array derivative
   * \tparam Array21: type of the first array for the second array derivative
   * \tparam Array22: type of the second array for the second array derivative
   */
  template <typename Array11,
            typename Array12,
            typename Array21,
            typename Array22>
  class ComputeBinaryOperationResult<
      FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array11>::type,
                                  typename ComputeObjectTag<Array12>::type>,
      FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array21>::type,
                                  typename ComputeObjectTag<Array22>::type>,
      FixedSizeArrayDerivative<Array11, Array12>,
      FixedSizeArrayDerivative<Array21, Array22>,
      OpMinus> {
    //! \brief check if the two derivatives are compatible
    static constexpr auto areCompatible =
        std::is_same_v<typename ComputeObjectTag<Array11>::type,
                       typename ComputeObjectTag<Array21>::type> &&
        std::is_same_v<typename ComputeObjectTag<Array12>::type,
                       typename ComputeObjectTag<Array22>::type>;
    /*!
     * \brief numeric type of the resulting array
     * Here we make a tricky choice and it is not totally clean.
     */
    using NumType = result_type<
        result_type<numeric_type<Array11>, numeric_type<Array21>, OpMinus>,
        result_type<numeric_type<Array12>, numeric_type<Array22>, OpPlus>,
        OpDiv>;

   public:
    //! \brief result
    using type = std::conditional_t<
        areCompatible && isInvalid<NumType>(),
        tfel::meta::InvalidType,
        FixedSizeArrayDerivative<result_type<Array11, Array21, OpMinus>,
                                 result_type<Array11, Array21, OpPlus>>>;
  };
  //   /*!
  //    * \brief partial specialisation for the addition of two array
  //    derivatives.
  //    * \see   ComputeBinaryOperationResult
  //    * \tparam Array11: type of the first array for the first array
  //    derivative
  //    * \tparam Array12: type of the second array for the first array
  //    derivative
  //    * \tparam Array21: type of the first array for the second array
  //    derivative
  //    * \tparam Array22: type of the second array for the second array
  //    derivative
  //    */
  //   template <typename Array11,
  //             typename Array12,
  //             typename Array21,
  //             typename Array22>
  //   class ComputeBinaryOperationResult<
  //       FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array11>::type,
  //                                   typename
  //                                   ComputeObjectTag<Array12>::type>,
  //       FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array21>::type,
  //                                   typename
  //                                   ComputeObjectTag<Array22>::type>,
  //       FixedSizeArrayDerivative<Array11, Array12>,
  //       FixedSizeArrayDerivative<Array21, Array22>,
  //       OpMult> {
  //     //! \brief check if the two derivatives are compatible
  //     static constexpr auto areCompatible =
  //         std::is_same_v<typename ComputeObjectTag<Array12>::type,
  //                        typename ComputeObjectTag<Array21>::type>;
  //     /*!
  //      * \brief numeric type of resulting array
  //      * Here we make a tricky choice and it is not totally clean.
  //      */
  //     using NumType =
  //         result_type<numeric_type<Array12>, numeric_type<Array21>, OpMult>;
  //
  //    public:
  //     //! \brief result
  //     using type = std::conditional_t<
  //         areCompatible && isInvalid<NumType>(),
  //         tfel::meta::InvalidType,
  //         FixedSizeArrayDerivative<result_type<Array11, Scalar, OpMinus>,
  //                                  Array21>;
  //   };

  /*!
   * \brief partial specialisation for the multiplication of a scalar by
   * an array derivative.
   * \see   ComputeBinaryOperationResult
   * \tparam ScalarType: type of the scalar by which the derivative array is
   * multiplied.
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   */
  template <typename ScalarType, typename Array1, typename Array2>
  class ComputeBinaryOperationResult<
      ScalarTag,
      FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array1>::type,
                                  typename ComputeObjectTag<Array2>::type>,
      ScalarType,
      FixedSizeArrayDerivative<Array1, Array2>,
      OpMult> {
    //! \brief intermediate numerical type
    using ResultOnArray1 = result_type<ScalarType, Array1, OpMult>;
    //! \brief numeric type of the resulting array
    using NumType =
        result_type<numeric_type<ResultOnArray1>, numeric_type<Array2>, OpDiv>;

   public:
    //! \brief result
    using type =
        std::conditional_t<isInvalid<NumType>(),
                           tfel::meta::InvalidType,
                           FixedSizeArrayDerivative<ResultOnArray1, Array2>>;
  };
  /*!
   * \brief partial specialisation for the multiplication  an array derivative
   * by a scalar.
   * \see   ComputeBinaryOperationResult
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   * \tparam ScalarType: type of the scalar by which the derivative array is
   * multiplied.
   */
  template <typename Array1, typename Array2, typename ScalarType>
  class ComputeBinaryOperationResult<
      FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array1>::type,
                                  typename ComputeObjectTag<Array2>::type>,
      ScalarTag,
      FixedSizeArrayDerivative<Array1, Array2>,
      ScalarType,
      OpMult> {
    //! \brief intermediate numerical type
    using ResultOnArray1 = result_type<Array1, ScalarType, OpMult>;
    //! \brief numeric type of the resulting array
    using NumType =
        result_type<numeric_type<ResultOnArray1>, numeric_type<Array2>, OpDiv>;

   public:
    //! \brief result
    using type =
        std::conditional_t<isInvalid<NumType>(),
                           tfel::meta::InvalidType,
                           FixedSizeArrayDerivative<ResultOnArray1, Array2>>;
  };
  /*!
   * \brief partial specialisation for the multiplication  an array derivative
   * by a scalar.
   * \see   ComputeBinaryOperationResult
   * \tparam Array1: type of the first array
   * \tparam Array2: type of the second array
   * \tparam ScalarType: type of the scalar by which the derivative array is
   * multiplied.
   */
  template <typename Array1, typename Array2, typename ScalarType>
  class ComputeBinaryOperationResult<
      FixedSizeArrayDerivativeTag<typename ComputeObjectTag<Array1>::type,
                                  typename ComputeObjectTag<Array2>::type>,
      ScalarTag,
      FixedSizeArrayDerivative<Array1, Array2>,
      ScalarType,
      OpDiv> {
    //! \brief intermediate numerical type
    using ResultOnArray1 = result_type<Array1, ScalarType, OpDiv>;
    //! \brief numeric type of the resulting array
    using NumType =
        result_type<numeric_type<ResultOnArray1>, numeric_type<Array2>, OpDiv>;

   public:
    //! \brief result
    using type =
        std::conditional_t<isInvalid<NumType>(),
                           tfel::meta::InvalidType,
                           FixedSizeArrayDerivative<ResultOnArray1, Array2>>;
  };

  template <typename Array1,
            typename Array2,
            index_type<FixedSizeArrayDerivative<Array1, Array2>>... I1,
            index_type<FixedSizeArrayDerivative<Array1, Array2>>... I2>
  numeric_type<FixedSizeArrayDerivative<Array1, Array2>>& getValue(
      FixedSizeArrayDerivative<Array1, Array2>& a,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array1::indexing_policy::arity>& i1,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array2::indexing_policy::arity>& i2,
      const std::integer_sequence<
          index_type<FixedSizeArrayDerivative<Array1, Array2>>,
          I1...>&,
      const std::integer_sequence<
          index_type<FixedSizeArrayDerivative<Array1, Array2>>,
          I2...>&) {
    return a(i1[I1]..., i2[I2]...);
  }

  template <typename Array1, typename Array2>
  numeric_type<FixedSizeArrayDerivative<Array1, Array2>>& getValue(
      FixedSizeArrayDerivative<Array1, Array2>& a,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array1::indexing_policy::arity>& i1,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array2::indexing_policy::arity>& i2) {
    using SizeType = index_type<FixedSizeArrayDerivative<Array1, Array2>>;
    constexpr auto iseq1 =
        std::make_integer_sequence<SizeType, Array1::indexing_policy::arity>();
    constexpr auto iseq2 =
        std::make_integer_sequence<SizeType, Array2::indexing_policy::arity>();
    return getValue(a, i1, i2, iseq1, iseq2);
  }

  template <typename Array1,
            typename Array2,
            index_type<FixedSizeArrayDerivative<Array1, Array2>>... I1,
            index_type<FixedSizeArrayDerivative<Array1, Array2>>... I2>
  const numeric_type<FixedSizeArrayDerivative<Array1, Array2>>& getValue(
      const FixedSizeArrayDerivative<Array1, Array2>& a,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array1::indexing_policy::arity>& i1,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array2::indexing_policy::arity>& i2,
      const std::integer_sequence<
          index_type<FixedSizeArrayDerivative<Array1, Array2>>,
          I1...>&,
      const std::integer_sequence<
          index_type<FixedSizeArrayDerivative<Array1, Array2>>,
          I2...>&) {
    return a(i1[I1]..., i2[I2]...);
  }

  template <typename Array1, typename Array2>
  const numeric_type<FixedSizeArrayDerivative<Array1, Array2>>& getValue(
      const FixedSizeArrayDerivative<Array1, Array2>& a,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array1::indexing_policy::arity>& i1,
      const std::array<index_type<FixedSizeArrayDerivative<Array1, Array2>>,
                       Array2::indexing_policy::arity>& i2) {
    using SizeType = index_type<FixedSizeArrayDerivative<Array1, Array2>>;
    constexpr auto iseq1 =
        std::make_integer_sequence<SizeType, Array1::indexing_policy::arity>();
    constexpr auto iseq2 =
        std::make_integer_sequence<SizeType, Array2::indexing_policy::arity>();
    return getValue(a, i1, i2, iseq1, iseq2);
  }

  template <typename SizeType, SizeType I>
  struct LoopOverInnerIndices {
    template <typename Array11,
              typename Array12,
              typename Array21,
              typename Array22>
    static void exe(
        numeric_type<FixedSizeArrayDerivative<Array11, Array22>>& cv,
        std::array<SizeType, Array12::indexing_policy::arity>& i12,
        const FixedSizeArrayDerivative<Array11, Array12>& a,
        const FixedSizeArrayDerivative<Array21, Array22>& b,
        const std::array<SizeType, Array11::indexing_policy::arity>& i11,
        const std::array<SizeType, Array22::indexing_policy::arity>& i22) {
      constexpr typename Array12::indexing_policy p;
      constexpr auto a12 = Array12::indexing_policy::arity;
      for (SizeType i = 0; i != p.size(I); ++i) {
        i12[I] = i;
        if constexpr (I + 1 == a12) {
          const auto av = getValue(a, i11, i12);
          const auto bv = getValue(b, i12, i22);
          cv += av * bv;
        } else {
          LoopOverInnerIndices<SizeType, I + 1>::exe(cv, i12, a, b, i11, i22);
        }
      }
    }
  };

  template <typename SizeType, SizeType I>
  struct LoopOverSecondIndices {
    template <typename Array11,
              typename Array12,
              typename Array21,
              typename Array22>
    static void exe(
        FixedSizeArrayDerivative<Array11, Array22>& c,
        std::array<SizeType, Array22::indexing_policy::arity>& i22,
        const FixedSizeArrayDerivative<Array11, Array12>& a,
        const FixedSizeArrayDerivative<Array21, Array22>& b,
        const std::array<SizeType, Array11::indexing_policy::arity>& i11) {
      constexpr typename Array22::indexing_policy p;
      constexpr auto a22 = Array22::indexing_policy::arity;
      for (SizeType i = 0; i != p.size(I); ++i) {
        i22[I] = i;
        if constexpr (I + 1 == a22) {
          std::array<SizeType, Array12::indexing_policy::arity> i12;
          auto& cv = getValue(c, i11, i22);
          cv = 0;
          LoopOverInnerIndices<SizeType, 0>::exe(cv, i12, a, b, i11, i22);
        } else {
          LoopOverSecondIndices<SizeType, I + 1>::exe(c, i22, a, b, i11);
        }
      }
    }
  };

  template <typename SizeType, SizeType I>
  struct LoopOverFirstIndices {
    template <typename Array11,
              typename Array12,
              typename Array21,
              typename Array22>
    static void exe(FixedSizeArrayDerivative<Array11, Array22>& c,
                    std::array<SizeType, Array11::indexing_policy::arity>& i11,
                    const FixedSizeArrayDerivative<Array11, Array12>& a,
                    const FixedSizeArrayDerivative<Array21, Array22>& b) {
      constexpr typename Array11::indexing_policy p;
      constexpr auto a11 = Array11::indexing_policy::arity;
      for (SizeType i = 0; i != p.size(I); ++i) {
        i11[I] = i;
        if constexpr (I + 1 == a11) {
          std::array<SizeType, Array22::indexing_policy::arity> i22;
          LoopOverSecondIndices<SizeType, 0>::exe(c, i22, a, b, i11);
        } else {
          LoopOverFirstIndices<SizeType, I + 1>::exe(c, i11, a, b);
        }
      }
    }
    template <typename Array11,
              typename Array12,
              typename Array21,
              typename Array22>
    static void exe(FixedSizeArrayDerivative<Array11, Array22>& c,
                    const FixedSizeArrayDerivative<Array11, Array12>& a,
                    const FixedSizeArrayDerivative<Array21, Array22>& b) {
      std::array<SizeType, Array11::indexing_policy::arity> i11;
      LoopOverFirstIndices<SizeType, I>::exe(c, i11, a, b);
    }
  };

  struct MultiplyFixedSizeArrayDerivatives {
    template <typename Array11,
              typename Array12,
              typename Array21,
              typename Array22>
    static void exe(FixedSizeArrayDerivative<Array11, Array22>& c,
                    const FixedSizeArrayDerivative<Array11, Array12>& a,
                    const FixedSizeArrayDerivative<Array21, Array22>& b) {
      using size_type = index_type<FixedSizeArrayDerivative<Array11, Array22>>;
      if constexpr ((Array11::indexing_policy::arity == 1) &&
                    (Array12::indexing_policy::arity == 1) &&
                    (Array21::indexing_policy::arity == 1) &&
                    (Array22::indexing_policy::arity == 1)) {
        using numtype =
            numeric_type<FixedSizeArrayDerivative<Array11, Array22>>;
        for (size_type i = 0; i != c.size(0); ++i) {
          for (size_type j = 0; j != c.size(1); ++j) {
            auto& v = c(i, j);
            v = numtype{};
            for (size_type k = 0; k != a.size(1); ++k) {
              v += a(i, k) * b(k, j);
            }
          }
        }
      } else {
        LoopOverFirstIndices<size_type, 0>::exe(c, a, b);
      }
    }  // end of exe
  };

  template <typename Array11,
            typename Array12,
            typename Array21,
            typename Array22>
  FixedSizeArrayDerivative<Array11, Array22> operator*(
      const FixedSizeArrayDerivative<Array11, Array12>& a,
      const FixedSizeArrayDerivative<Array21, Array22>& b) {
    FixedSizeArrayDerivative<Array11, Array22> c;
    MultiplyFixedSizeArrayDerivatives::exe(c, a, b);
    return c;
  }  // end of operator*

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
