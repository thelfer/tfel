/*!
 * \file   include/TFEL/Math/st2tost2.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 May 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOST2_HXX
#define LIB_TFEL_MATH_ST2TOST2_HXX

#include <cstddef>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx"
#include "TFEL/Math/ST2toST2/StensorSquareDerivative.hxx"

namespace tfel::math {

  // forward declaration
  template <unsigned short N>
  struct StensorProductLeftDerivativeExpr;

  // forward declaration
  template <unsigned short N>
  struct StensorProductRightDerivativeExpr;

  // forward declaration
  template <unsigned short N>
  struct ConvertT2toST2ToST2toST2Expr;

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <ST2toST2Concept ST2toST2Type, typename ScalarType>
  struct DerivativeTypeDispatcher<ST2toST2Tag,
                                  ScalarTag,
                                  ST2toST2Type,
                                  ScalarType> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<ST2toST2Type>>(),
                  "the st2tost2 type does not hold a scalar");
    //! \brief result
    using type =
        st2tost2<getSpaceDimension<ST2toST2Type>(),
                 derivative_type<numeric_type<ST2toST2Type>, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType, ST2toST2Concept ST2toST2Type>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  ST2toST2Tag,
                                  ScalarType,
                                  ST2toST2Type> {
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<ST2toST2Type>>(),
                  "the st2tost2 type does not hold a scalar");
    //! \brief result
    using type =
        st2tost2<getSpaceDimension<ST2toST2Type>(),
                 derivative_type<ScalarType, numeric_type<ST2toST2Type>>>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <StensorConcept StensorType1, StensorConcept StensorType2>
  struct DerivativeTypeDispatcher<StensorTag,
                                  StensorTag,
                                  StensorType1,
                                  StensorType2> {
    static_assert(getSpaceDimension<StensorType1>() ==
                      getSpaceDimension<StensorType2>(),
                  "symmetric tensor types don't have the same dimension");
    static_assert(isScalar<numeric_type<StensorType1>>(),
                  "the first symmetric tensor type does not hold a scalar");
    static_assert(isScalar<numeric_type<StensorType2>>(),
                  "the second symmetric tensor type does not hold a scalar");
    //! \brief result
    using type = st2tost2<getSpaceDimension<StensorType1>(),
                          derivative_type<numeric_type<StensorType1>,
                                          numeric_type<StensorType2>>>;
  };  // end of struct DerivativeTypeDispatcher

  template <unsigned short N, typename ValueType>
  struct st2tost2
      : ST2toST2ConceptBase<st2tost2<N, ValueType>>,
        GenericFixedSizeArray<
            st2tost2<N, ValueType>,
            FixedSizeRowMajorMatrixPolicy<StensorDimeToSize<N>::value,
                                          StensorDimeToSize<N>::value,
                                          ValueType>> {
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    //! \brief a simple alias
    using GenericFixedSizeArrayBase = GenericFixedSizeArray<
        st2tost2<N, ValueType>,
        FixedSizeRowMajorMatrixPolicy<StensorDimeToSize<N>::value,
                                      StensorDimeToSize<N>::value,
                                      ValueType>>;
    /*!
     * \param[in] s : tensor squared
     * \return the derivative of the square of a symmetric tensor
     */
    template <StensorConcept StensorType>
    TFEL_HOST_DEVICE static constexpr auto dsquare(const StensorType&) noexcept
        requires(getSpaceDimension<StensorType>() == N &&
                 isAssignableTo<numeric_type<StensorType>, ValueType>());
    /*!
     * \param[in] s : tensor squared
     * \param[in] C : derivative of s
     * \return the derivative of the square of a symmetric tensor
     */
    template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE static constexpr auto dsquare(const StensorType&,
                                                   const ST2toST2Type&) noexcept
        requires(
            getSpaceDimension<StensorType>() == N &&
            getSpaceDimension<ST2toST2Type>() == N &&
            isAssignableTo<BinaryOperationResult<numeric_type<StensorType>,
                                                 numeric_type<ST2toST2Type>,
                                                 OpMult>,
                           ValueType>());
    /*!
     * convert a T2toST2 to a ST2toST2
     * \param[in] src : T2toST2 to be converted
     */
    template <T2toST2Concept T2toST2Type>
    TFEL_HOST_DEVICE static constexpr auto convert(const T2toST2Type&) noexcept
        requires(getSpaceDimension<T2toST2Type>() == N &&
                 isAssignableTo<numeric_type<T2toST2Type>, ValueType>());
    /*!
     * build the equivalent st2tost2 from a rotation matrix
     * \param[in] r : rotation matrix
     */
    TFEL_HOST_DEVICE static constexpr auto fromRotationMatrix(
        const rotation_matrix<ValueType>&) noexcept;
    /*!
     * \brief compute the derivative of the symmetric tensor product:
     * \f[
     * \frac{d}{ds1}\left(s1*s+s*s1\right)
     * \f]
     * \param[in] s: second tensor of the product
     */
    template <StensorConcept StensorType>
    TFEL_HOST_DEVICE static constexpr auto stpd(const StensorType&) noexcept
        requires(getSpaceDimension<StensorType>() == N &&
                 isAssignableTo<numeric_type<StensorType>, ValueType>());
    //
    static constexpr st2tost2 Id() noexcept;
    static constexpr st2tost2 IxI() noexcept;
    static constexpr st2tost2 K() noexcept;
    static constexpr st2tost2 M() noexcept;
    static constexpr st2tost2 J() noexcept;
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(st2tost2,
                                               GenericFixedSizeArrayBase);
    /*!
     * \brief constructors from arrays to implement class
     * template argument deduction.
     * \tparam d: dimensions of the arrays
     */
    template <std::size_t... d>
    TFEL_HOST_DEVICE constexpr st2tost2(ValueType const (&... arrays)[d])  //
        requires((sizeof...(d) == StensorDimeToSize<N>::value) &&
                 ((d == StensorDimeToSize<N>::value) && ...));
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! \brief import values from an external memory location
    TFEL_HOST_DEVICE constexpr void import(
        const base_type<ValueType>* const) noexcept;
    /*!
     * \brief copy the values from a range
     * \param[in] p: iterator
     */
    TFEL_HOST_DEVICE constexpr void copy(const auto) noexcept;
  };

  /*!
   * \brief class template argument deduction
   * \tparam ValueType: numeric type
   * \tparam M: number of columns
   * \tparam d: sizes of the arrays, that must be all equal to M
   */
  template <typename ValueType, std::size_t M, std::size_t... d>
  st2tost2(ValueType const (&)[M], ValueType const (&... arrays)[d])
      -> st2tost2<StensorSizeToDime<sizeof...(d) + 1>::value, ValueType>;

  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ST2toST2View = View<st2tost2<N, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ConstST2toST2View = ConstView<st2tost2<N, T>>;

  /*!
   * \return change the basis of a st2tost2
   * \param[in] s : st2tost2
   * \param[in] r : rotation matrix
   */
  template <ST2toST2Concept ST2toST2Type>
  TFEL_HOST_DEVICE constexpr auto change_basis(
      const ST2toST2Type&,
      const rotation_matrix<numeric_type<ST2toST2Type>>&) noexcept;
  /*!
   * \return the invert of a st2tost2
   * \param[in] s : st2tost2 to be inverted
   */
  TFEL_HOST constexpr auto invert(const ST2toST2Concept auto&);
  /*!
   * \return the push-forward of a fourth order tensor:
   * \f[
   * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
   * \f]
   * \param[in] C: input
   * \param[in] F: deformation gradient
   */
  template <ST2toST2Concept ST2toST2Type, TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto push_forward(const ST2toST2Type&,
                                               const TensorType&) noexcept
      requires(getSpaceDimension<ST2toST2Type>() ==
               getSpaceDimension<TensorType>());
  /*!
   * \return the pull-back of a fourth order tensor
   * \param[in] C: input
   * \param[in] F: deformation gradient
   */
  template <ST2toST2Concept ST2toST2Type, TensorConcept TensorType>
  TFEL_HOST constexpr auto pull_back(
      const ST2toST2Type&,
      const TensorType&) requires(getSpaceDimension<ST2toST2Type>() ==
                                  getSpaceDimension<TensorType>());
  /*!
   * \brief compute the second derivative of determinant of a
   * \param[in] s: tensor
   */
  TFEL_HOST_DEVICE constexpr auto computeDeterminantSecondDerivative(
      const StensorConcept auto&) noexcept;
  /*!
   * \brief compute the second derivative of the determinant of the
   * deviator of symmetric tensor.

   * Let \f$\underline{s}\f$ be a symmetric tensor and \f$J_{3}\f$
   * be the determinant of \f$\underline{s}'\f$ the deviator of
   * \f$\underline{s}\f$:
   * \f[
   * J_{3} = \mathrm{det}\left(\underline{s}'\right)
   *       =
   \mathrm{det}\left(\underline{s}-\mathrm{tr}\left(\underline{s}'\right)\,\underline{I}\right)
   * \f]
   *
   * This function computes \f$\displaystyle\frac{\partial^{2} J_{3}}{\partial
   * \underline{\sigma}^{2}}\f$.
   *
   * \param[in] s: tensor
   */
  TFEL_HOST_DEVICE constexpr auto computeDeviatorDeterminantSecondDerivative(
      const StensorConcept auto&) noexcept;

}  // end of namespace tfel::math

namespace tfel::typetraits {
  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::st2tost2<N, T2>,
                        tfel::math::st2tost2<N, T>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/ST2toST2/st2tost2.ixx"
#include "TFEL/Math/ST2toST2/st2tost2ResultType.hxx"

#endif /* LIB_TFEL_MATH_ST2TOST2_HXX */
