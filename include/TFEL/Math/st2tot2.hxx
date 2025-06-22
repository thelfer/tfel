/*!
 * \file   include/TFEL/Math/st2tot2.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2_HXX
#define LIB_TFEL_MATH_ST2TOT2_HXX

#include <cstddef>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Forward/st2tot2.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2Concept.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2ConceptOperations.hxx"
#include "TFEL/Math/ST2toT2/StensorProductLeftDerivativeExpr.hxx"
#include "TFEL/Math/ST2toT2/StensorProductRightDerivativeExpr.hxx"

namespace tfel::math {

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ST2toT2Type, typename ScalarType>
  struct DerivativeTypeDispatcher<ST2toT2Tag,
                                  ScalarTag,
                                  ST2toT2Type,
                                  ScalarType> {
    static_assert(implementsST2toT2Concept<ST2toT2Type>(),
                  "template argument ST2toT2Type is not a st2tot2");
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<ST2toT2Type>>(),
                  "the st2tot2 type does not hold a scalar");
    //! \brief result
    using type =
        st2tot2<getSpaceDimension<ST2toT2Type>(),
                derivative_type<numeric_type<ST2toT2Type>, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType, typename ST2toT2Type>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  ST2toT2Tag,
                                  ScalarType,
                                  ST2toT2Type> {
    static_assert(implementsST2toT2Concept<ST2toT2Type>(),
                  "template argument ST2toT2Type is not a st2tot2");
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<ST2toT2Type>>(),
                  "the st2tot2 type does not hold a scalar");
    //! \brief result
    using type =
        st2tot2<getSpaceDimension<ST2toT2Type>(),
                derivative_type<ScalarType, numeric_type<ST2toT2Type>>>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename TensorType1, typename StensorType2>
  struct DerivativeTypeDispatcher<TensorTag,
                                  StensorTag,
                                  TensorType1,
                                  StensorType2> {
    static_assert(implementsTensorConcept<TensorType1>(),
                  "template argument TensorType1 is not a tensor");
    static_assert(implementsStensorConcept<StensorType2>(),
                  "template argument StensorType2 is not a symmetric tensor");
    static_assert(getSpaceDimension<TensorType1>() ==
                      getSpaceDimension<StensorType2>(),
                  "symmetric tensor types don't have the same dimension");
    static_assert(isScalar<numeric_type<TensorType1>>(),
                  "the first tensor type does not hold a scalar");
    static_assert(isScalar<numeric_type<StensorType2>>(),
                  "the second symmetric tensor type does not hold a scalar");
    //! \brief result
    using type = st2tot2<
        getSpaceDimension<TensorType1>(),
        derivative_type<numeric_type<TensorType1>, numeric_type<StensorType2>>>;
  };  // end of struct DerivativeTypeDispatcher

  template <unsigned short N, typename ValueType>
  struct st2tot2
      : ST2toT2Concept<st2tot2<N, ValueType>>,
        GenericFixedSizeArray<
            st2tot2<N, ValueType>,
            FixedSizeRowMajorMatrixPolicy<TensorDimeToSize<N>::value,
                                          StensorDimeToSize<N>::value,
                                          ValueType>> {
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    //! \brief a simple alias
    using GenericFixedSizeArrayBase = GenericFixedSizeArray<
        st2tot2<N, ValueType>,
        FixedSizeRowMajorMatrixPolicy<TensorDimeToSize<N>::value,
                                      StensorDimeToSize<N>::value,
                                      ValueType>>;
    /*!
     * \param[in] B : second tensor of the product
     * \return the left part of the derivative of a tensor product
     */
    template <typename StensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            getSpaceDimension<StensorType>() == N &&
            isAssignableTo<numeric_type<StensorType>, ValueType>(),
        Expr<st2tot2<N, ValueType>, StensorProductLeftDerivativeExpr<N>>>
    tpld(const StensorType&);
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the left part of the derivative of a tensor product
     */
    template <typename StensorType, typename ST2toST2Type>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            implementsST2toST2Concept<ST2toST2Type>() &&
            getSpaceDimension<StensorType>() == N &&
            getSpaceDimension<ST2toST2Type>() == N &&
            isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                                 numeric_type<ST2toST2Type>,
                                                 OpMult>,
                           ValueType>(),
        Expr<st2tot2<N, ValueType>, StensorProductLeftDerivativeExpr<N>>>
    tpld(const StensorType&, const ST2toST2Type&);
    /*!
     * \param[in] A : first tensor of the product
     * \return the right part of the derivative of a tensor product
     */
    template <typename StensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            getSpaceDimension<StensorType>() == N &&
            isAssignableTo<numeric_type<StensorType>, ValueType>(),
        Expr<st2tot2<N, ValueType>, StensorProductRightDerivativeExpr<N>>>
    tprd(const StensorType&);
    /*!
     * \param[in] A : first tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the right part of the derivative of a tensor product
     */
    template <typename StensorType, typename ST2toST2Type>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            implementsST2toST2Concept<ST2toST2Type>() &&
            getSpaceDimension<StensorType>() == N &&
            getSpaceDimension<ST2toST2Type>() == N &&
            isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                                 numeric_type<ST2toST2Type>,
                                                 OpMult>,
                           ValueType>(),
        Expr<st2tot2<N, ValueType>, StensorProductRightDerivativeExpr<N>>>
    tprd(const StensorType&, const ST2toST2Type&);
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(st2tot2,
                                               GenericFixedSizeArrayBase);
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! \brief import values from an external memory location
    void import(const base_type<ValueType>* const);
    //
    template <typename InputIterator>
    TFEL_MATH_INLINE2 void copy(const InputIterator src);
  };

  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ST2toT2View = View<st2tot2<N, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ConstST2toT2View = ConstView<st2tot2<N, T>>;

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::st2tot2<N, T2>, tfel::math::st2tot2<N, T>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/ST2toT2/st2tot2.ixx"
#include "TFEL/Math/ST2toT2/st2tot2ResultType.hxx"

#endif /* LIB_TFEL_MATH_ST2TOT2_HXX */
