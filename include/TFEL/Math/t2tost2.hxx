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
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
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
  template <typename T2toST2Type, typename ScalarType>
  struct DerivativeTypeDispatcher<T2toST2Tag,
                                  ScalarTag,
                                  T2toST2Type,
                                  ScalarType> {
    static_assert(implementsT2toST2Concept<T2toST2Type>(),
                  "template argument T2toST2Type is not a t2tost2");
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
  template <typename ScalarType, typename T2toST2Type>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  T2toST2Tag,
                                  ScalarType,
                                  T2toST2Type> {
    static_assert(implementsT2toST2Concept<T2toST2Type>(),
                  "template argument T2toST2Type is not a t2tost2");
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
  template <typename StensorType1, typename TensorType2>
  struct DerivativeTypeDispatcher<StensorTag,
                                  TensorTag,
                                  StensorType1,
                                  TensorType2> {
    static_assert(implementsStensorConcept<StensorType1>(),
                  "template argument StensorType1 is not a symmetric tensor");
    static_assert(implementsTensorConcept<TensorType2>(),
                  "template argument TensorType2 is not a tensor");
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
      : T2toST2Concept<t2tost2<N, ValueType>>,
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
    template <typename TensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsTensorConcept<TensorType>() &&
            getSpaceDimension<TensorType>() == N &&
            isAssignableTo<numeric_type<TensorType>, ValueType>(),
        Expr<t2tost2<N, ValueType>, RightCauchyGreenTensorDerivativeExpr<N>>>
    dCdF(const TensorType&);
    /*!
     * \param[in] F : deformation gradient
     * \return the derivative of the Cauchy left symmetric tensor
     * with respect to the deformation gradient
     */
    template <typename TensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsTensorConcept<TensorType>() &&
            getSpaceDimension<TensorType>() == N &&
            isAssignableTo<numeric_type<TensorType>, ValueType>(),
        Expr<t2tost2<N, ValueType>, LeftCauchyGreenTensorDerivativeExpr<N>>>
    dBdF(const TensorType&);
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(t2tost2,
                                               GenericFixedSizeArrayBase);
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArrayBase::operator[];
    using GenericFixedSizeArrayBase::operator();
    //! \brief import values
    template <typename T2>
    std::enable_if_t<std::is_convertible_v<T2, base_type<ValueType>>, void>
    import(const T2* const);

    template <typename InputIterator>
    TFEL_MATH_INLINE2 void copy(const InputIterator src);
  };

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
   * \return change the basis of a 1D t2tost2
   * \param[in] s: t2tost2
   * \param[in] r: rotation matrix
   */
  template <typename T2toST2Type>
  std::enable_if_t<((implementsT2toST2Concept<T2toST2Type>()) &&
                    (getSpaceDimension<T2toST2Type>() == 1u)),
                   t2tost2<1u, numeric_type<T2toST2Type>>>
  change_basis(const T2toST2Type&,
               const rotation_matrix<numeric_type<T2toST2Type>>&);
  /*!
   * \return change the basis of a 2D or 3D t2tost2
   * \param[in] s: t2tost2
   * \param[in] r: rotation matrix
   */
  template <typename T2toST2Type>
  std::enable_if_t<
      ((implementsT2toST2Concept<T2toST2Type>()) &&
       (getSpaceDimension<T2toST2Type>() != 1u)),
      t2tost2<getSpaceDimension<T2toST2Type>(), numeric_type<T2toST2Type>>>
  change_basis(const T2toST2Type& s,
               const rotation_matrix<numeric_type<T2toST2Type>>&);

  /*!
   * convert a t2tot2 tensor to a t2tost2
   */
  template <typename T2toT2Type>
  TFEL_MATH_INLINE std::enable_if_t<((getSpaceDimension<T2toT2Type>() == 1u) &&
                                     implementsT2toT2Concept<T2toT2Type>()),
                                    t2tost2<1u, numeric_type<T2toT2Type>>>
  convertToT2toST2(const T2toT2Type&);

  /*!
   * convert a t2tot2 tensor to a t2tost2
   */
  template <typename T2toT2Type>
  TFEL_MATH_INLINE std::enable_if_t<((getSpaceDimension<T2toT2Type>() == 2u) &&
                                     implementsT2toT2Concept<T2toT2Type>()),
                                    t2tost2<2u, numeric_type<T2toT2Type>>>
  convertToT2toST2(const T2toT2Type&);

  /*!
   * convert a t2tot2 tensor to a t2tost2
   */
  template <typename T2toT2Type>
  TFEL_MATH_INLINE std::enable_if_t<((getSpaceDimension<T2toT2Type>() == 3u) &&
                                     implementsT2toT2Concept<T2toT2Type>()),
                                    t2tost2<3u, numeric_type<T2toT2Type>>>
  convertToT2toST2(const T2toT2Type&);

  /*!
   * \return compute the "derivative" of the rate of deformation
   * \delta D = dD_dF:\delta F with \delta D = ((delta F).F^{-1}+F^{-T}.(delta
   * F)^{T})/2
   * \param[in] F : deformation gradient
   */
  template <typename TensorType>
  TFEL_MATH_INLINE2
      std::enable_if_t<(implementsTensorConcept<TensorType>()) &&
                           (getSpaceDimension<TensorType>() == 1u),
                       t2tost2<1u, numeric_type<TensorType>>>
      computeRateOfDeformationDerivative(const TensorType&);
  /*!
   * \return compute the derivative of the rate of deformation
   * \param[in] F : deformation gradient
   */
  template <typename TensorType>
  TFEL_MATH_INLINE2
      std::enable_if_t<(implementsTensorConcept<TensorType>()) &&
                           (getSpaceDimension<TensorType>() == 2u),
                       t2tost2<2u, numeric_type<TensorType>>>
      computeRateOfDeformationDerivative(const TensorType&);
  /*!
   * \return compute the derivative of the rate of deformation
   * \param[in] F : deformation gradient
   */
  template <typename TensorType>
  TFEL_MATH_INLINE2
      std::enable_if_t<(implementsTensorConcept<TensorType>()) &&
                           (getSpaceDimension<TensorType>() == 1u),
                       t2tost2<1u, numeric_type<TensorType>>>
      computeRateOfDeformationDerivative(const TensorType&);
  /*!
   * \brief compute the Cauchy stress derivative from the Kirchhoff stress
   * derivative
   * \param[in]  dt: Kirchhoff stress derivative
   * \param[in]  s:  Cauchy stress
   * \param[in]  F:  deformation gradient
   */
  template <typename T2toST2Type, typename StensorType, typename TensorType>
  std::enable_if_t<
      implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2Type>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2Type>() == getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond,
      t2tost2<getSpaceDimension<T2toST2Type>(),
              typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                           numeric_type<StensorType>,
                                           OpPlus>::Result>>
  computeCauchyStressDerivativeFromKirchhoffStressDerivative(const T2toST2Type&,
                                                             const StensorType&,
                                                             const TensorType&);
  /*!
   * \brief compute the Cauchy stress derivative from the Kirchhoff stress
   * derivative
   * \param[in]  dt: Kirchhoff stress derivative
   * \param[in]  s:  Cauchy stress
   * \param[in]  F:  deformation gradient
   */
  template <typename T2toST2Type, typename StensorType, typename TensorType>
  std::enable_if_t<
      implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2Type>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2Type>() == getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond,
      t2tost2<getSpaceDimension<T2toST2Type>(),
              typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                           numeric_type<StensorType>,
                                           OpPlus>::Result>>
  computeKirchhoffStressDerivativeFromCauchyStressDerivative(const T2toST2Type&,
                                                             const StensorType&,
                                                             const TensorType&);

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
