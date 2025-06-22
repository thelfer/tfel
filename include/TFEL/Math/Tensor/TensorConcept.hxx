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

  //! forward declaration
  template <typename TensorType>
  struct MatrixViewFromTensorExpr;

  /*!
   * \class TensorTag
   * \brief an helper class to characterize tensors.
   */
  struct TensorTag {};  // end of TensorTag

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

  /*!
   * The base class which implements the tensor concept
   */
  template <typename T>
  struct TensorConcept {
    typedef TensorTag ConceptTag;

    numeric_type<T> operator()(const unsigned short,
                               const unsigned short) const;

   protected:
    TensorConcept() = default;
    TensorConcept(TensorConcept&&) = default;
    TensorConcept(const TensorConcept&) = default;
    TensorConcept& operator=(const TensorConcept&) = default;
    ~TensorConcept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `TensorConcept`.
   * \tparam TensorType: type tested
   */
  template <typename TensorType>
  TFEL_HOST_DEVICE constexpr bool implementsTensorConcept() {
    return tfel::meta::implements<TensorType, TensorConcept>();
  }  // end of implementsTensorConcept

  //! partial specialisation for tensors
  template <typename Type>
  struct ConceptRebind<TensorTag, Type> {
    using type = TensorConcept<Type>;
  };
  /*!
   * \return the sum of the absolute value of the components of a tensor
   * \tparam TensorType: type of the tensor
   * \param[in] t: tensor
   */
  template <typename TensorType>
  std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      typename tfel::typetraits::AbsType<numeric_type<TensorType>>::type>
  abs(const TensorType&);

  template <typename TensorType>
  TFEL_MATH_INLINE std::enable_if_t<implementsTensorConcept<TensorType>(),
                                    numeric_type<TensorType>>
  trace(const TensorType& s);

  template <typename TensorType>
  TFEL_MATH_INLINE std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      stensor<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  syme(const TensorType&);

  //   template <typename TensorType>
  //   TFEL_MATH_INLINE2 std::enable_if_t<
  //       (implementsTensorConcept<TensorType>()) &&
  //           (tfel::typetraits::IsFundamentalNumericType<
  //               numeric_type<TensorType>>::cond),
  //       stensor<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  //   computeRightCauchyGreenTensor(const TensorType&);

  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      (implementsTensorConcept<TensorType>()) &&
          (tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond),
      stensor<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  computeLeftCauchyGreenTensor(const TensorType&);

  //   template <typename TensorType>
  //   TFEL_HOST_DEVICE constexpr std::enable_if_t<
  //       (implementsTensorConcept<TensorType>()) &&
  //           (tfel::typetraits::IsFundamentalNumericType<
  //               numeric_type<TensorType>>::cond),
  //       stensor<getSpaceDimension<TensorType>(), numeric_type<TensorType>>>
  //   computeGreenLagrangeTensor(const TensorType&);

  /*!
   * compute the product:
   * \[F.s.F^{T}\]
   * \param[in] p: a symmetric tensor
   * \param[in] F: a tensor
   */
  template <typename StensorType, typename TensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       (implementsTensorConcept<TensorType>()) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  pushForward(const StensorType&, const TensorType&);
  /*!
   * compute the product:
   * \[F.s.F^{T}\]
   * \param[in] p: a symmetric tensor
   * \param[in] F: a tensor
   * \note the same as pushForward
   */
  template <typename StensorType, typename TensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       (implementsTensorConcept<TensorType>()) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<StensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  push_forward(const StensorType&, const TensorType&);

  template <typename StensorType, typename TensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       (implementsTensorConcept<TensorType>()) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<TensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  convertCauchyStressToSecondPiolaKirchhoffStress(const StensorType&,
                                                  const TensorType&);

  template <typename StensorType, typename TensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       (implementsTensorConcept<TensorType>()) &&
       (tfel::typetraits::IsFundamentalNumericType<
           numeric_type<TensorType>>::cond)),
      stensor<getSpaceDimension<StensorType>(), numeric_type<StensorType>>>
  convertSecondPiolaKirchhoffStressToCauchyStress(const StensorType&,
                                                  const TensorType&);

  template <typename TensorType>
  std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      typename ComputeUnaryResult<numeric_type<TensorType>, Power<3>>::Result>
  det(const TensorType&);

  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] : determinant derivative
   * \param[in]  : argument
   */
  template <typename TensorResultType, typename TensorType>
  std::enable_if_t<
      (implementsTensorConcept<TensorResultType>() &&
       implementsTensorConcept<TensorType>() &&
       isAssignableTo<typename ComputeUnaryResult<numeric_type<TensorType>,
                                                  Power<2>>::Result,
                      numeric_type<TensorResultType>>()),
      void>
  computeDeterminantDerivative(TensorResultType&, const TensorType&);
  /*!
   * \brief provide the polar decomposition of a tensor
   * \param[out] rotation
   * \param[out] stretch
   * \param[in]  F
   */
  template <typename TensorType, typename StensorType, typename TensorType2>
  std::enable_if_t<implementsTensorConcept<TensorType>() &&
                       implementsTensorConcept<TensorType2>() &&
                       implementsStensorConcept<StensorType>() &&
                       std::is_same<numeric_type<StensorType>,
                                    numeric_type<TensorType2>>::value &&
                       std::is_same<base_type<numeric_type<TensorType2>>,
                                    numeric_type<TensorType>>::value &&
                       (getSpaceDimension<TensorType>() ==
                        getSpaceDimension<TensorType2>()) &&
                       (getSpaceDimension<TensorType>() ==
                        getSpaceDimension<StensorType>()) &&
                       (getSpaceDimension<TensorType>() == 1u),
                   void>
  polar_decomposition(TensorType&, StensorType&, const TensorType2&);

  /*!
   * \brief provide the polar decomposition of a tensor
   * \param[out] rotation
   * \param[out] stretch
   * \param[in]  F
   */
  template <typename TensorType, typename StensorType, typename TensorType2>
  std::enable_if_t<implementsTensorConcept<TensorType>() &&
                       implementsTensorConcept<TensorType2>() &&
                       implementsStensorConcept<StensorType>() &&
                       std::is_same<numeric_type<StensorType>,
                                    numeric_type<TensorType2>>::value &&
                       std::is_same<base_type<numeric_type<TensorType2>>,
                                    numeric_type<TensorType>>::value &&
                       (getSpaceDimension<TensorType>() ==
                        getSpaceDimension<TensorType2>()) &&
                       (getSpaceDimension<TensorType>() ==
                        getSpaceDimension<StensorType>()) &&
                       ((getSpaceDimension<TensorType>() == 2u) ||
                        (getSpaceDimension<TensorType>() == 3u)),
                   void>
  polar_decomposition(TensorType&, StensorType&, const TensorType2&);

}  // end of namespace tfel::math

#include "TFEL/Math/Tensor/TensorConcept.ixx"

#endif /* LIB_TFEL_MATH_TENSORCONCEPT_HXX */
