/*!
 * \file   include/TFEL/Math/Tensor/TensorConcept.hxx
 * \brief
 * \author Thomas Helfer
 * \date   01/12/2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORCONCEPT_HXX
#define LIB_TFEL_MATH_TENSORCONCEPT_HXX 1

#include <stdexcept>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/MathException.hxx"
#include "TFEL/Math/Forward/tensor.hxx"
#include "TFEL/Math/Forward/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel::math {

  /*!
   * \class TensorTag
   * \brief an helper class to characterize tensors.
   */
  struct TensorTag {};  // end of TensorTag

  /*!
   * \brief definition of the StensorConcept
   * a class matching the stensor concept must expose the `StensorTag` and have
   * access operators.
   */
  template <typename T>
  concept TensorConcept =
      (std::is_same_v<typename std::decay_t<T>::ConceptTag, TensorTag>)&&  //
      (requires(const T t, const unsigned short i) { t[i]; }) &&           //
      (requires(const T t, const unsigned short i) { t(i); });

  //! forward declaration
  template <TensorConcept TensorType>
  struct MatrixViewFromTensorExpr;

  /*!
   * \brief exception thrown if invalid index is given
   */
  struct TFELMATH_VISIBILITY_EXPORT TensorInvalidIndexException
      : public MathRunTimeException {
    TensorInvalidIndexException() = default;
    TensorInvalidIndexException(const TensorInvalidIndexException&) = default;
    TensorInvalidIndexException(TensorInvalidIndexException&&) = default;
    const char* what() const noexcept override final;
    ~TensorInvalidIndexException() noexcept override;
  };  // end of struct TensorInvalidIndexException

  /*!
   * \brief exception thrown if invalid indexes were given
   */
  struct TFELMATH_VISIBILITY_EXPORT TensorInvalidIndexesException
      : public MathRunTimeException {
    TensorInvalidIndexesException() = default;
    TensorInvalidIndexesException(const TensorInvalidIndexesException&) =
        default;
    TensorInvalidIndexesException(TensorInvalidIndexesException&&) = default;
    const char* what() const noexcept override final;
    ~TensorInvalidIndexesException() noexcept override;
  };  // end of struct TensorInvalidIndexesException

  /*!
   * \brief exception thrown if an std::initializer_list with no
   * appropriate size is affected to a tensor.
   */
  struct TFELMATH_VISIBILITY_EXPORT TensorInvalidInitializerListSizeException
      : public MathRunTimeException {
    TensorInvalidInitializerListSizeException() = default;
    TensorInvalidInitializerListSizeException(
        const TensorInvalidInitializerListSizeException&) = default;
    TensorInvalidInitializerListSizeException(
        TensorInvalidInitializerListSizeException&&) = default;
    const char* what() const noexcept override final;
    ~TensorInvalidInitializerListSizeException() noexcept override;
  };  // end of struct TensorInvalidInitializerListSizeException

  //! \brief base class which implements the tensor concept
  template <typename T>
  struct TensorConceptBase {
    //! \brief alias exposing TensorTag as the concept that must be satisfied by
    //! the T-type
    using ConceptTag = TensorTag;
    /*!
     * \brief matrix-like access to values
     * \param[in] i: row number
     * \param[in] j: column number
     */
    TFEL_HOST numeric_type<T> operator()(const unsigned short,
                                         const unsigned short) const;
  };

  //! partial specialisation for tensors
  template <typename Type>
  struct ConceptRebind<TensorTag, Type> {
    using type = TensorConceptBase<Type>;
  };
  /*!
   * \return the sum of the absolute value of the components of a tensor
   * \param[in] t: tensor
   */
  TFEL_HOST_DEVICE constexpr auto abs(const TensorConcept auto&) noexcept;
  /*!
   * \return the trace of a tensor
   * \param[in] t: tensor
   */
  TFEL_HOST_DEVICE constexpr auto trace(const TensorConcept auto&) noexcept;
  /*!
   * \return the symmetric part of a tensor
   * \param[in] t: tensor
   */
  TFEL_HOST_DEVICE constexpr auto syme(const TensorConcept auto&) noexcept;
  /*!
   * \return the right Cauchy-Green tensor
   * \param[in] t: tensor
   */
  TFEL_HOST_DEVICE constexpr auto computeRightCauchyGreenTensor(
      const TensorConcept auto&) noexcept;
  /*!
   * \return the left Cauchy-Green tensor
   * \param[in] t: tensor
   */
  TFEL_HOST_DEVICE constexpr auto computeLeftCauchyGreenTensor(
      const TensorConcept auto&) noexcept;
  /*!
   * \return the left Green-Lagrange tensor
   * \param[in] t: tensor
   */
  TFEL_HOST_DEVICE constexpr auto computeGreenLagrangeTensor(
      const TensorConcept auto&) noexcept;
  /*!
   * compute the product:
   * \[F.s.F^{T}\]
   * \param[in] p: a symmetric tensor
   * \param[in] F: a tensor
   */
  TFEL_HOST_DEVICE constexpr auto pushForward(
      const StensorConcept auto&, const TensorConcept auto&) noexcept;
  /*!
   * compute the product:
   * \[F.s.F^{T}\]
   * \param[in] p: a symmetric tensor
   * \param[in] F: a tensor
   * \note the same as pushForward
   */
  TFEL_HOST_DEVICE constexpr auto push_forward(
      const StensorConcept auto&, const TensorConcept auto&) noexcept;
  /*!
   * \brief convert the Cauchy stress to the second Piola-Kirchhoff stress
   * \param[in] s: Cauchy stress
   * \param[in] F: deformation gradient
   */
  TFEL_HOST_DEVICE constexpr auto
  convertCauchyStressToSecondPiolaKirchhoffStress(
      const StensorConcept auto&, const TensorConcept auto&) noexcept;
  /*!
   * \brief convert the second Piola-Kirchhoff stress to the Cauchy stress
   * \param[in] p: Cauchy stress
   * \param[in] F: deformation gradient
   */
  TFEL_HOST_DEVICE constexpr auto
  convertSecondPiolaKirchhoffStressToCauchyStress(
      const StensorConcept auto&, const TensorConcept auto&) noexcept;
  /*!
   * \return the determinant of a tensor
   * \param[in] t: tensor
   */
  TFEL_HOST_DEVICE constexpr auto det(const TensorConcept auto&) noexcept;
  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] : determinant derivative
   * \param[in]  : argument
   */
  TFEL_HOST_DEVICE constexpr auto computeDeterminantDerivative(
      TensorConcept auto&, const TensorConcept auto&) noexcept;
  /*!
   * \brief provide the polar decomposition of a tensor
   * \param[out] R: rotation tensor
   * \param[out] U: stretch tensor
   * \param[in]  F: deformation gradient
   */
  TFEL_HOST void polar_decomposition(TensorConcept auto&,
                                     StensorConcept auto&,
                                     const TensorConcept auto&);
  /*!
   * \return an expression representing a matrix view from a tensor
   * \param[in] t: tensor
   */
  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto matrix_view(TensorType&& t) noexcept;
  /*!
   * \return an expression representing the transpose of a tensor
   * \param[in] t: tensor
   */
  template <TensorConcept TensorType>
  TFEL_HOST_DEVICE constexpr auto transpose(TensorType&&) noexcept;

  /*!
   * \brief an helper function which returns if the given type implements the
   * `TensorConcept`.
   * \tparam TensorType: type tested
   * \note function given for backward compatibility with versions prior
   * to 5.0
   */
  template <typename TensorType>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsTensorConcept() {
    return TensorConcept<TensorType>;
  }  // end of implementsTensorConcept

}  // end of namespace tfel::math

#include "TFEL/Math/Tensor/TensorConcept.ixx"

#endif /* LIB_TFEL_MATH_TENSORCONCEPT_HXX */
