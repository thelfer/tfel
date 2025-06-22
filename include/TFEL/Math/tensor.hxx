/*!
 * \file   include/TFEL/Math/tensor.hxx
 * \brief
 * \author Thomas Helfer
 * \date   01 Déc 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSOR_HXX
#define LIB_TFEL_MATH_TENSOR_HXX

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorConceptOperations.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Forward/tensor.hxx"

namespace tfel::math {

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename TensorType, typename ScalarType>
  struct DerivativeTypeDispatcher<TensorTag,
                                  ScalarTag,
                                  TensorType,
                                  ScalarType> {
    static_assert(implementsTensorConcept<TensorType>(),
                  "template argument TensorType is not a tensor");
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<TensorType>>(),
                  "the tensor type does not hold a scalar");
    //! \brief result
    using type = tensor<getSpaceDimension<TensorType>(),
                        derivative_type<numeric_type<TensorType>, ScalarType>>;
  };  // end of struct DerivativeTypeDispatcher
  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename ScalarType, typename TensorType>
  struct DerivativeTypeDispatcher<ScalarTag,
                                  TensorTag,
                                  ScalarType,
                                  TensorType> {
    static_assert(implementsTensorConcept<TensorType>(),
                  "template argument TensorType is not a tensor");
    static_assert(isScalar<ScalarType>(),
                  "template argument ScalarType is not a scalar");
    static_assert(isScalar<numeric_type<TensorType>>(),
                  "the tensor type does not hold a scalar");
    //! \brief result
    using type = tensor<getSpaceDimension<TensorType>(),
                        derivative_type<ScalarType, numeric_type<TensorType>>>;
  };  // end of struct DerivativeTypeDispatcher

  template <unsigned short N, typename ValueType>
  struct tensor
      : TensorConcept<tensor<N, ValueType>>,
        GenericFixedSizeArray<
            tensor<N, ValueType>,
            FixedSizeVectorPolicy<TensorDimeToSize<N>::value, ValueType>> {
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    //! a simple alias
    using GenericFixedSizeArrayBase = GenericFixedSizeArray<
        tensor<N, ValueType>,
        FixedSizeVectorPolicy<TensorDimeToSize<N>::value, ValueType>>;
    /*!
     * \return the identity tensor
     */
    static constexpr tensor<N, base_type<ValueType>> Id();
    /*!
     * \brief Build a tensor from a fortran matrix.
     * \param[in] t: tensor to be filled
     * \param[in] v: pointer to an array used to initialise the
     * components of the tensor. This array is left unchanged.
     */
    TFEL_MATH_INLINE2 static void buildFromFortranMatrix(
        tensor<N, ValueType>&, const base_type<ValueType>* const);
    /*!
     * \brief Build a tensor from a fortran matrix.
     * \param[in] t: tensor to be filled
     * \param[in] v: pointer to an array used to initialise the
     * components of the tensor. This array is left unchanged.
     */
    TFEL_MATH_INLINE2 static tensor<N, ValueType> buildFromFortranMatrix(
        const base_type<ValueType>* const);
    //
    TFEL_MATH_FIXED_SIZE_ARRAY_DEFAULT_METHODS(tensor,
                                               GenericFixedSizeArrayBase);
    // inheriting GenericFixedSizeArray' access operators
    using GenericFixedSizeArray<
        tensor<N, ValueType>,
        FixedSizeVectorPolicy<TensorDimeToSize<N>::value,
                              ValueType>>::operator[];
    /*!
     * \brief access operator
     * \param[in] i: index
     */
    constexpr ValueType operator()(const typename tensor::size_type) const;
    /*!
     * \brief access operator
     * \param[in] i: index
     */
    constexpr ValueType& operator()(const typename tensor::size_type);
    /*!
     * \brief matrix-like access operator
     * \param[in] i: row number
     * \param[in] j: column number
     */
    ValueType operator()(const typename tensor::size_type,
                         const typename tensor::size_type) const;
    //! \brief write to an external memory location
    TFEL_MATH_INLINE2 void write(base_type<ValueType>* const) const;
    //! \brief import values from an external memory location
    void import(const base_type<ValueType>* const);
    //! change basis
    TFEL_MATH_INLINE2 void changeBasis(
        const rotation_matrix<ValueType>&) noexcept;

    template <typename InputIterator>
    TFEL_MATH_INLINE2 void copy(const InputIterator src);
  };  // end of struct tensor

  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using TensorView = View<tensor<N, T>>;
  /*!
   * \brief a simple alias for backward compatibility
   * \tparam N: space dimension
   * \tparam T: value type
   */
  template <unsigned short N, typename T>
  using ConstTensorView = ConstView<tensor<N, T>>;

  template <unsigned short N, typename T, typename OutputIterator>
  TFEL_MATH_INLINE2 std::enable_if_t<isScalar<T>(), void> exportToBaseTypeArray(
      const tensor<N, T>&, OutputIterator);
  /*!
   * \return the invert of a tensor
   * \param[in] t : tensor to be inverted
   */
  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      tensor<getSpaceDimension<TensorType>(),
             typename ComputeBinaryResult<base_type<numeric_type<TensorType>>,
                                          numeric_type<TensorType>,
                                          OpDiv>::Result>>
  invert(const TensorType&) noexcept;
  /*!
   * \return the derivative of the determinant
   * \param[in] F: tensor where the the determinant is evaluated
   */
  template <typename TensorType>
  std::enable_if_t<implementsTensorConcept<TensorType>(),
                   tensor<getSpaceDimension<TensorType>(),
                          typename ComputeUnaryResult<numeric_type<TensorType>,
                                                      Power<2>>::Result>>
  computeDeterminantDerivative(const TensorType&);
  /*!
   * \brief rotate a tensor using a rotation matrix
   * \param[in] s: tensor to be rotated
   * \param[in] r: rotation matrix
   * \return the rotated tensor
   */
  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      tensor<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  change_basis(const TensorType&,
               const rotation_matrix<numeric_type<TensorType>>&) noexcept;
  /*!
   * \return the unsymmetric tensor corresponding to the given symmetric tensor.
   * \param[in] s: symmetric tensor
   */
  template <typename StensorType>
  TFEL_MATH_INLINE std::enable_if_t<
      implementsStensorConcept<StensorType>(),
      tensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  unsyme(const StensorType&);
  /*!
   * \brief convert the Cauchy stress to the first Piola-Kirchhoff stress.
   *
   * \tparam StensorType: type of the  Cauchy stress
   * \tparam TensorType: type of the  deformation gradient
   *
   * \param[in] s: Cauchy stress
   * \param[in] F: deformation gradient
   * \return the first Piola-Kirchhoff stress
   */
  template <typename StensorType, typename TensorType>
  TFEL_MATH_INLINE
      std::enable_if_t<(implementsStensorConcept<StensorType>() &&
                        implementsTensorConcept<TensorType>()),
                       tensor<getSpaceDimension<StensorType>(),
                              result_type<numeric_type<StensorType>,
                                          numeric_type<TensorType>,
                                          OpMult>>>
      convertCauchyStressToFirstPiolaKirchhoffStress(const StensorType&,
                                                     const TensorType&);
  /*!
   * \brief convert the first Piola-Kirchhoff stress to the Cauchy stress
   * \tparam TensorType: type of the  first Piola-Kirchhoff stress
   * \tparam TensorType2: type of the  deformation gradient
   *
   * \param[in] P: Cauchy stress
   * \param[in] F: deformation gradient
   * \return the Cauchy stress
   */
  template <typename TensorType, typename TensorType2>
  std::enable_if_t<(implementsTensorConcept<TensorType>() &&
                    implementsTensorConcept<TensorType2>()),
                   stensor<getSpaceDimension<TensorType>(),
                           result_type<numeric_type<TensorType>,
                                       numeric_type<TensorType2>,
                                       OpMult>>>
  convertFirstPiolaKirchhoffStressToCauchyStress(const TensorType&,
                                                 const TensorType2&);

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::tensor<N, T2>, tfel::math::tensor<N, T>> {
    //! result
    static constexpr bool cond = isAssignableTo<T2, T>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/tensor.ixx"
#include "TFEL/Math/Tensor/tensorResultType.hxx"

#endif /* LIB_TFEL_MATH_TENSOR_HXX */
