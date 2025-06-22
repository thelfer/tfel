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
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
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
  template <typename ST2toST2Type, typename ScalarType>
  struct DerivativeTypeDispatcher<ST2toST2Tag,
                                  ScalarTag,
                                  ST2toST2Type,
                                  ScalarType> {
    static_assert(implementsST2toST2Concept<ST2toST2Type>(),
                  "template argument ST2toST2Type is not a st2tost2");
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
  template <typename ScalarType, typename ST2toST2Type>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  ST2toST2Tag,
                                  ScalarType,
                                  ST2toST2Type> {
    static_assert(implementsST2toST2Concept<ST2toST2Type>(),
                  "template argument ST2toST2Type is not a st2tost2");
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
  template <typename StensorType1, typename StensorType2>
  struct DerivativeTypeDispatcher<StensorTag,
                                  StensorTag,
                                  StensorType1,
                                  StensorType2> {
    static_assert(implementsStensorConcept<StensorType1>(),
                  "template argument StensorType1 is not a symmetric tensor");
    static_assert(implementsStensorConcept<StensorType2>(),
                  "template argument StensorType2 is not a symmetric tensor");
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
      : ST2toST2Concept<st2tost2<N, ValueType>>,
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
    template <typename StensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            getSpaceDimension<StensorType>() == N &&
            isAssignableTo<numeric_type<StensorType>, ValueType>(),
        Expr<st2tost2<N, ValueType>, StensorSquareDerivativeExpr<N>>>
    dsquare(const StensorType&);
    /*!
     * \param[in] s : tensor squared
     * \param[in] C : derivative of s
     * \return the derivative of the square of a symmetric tensor
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
        Expr<st2tost2<N, ValueType>, StensorSquareDerivativeExpr<N>>>
    dsquare(const StensorType&, const ST2toST2Type&);
    /*!
     * convert a T2toST2 to a ST2toST2
     * \param[in] src : T2toST2 to be converted
     */
    template <typename T2toST2Type>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsT2toST2Concept<T2toST2Type>() &&
            getSpaceDimension<T2toST2Type>() == N &&
            isAssignableTo<numeric_type<T2toST2Type>, ValueType>(),
        Expr<st2tost2<N, ValueType>, ConvertT2toST2ToST2toST2Expr<N>>>
    convert(const T2toST2Type&);
    /*!
     * build the equivalent st2tost2 from a rotation matrix
     * \param[in] r : rotation matrix
     */
    static tfel::math::st2tost2<N, base_type<ValueType>> fromRotationMatrix(
        const rotation_matrix<ValueType>&);
    /*!
     * \brief compute the derivative of the symmetric tensor product:
     * \f[
     * \frac{d}{ds1}\left(s1*s+s*s1\right)
     * \f]
     * \param[in] s: second tensor of the product
     */
    template <typename StensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            getSpaceDimension<StensorType>() == N &&
            isAssignableTo<numeric_type<StensorType>, ValueType>(),
        tfel::math::st2tost2<N, ValueType>>
    stpd(const StensorType&);
    //
    static constexpr st2tost2 Id();
    static constexpr st2tost2 IxI();
    static constexpr st2tost2 K();
    static constexpr st2tost2 M();
    static constexpr st2tost2 J();
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(st2tost2,
                                               GenericFixedSizeArrayBase);
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! \brief import values from an external memory location
    void import(const base_type<ValueType>* const);

    template <typename InputIterator>
    TFEL_MATH_INLINE2 void copy(const InputIterator src);
  };

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
  template <typename ST2toST2Type>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>(),
      st2tost2<getSpaceDimension<ST2toST2Type>(), numeric_type<ST2toST2Type>>>
  change_basis(const ST2toST2Type&,
               const rotation_matrix<numeric_type<ST2toST2Type>>&);
  /*!
   * \return the invert of a st2tost2
   * \param[in] s : st2tost2 to be inverted
   */
  template <typename ST2toST2Type>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsST2toST2Concept<ST2toST2Type>(),
      st2tost2<getSpaceDimension<ST2toST2Type>(),
               BinaryOperationResult<base_type<numeric_type<ST2toST2Type>>,
                                     numeric_type<ST2toST2Type>,
                                     OpDiv>>>
  invert(const ST2toST2Type&);

  /*!
   * \return the push_forward of a st2tost2:
   * \f[
   * Ct_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C_{mnpq}
   * \f]
   * \param[in] C: input
   * \param[in] F: deformation gradient
   */
  template <typename ST2toST2Type, typename TensorType>
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       implementsTensorConcept<TensorType>() &&
                       getSpaceDimension<ST2toST2Type>() ==
                           getSpaceDimension<TensorType>(),
                   st2tost2<getSpaceDimension<ST2toST2Type>(),
                            BinaryOperationResult<numeric_type<ST2toST2Type>,
                                                  numeric_type<TensorType>,
                                                  OpMult>>>
  push_forward(const ST2toST2Type&, const TensorType&);

  template <typename ST2toST2Type, typename TensorType>
  std::enable_if_t<implementsST2toST2Concept<ST2toST2Type>() &&
                       implementsTensorConcept<TensorType>() &&
                       getSpaceDimension<ST2toST2Type>() ==
                           getSpaceDimension<TensorType>(),
                   st2tost2<getSpaceDimension<ST2toST2Type>(),
                            BinaryOperationResult<numeric_type<ST2toST2Type>,
                                                  numeric_type<TensorType>,
                                                  OpMult>>>
  pull_back(const ST2toST2Type&, const TensorType&);
  /*!
   * \brief compute the second derivative of determinant of the
   * deviator of a symmetric tensor with respect to this tensor.
   *
   * Let \f$\underline{s}\f$ be a symmetric tensor and \f$J_{3}\f$
   * be the determinant of \f$\underline{s}'\f$ the deviator of
   * \f$\underline{s}\f$:
   * \f[
   * J_{3} = \mathrm{det}\left(\underline{s}'\right)
   *       =
   * \mathrm{det}\left(\underline{s}-\mathrm{tr}\left(\underline{s}'\right)\,\underline{I}\right)
   * \f]
   *
   * This function computes \f$\displaystyle\frac{\partial^{2} J_{3}}{\partial
   * \underline{\sigma}^{2}}\f$.
   *
   * \[ \param[in] s: tensor
   */
  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>() &&
          isScalar<numeric_type<StensorType>>(),
      st2tost2<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeDeviatorDeterminantSecondDerivative(const StensorType&);
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
   \underline{\sigma}^{2}}\f$.
   *
   * \param[in] s: tensor
   */
  template <typename StensorType>
  std::enable_if_t<
      implementsStensorConcept<StensorType>() &&
          isScalar<numeric_type<StensorType>>(),
      st2tost2<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  computeDeviatorDeterminantSecondDerivative(const StensorType&);

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
