/*!
 * \file   include/TFEL/Math/t2tot2.hxx
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

#ifndef LIB_TFEL_MATH_T2TOT2_HXX
#define LIB_TFEL_MATH_T2TOT2_HXX

#include <cstddef>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Forward/t2tot2.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include "TFEL/Math/T2toT2/T2toT2ConceptOperations.hxx"

namespace tfel::math {

  // forward declaration
  template <unsigned short N>
  struct TensorProductLeftDerivativeExpr;

  // forward declaration
  template <unsigned short N>
  struct TensorProductRightDerivativeExpr;

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <T2toT2Concept T2toT2Type, typename ScalarType>
  struct DerivativeTypeDispatcher<T2toT2Tag,
                                  ScalarTag,
                                  T2toT2Type,
                                  ScalarType> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<T2toT2Type>>(),
                  "the t2tot2 type does not hold a scalar");
    //! \brief result
    using type = t2tot2<getSpaceDimension<T2toT2Type>(),
                        derivative_type<numeric_type<T2toT2Type>, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType, T2toT2Concept T2toT2Type>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  T2toT2Tag,
                                  ScalarType,
                                  T2toT2Type> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<T2toT2Type>>(),
                  "the t2tot2 type does not hold a scalar");
    //! \brief result
    using type = t2tot2<getSpaceDimension<T2toT2Type>(),
                        derivative_type<ScalarType, numeric_type<T2toT2Type>>>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <TensorConcept TensorType1, TensorConcept TensorType2>
  struct DerivativeTypeDispatcher<TensorTag,
                                  TensorTag,
                                  TensorType1,
                                  TensorType2> {
    static_assert(getSpaceDimension<TensorType1>() ==
                      getSpaceDimension<TensorType2>(),
                  "symmetric tensor types don't have the same dimension");
    static_assert(isScalar<numeric_type<TensorType1>>(),
                  "the first tensor type does not hold a scalar");
    static_assert(isScalar<numeric_type<TensorType2>>(),
                  "the second tensor type does not hold a scalar");
    //! \brief result
    using type = t2tot2<
        getSpaceDimension<TensorType1>(),
        derivative_type<numeric_type<TensorType1>, numeric_type<TensorType2>>>;
  };  // end of struct DerivativeTypeDispatcher

  template <unsigned short N, typename ValueType>
  struct t2tot2 : T2toT2ConceptBase<t2tot2<N, ValueType>>,
                  GenericFixedSizeArray<
                      t2tot2<N, ValueType>,
                      FixedSizeRowMajorMatrixPolicy<TensorDimeToSize<N>::value,
                                                    TensorDimeToSize<N>::value,
                                                    ValueType>> {
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    //! \brief a simple alias
    using GenericFixedSizeArrayBase = GenericFixedSizeArray<
        t2tot2<N, ValueType>,
        FixedSizeRowMajorMatrixPolicy<TensorDimeToSize<N>::value,
                                      TensorDimeToSize<N>::value,
                                      ValueType>>;
    /*!
     * \return a t2tot2 which acts on a tensor as a rotation of the coordinate
     * system
     * \param[in] r : rotation matrix
     */
    TFEL_HOST_DEVICE static constexpr auto fromRotationMatrix(
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * \param[in] B : second tensor of the product
     * \return the left part of the derivative of a tensor product
     */
    template <TensorConcept TensorType>
    TFEL_HOST_DEVICE static constexpr auto tpld(const TensorType&) noexcept
        requires(getSpaceDimension<TensorType>() == N &&
                 isAssignableTo<numeric_type<TensorType>, ValueType>());
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the left part of the derivative of a tensor product
     */
    template <TensorConcept TensorType, T2toT2Concept T2toT2Type>
    TFEL_HOST_DEVICE static constexpr auto
    tpld(const TensorType&, const T2toT2Type&) noexcept requires(
        getSpaceDimension<TensorType>() == N &&
        getSpaceDimension<T2toT2Type>() == N &&
        isAssignableTo<typename ComputeBinaryResult<numeric_type<TensorType>,
                                                    numeric_type<T2toT2Type>,
                                                    OpMult>::Result,
                       ValueType>());
    /*!
     * \param[in] A : first tensor of the product
     * \return the right part of the derivative of a tensor product
     */
    template <TensorConcept TensorType>
    TFEL_HOST_DEVICE static constexpr auto tprd(const TensorType&) noexcept
        requires(getSpaceDimension<TensorType>() == N &&
                 isAssignableTo<numeric_type<TensorType>, ValueType>());
    /*!
     * \param[in] A : first tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the right part of the derivative of a tensor product
     */
    template <TensorConcept TensorType, T2toT2Concept T2toT2Type>
    TFEL_HOST_DEVICE static constexpr auto
    tprd(const TensorType&, const T2toT2Type&) noexcept requires(
        getSpaceDimension<TensorType>() == N &&
        getSpaceDimension<T2toT2Type>() == N &&
        isAssignableTo<typename ComputeBinaryResult<numeric_type<TensorType>,
                                                    numeric_type<T2toT2Type>,
                                                    OpMult>::Result,
                       ValueType>());
    /*!
     * \return the derivative of the transpose of a tensor with respect of
     * this tensor
     */
    TFEL_HOST_DEVICE static constexpr auto transpose_derivative() noexcept;
    //! \return the identity
    TFEL_HOST_DEVICE static constexpr auto Id() noexcept;
    //! \return Id2^Id2, where Id2 is the identity tensor
    TFEL_HOST_DEVICE static constexpr auto IxI() noexcept;
    /*!
     * \return Id4-Id2^Id2/3, where Id4 is the identity of t2tot2 and
     * Id2 is the identity tensor
     */
    TFEL_HOST_DEVICE static constexpr auto K() noexcept;
    /*!
     * \brief constructor from a t2tost2
     * \param[in] v : values
     */
    template <T2toST2Concept T2toST2Type>
    TFEL_HOST_DEVICE constexpr t2tot2(const T2toST2Type&) noexcept
        requires((isAssignableTo<numeric_type<T2toST2Type>, ValueType>()) &&
                 (getSpaceDimension<T2toST2Type>() == N));
    /*!
     * \brief constructors from arrays to implement class
     * template argument deduction.
     * \tparam d: dimensions of the arrays
     */
    template <std::size_t... d>
    TFEL_HOST_DEVICE constexpr t2tot2(ValueType const (&... arrays)[d])  //
        requires((sizeof...(d) == TensorDimeToSize<N>::value) &&
                 ((d == TensorDimeToSize<N>::value) && ...));
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(t2tot2,
                                               GenericFixedSizeArrayBase);
    //! exposing GenericFixedSizeArray access operator
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! \brief import values from an external memory location
    TFEL_HOST_DEVICE constexpr void import(
        const base_type<ValueType>* const) noexcept;
    //! \brief copy from a range
    TFEL_HOST_DEVICE constexpr void copy(const auto) noexcept;
  };

  /*!
   * \brief class template argument deduction
   * \tparam ValueType: numeric type
   * \tparam M: number of columns
   * \tparam d: sizes of the arrays, that must be all equal to M
   */
  template <typename ValueType, std::size_t M, std::size_t... d>
  t2tot2(ValueType const (&)[M], ValueType const (&... arrays)[d])
      -> t2tot2<TensorSizeToDime<sizeof...(d) + 1>::value, ValueType>;

  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using T2toT2View = View<t2tot2<N, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ConstT2toT2View = ConstView<t2tot2<N, T>>;

  /*!
   * \return change the basis of a t2tot2
   * \param[in] s : t2tot2
   * \param[in] r : rotation matrix
   */
  template <T2toT2Concept T2toT2Type>
  TFEL_HOST_DEVICE constexpr auto change_basis(
      const T2toT2Type&,
      const rotation_matrix<numeric_type<T2toT2Type>>&) noexcept;
  /*!
   * \return compute the derivative of the velocity gradient
   * \param[in] F: deformation gradient
   */
  TFEL_HOST_DEVICE constexpr auto computeVelocityGradientDerivative(
      const TensorConcept auto&) noexcept;
  /*!
   * \return compute the derivative of the spin rate
   * \param[in] F: deformation gradient
   */
  TFEL_HOST_DEVICE constexpr auto computeSpinRateDerivative(
      const TensorConcept auto&) noexcept;
  /*!
   * \brief compute the second derivative of the determinant of a
   * symmetric tensor
   * \param[in] s: tensor
   */
  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto computeDeterminantSecondDerivative(
      const TensorType&) noexcept
      requires(isScalar<numeric_type<TensorType>>());

  template <typename T, T2toST2Concept T2toST2Type>
  TFEL_HOST_DEVICE constexpr void convert(
      t2tot2<getSpaceDimension<T2toST2Type>(), T>&,
      const T2toST2Type&) noexcept  //
      requires(isAssignableTo<numeric_type<T2toST2Type>, T>());

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::t2tot2<N, T2>, tfel::math::t2tot2<N, T>> {
    /*!
     *  Result
     */
    static constexpr bool cond = IsAssignableTo<T2, T>::cond;
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/T2toT2/t2tot2.ixx"
#include "TFEL/Math/T2toT2/t2tot2ResultType.hxx"

#endif /* LIB_TFEL_MATH_T2TOT2_HXX */
