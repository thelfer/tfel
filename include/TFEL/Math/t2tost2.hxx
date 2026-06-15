/*!
 * \file   include/TFEL/Math/t2tost2.hxx
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

#ifndef LIB_TFEL_MATH_T2TOST2_HXX
#define LIB_TFEL_MATH_T2TOST2_HXX

#include <cstddef>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Forward/t2tost2.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/T2toST2/T2toST2ConceptOperations.hxx"

namespace tfel::math {

  //! forward declaration
  template <unsigned short>
  struct RightCauchyGreenTensorDerivativeExpr;

  //! forward declaration
  template <unsigned short>
  struct LeftCauchyGreenTensorDerivativeExpr;

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <T2toST2Concept T2toST2Type, typename ScalarType>
  struct DerivativeTypeDispatcher<T2toST2Tag,
                                  ScalarTag,
                                  T2toST2Type,
                                  ScalarType> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<T2toST2Type>>(),
                  "the t2tost2 type does not hold a scalar");
    //! \brief result
    using type =
        t2tost2<getSpaceDimension<T2toST2Type>(),
                derivative_type<numeric_type<T2toST2Type>, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType, T2toST2Concept T2toST2Type>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  T2toST2Tag,
                                  ScalarType,
                                  T2toST2Type> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<T2toST2Type>>(),
                  "the t2tost2 type does not hold a scalar");
    //! \brief result
    using type =
        t2tost2<getSpaceDimension<T2toST2Type>(),
                derivative_type<ScalarType, numeric_type<T2toST2Type>>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <StensorConcept StensorType1, TensorConcept TensorType2>
  struct DerivativeTypeDispatcher<StensorTag,
                                  TensorTag,
                                  StensorType1,
                                  TensorType2> {
    static_assert(getSpaceDimension<StensorType1>() ==
                      getSpaceDimension<TensorType2>(),
                  "symmetric tensor types don't have the same dimension");
    static_assert(isScalar<numeric_type<StensorType1>>(),
                  "the first tensor type does not hold a scalar");
    static_assert(isScalar<numeric_type<TensorType2>>(),
                  "the second tensor type does not hold a scalar");
    //! \brief result
    using type = t2tost2<
        getSpaceDimension<StensorType1>(),
        derivative_type<numeric_type<StensorType1>, numeric_type<TensorType2>>>;
  };  // end of struct DerivativeTypeDispatcher

  //
  template <unsigned short N, typename ValueType>
  struct t2tost2
      : T2toST2ConceptBase<t2tost2<N, ValueType>>,
        GenericFixedSizeArray<
            t2tost2<N, ValueType>,
            FixedSizeRowMajorMatrixPolicy<StensorDimeToSize<N>::value,
                                          TensorDimeToSize<N>::value,
                                          ValueType>> {
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    //! \brief a simple alias
    using GenericFixedSizeArrayBase = GenericFixedSizeArray<
        t2tost2<N, ValueType>,
        FixedSizeRowMajorMatrixPolicy<StensorDimeToSize<N>::value,
                                      TensorDimeToSize<N>::value,
                                      ValueType>>;
    /*!
     * \param[in] F : deformation gradient
     * \return the derivative of the Cauchy right symmetric tensor
     * with respect to the deformation gradient
     */
    template <TensorConcept TensorType>
    TFEL_HOST_DEVICE static TFEL_HOST_DEVICE constexpr auto dCdF(
        const TensorType&) noexcept
        requires(getSpaceDimension<TensorType>() == N &&
                 isAssignableTo<numeric_type<TensorType>, ValueType>());
    /*!
     * \param[in] F : deformation gradient
     * \return the derivative of the Cauchy left symmetric tensor
     * with respect to the deformation gradient
     */
    template <TensorConcept TensorType>
    TFEL_HOST_DEVICE static TFEL_HOST_DEVICE constexpr auto dBdF(
        const TensorType&) noexcept
        requires(getSpaceDimension<TensorType>() == N &&
                 isAssignableTo<numeric_type<TensorType>, ValueType>());
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(t2tost2,
                                               GenericFixedSizeArrayBase);
    /*!
     * \brief constructors from arrays to implement class
     * template argument deduction.
     * \tparam d: dimensions of the arrays
     */
    template <std::size_t... d>
    TFEL_HOST_DEVICE constexpr t2tost2(ValueType const (&... arrays)[d])  //
        requires((sizeof...(d) == StensorDimeToSize<N>::value) &&
                 ((d == TensorDimeToSize<N>::value) && ...));
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! \brief import values
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
  t2tost2(ValueType const (&)[M], ValueType const (&... arrays)[d])
      -> t2tost2<StensorSizeToDime<sizeof...(d) + 1>::value, ValueType>;

  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using T2toST2View = View<t2tost2<N, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ConstT2toST2View = ConstView<t2tost2<N, T>>;

  /*!
   * \return change the basis of a t2tost2
   * \param[in] s: t2tost2
   * \param[in] r: rotation matrix
   */
  template <T2toST2Concept T2toST2Type>
  TFEL_HOST_DEVICE constexpr auto change_basis(
      const T2toST2Type&,
      const rotation_matrix<numeric_type<T2toST2Type>>&) noexcept;
  /*!
   * \brief convert a t2tot2 tensor to a t2tost2
   * \param[in] t: fourth order tensor to be converted
   */
  TFEL_HOST_DEVICE constexpr auto convertToT2toST2(
      const T2toT2Concept auto&) noexcept;
  /*!
   * \return compute the "derivative" of the rate of deformation
   * \delta D = dD_dF:\delta F with \delta D = ((delta F).F^{-1}+F^{-T}.(delta
   * F)^{T})/2
   * \param[in] F : deformation gradient
   */
  TFEL_HOST_DEVICE constexpr auto computeRateOfDeformationDerivative(
      const TensorConcept auto&) noexcept;
  /*!
   * \brief compute the Cauchy stress derivative from the Kirchhoff stress
   * derivative
   * \param[in]  dt: Kirchhoff stress derivative
   * \param[in]  s:  Cauchy stress
   * \param[in]  F:  deformation gradient
   */
  template <T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto
  computeCauchyStressDerivativeFromKirchhoffStressDerivative(
      const T2toST2Type&,
      const StensorType&,
      const TensorType&) noexcept  //
      requires(getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<StensorType>() &&
               getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<TensorType>() &&
               tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<TensorType>>::cond);
  /*!
   * \brief compute the Cauchy stress derivative from the Kirchhoff stress
   * derivative
   * \param[in]  dt: Kirchhoff stress derivative
   * \param[in]  s:  Cauchy stress
   * \param[in]  F:  deformation gradient
   */
  template <T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto
  computeKirchhoffStressDerivativeFromCauchyStressDerivative(
      const T2toST2Type&,
      const StensorType&,
      const TensorType&) noexcept  //
      requires(getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<StensorType>() &&
               getSpaceDimension<T2toST2Type>() ==
                   getSpaceDimension<TensorType>() &&
               tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<TensorType>>::cond);
  /*!
   * \return the derivative of the push-forward of a symmetric
   * second order tensor with respect to the deformation gradient
   * knowing the value of this derivative in the initial
   * configuration.
   *
   * This function is typically used to compute the derivative of
   * the Kirchhoff stress tensor knowing the derivative of the second
   * Piola-Kirschoff stress.
   *
   * \param[in]  : derivative of the orginal tensor
   * \param[in]  : orginal tensor (second Piola-Kirschoff stress)
   * \param[in]  : deformation gradient
   */
  template <T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto computePushForwardDerivative(
      const T2toST2Type&,
      const StensorType&,
      const TensorType&) noexcept  //
      requires(getSpaceDimension<StensorType>() ==
                   getSpaceDimension<T2toST2Type>() &&
               getSpaceDimension<TensorType>() ==
                   getSpaceDimension<T2toST2Type>() &&
               tfel::typetraits::IsFundamentalNumericType<
                   numeric_type<TensorType>>::cond);
  /*!
   * \brief compute de derivative of the push-forward of a symmetric
   * second order tensor with respect to the deformation gradient
   * knowing the value of this derivative in the initial
   * configuration.
   *
   * This function is typically used to compute the derivative of
   * the Kirchhoff stress tensor knowing the derivative of the second
   * Piola-Kirschoff stress.
   *
   * \param[out] : derivative of the push-forward symmetric tensor
   * \param[in]  : derivative of the orginal tensor
   * \param[in]  : orginal tensor (second Piola-Kirschoff stress)
   * \param[in]  : deformation gradient
   */
  template <T2toST2Concept T2toST2ResultType,
            T2toST2Concept T2toST2Type,
            StensorConcept StensorType,
            TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr void computePushForwardDerivative(
      T2toST2ResultType&,
      const T2toST2Type&,
      const StensorType&,
      const TensorType&) noexcept  //
      requires(tfel::typetraits::IsFundamentalNumericType<
               numeric_type<TensorType>>::cond&&
                   isAssignableTo<
                       typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                    numeric_type<StensorType>,
                                                    OpPlus>::Result,
                       numeric_type<T2toST2ResultType>>());

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::t2tost2<N, T2>, tfel::math::t2tost2<N, T>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/T2toST2/t2tost2.ixx"
#include "TFEL/Math/T2toST2/t2tost2ResultType.hxx"

#endif /* LIB_TFEL_MATH_T2TOST2_HXX */
