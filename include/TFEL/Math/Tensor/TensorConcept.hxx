/*!
 * \file   include/TFEL/Math/Tensor/TensorConcept.hxx
 * \brief
 * \author Thomas Helfer
 * \date   01 Déc 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/MathException.hxx"
#include "TFEL/Math/Forward/tensor.hxx"
#include "TFEL/Math/Forward/stensor.hxx"
#include "TFEL/Math/Forward/MatrixConcept.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorTransposeExpr.hxx"

namespace tfel::math {

  //! forward declaration
  template <typename TensorType>
  struct MatrixViewFromTensorExpr;

  /*!
   * \brief a traits class for tensors. This shall be specialised
   * for classes implementing the tensor concept (@see TensorConcept).
   * \tparam T: class
   */
  template <typename T>
  struct TensorTraits {
    //! numeric type using by the tensor
    typedef tfel::meta::InvalidType NumType;
    //! index type
    typedef unsigned short IndexType;
    //! space dimension
    static constexpr unsigned short dime = 0u;
  };  // end of struct TensorTraits

  //! a simple alias
  template <typename T>
  using TensorNumType = typename TensorTraits<T>::NumType;

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

    TensorNumType<T> operator()(const unsigned short,
                                const unsigned short) const;

    TensorNumType<T> operator()(const unsigned short) const;

    TensorNumType<T> operator[](const unsigned short) const;

   protected:
    TensorConcept() = default;
    TensorConcept(TensorConcept&&) = default;
    TensorConcept(const TensorConcept&) = default;
    TensorConcept& operator=(const TensorConcept&) = default;
    ~TensorConcept() = default;
  };

  template <typename TensorType>
  constexpr bool implementsTensorConcept() {
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
      typename tfel::typetraits::AbsType<TensorNumType<TensorType>>::type>
  abs(const TensorType&);

  template <typename TensorType>
  TFEL_MATH_INLINE std::enable_if_t<implementsTensorConcept<TensorType>(),
                                    TensorNumType<TensorType>>
  trace(const TensorType& s);

  template <typename TensorType>
  TFEL_MATH_INLINE std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      stensor<TensorTraits<TensorType>::dime, TensorNumType<TensorType>>>
  syme(const TensorType&);

  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      (implementsTensorConcept<TensorType>()) &&
          (tfel::typetraits::IsFundamentalNumericType<
              TensorNumType<TensorType>>::cond),
      stensor<TensorTraits<TensorType>::dime, TensorNumType<TensorType>>>
  computeRightCauchyGreenTensor(const TensorType&);

  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      (implementsTensorConcept<TensorType>()) &&
          (tfel::typetraits::IsFundamentalNumericType<
              TensorNumType<TensorType>>::cond),
      stensor<TensorTraits<TensorType>::dime, TensorNumType<TensorType>>>
  computeLeftCauchyGreenTensor(const TensorType&);

  template <typename TensorType>
  TFEL_MATH_INLINE2 std::enable_if_t<
      (implementsTensorConcept<TensorType>()) &&
          (tfel::typetraits::IsFundamentalNumericType<
              TensorNumType<TensorType>>::cond),
      stensor<TensorTraits<TensorType>::dime, TensorNumType<TensorType>>>
  computeGreenLagrangeTensor(const TensorType&);

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
           TensorNumType<StensorType>>::cond)),
      stensor<StensorTraits<StensorType>::dime, StensorNumType<StensorType>>>
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
           TensorNumType<StensorType>>::cond)),
      stensor<StensorTraits<StensorType>::dime, StensorNumType<StensorType>>>
  push_forward(const StensorType&, const TensorType&);

  template <typename StensorType, typename TensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       (implementsTensorConcept<TensorType>()) &&
       (tfel::typetraits::IsFundamentalNumericType<
           TensorNumType<TensorType>>::cond)),
      stensor<StensorTraits<StensorType>::dime, StensorNumType<StensorType>>>
  convertCauchyStressToSecondPiolaKirchhoffStress(const StensorType&,
                                                  const TensorType&);

  template <typename StensorType, typename TensorType>
  std::enable_if_t<
      ((implementsStensorConcept<StensorType>()) &&
       (implementsTensorConcept<TensorType>()) &&
       (tfel::typetraits::IsFundamentalNumericType<
           TensorNumType<TensorType>>::cond)),
      stensor<StensorTraits<StensorType>::dime, StensorNumType<StensorType>>>
  convertSecondPiolaKirchhoffStressToCauchyStress(const StensorType&,
                                                  const TensorType&);

  template <typename TensorType>
  std::enable_if_t<
      implementsTensorConcept<TensorType>(),
      typename ComputeUnaryResult<TensorNumType<TensorType>, Power<3>>::Result>
  det(const TensorType&);

  /*!
   * \brief compute the derivative of the determinant with respect
   * to its argument.
   * \param[out] : determinant derivative
   * \param[in]  : argument
   */
  template <typename TensorResultType, typename TensorType>
  std::enable_if_t<
      implementsTensorConcept<TensorResultType>() &&
          implementsTensorConcept<TensorType>() &&
          tfel::typetraits::IsAssignableTo<
              typename ComputeUnaryResult<TensorNumType<TensorType>,
                                          Power<2>>::Result,
              TensorNumType<TensorResultType>>::cond,
      void>
  computeDeterminantDerivative(TensorResultType&, const TensorType&);
  /*!
   * \brief provide the polar decomposition of a tensor
   * \param[out] rotation
   * \param[out] stretch
   * \param[in]  F
   */
  template <typename TensorType, typename StensorType, typename TensorType2>
  std::enable_if_t<
      implementsTensorConcept<TensorType>() &&
          implementsTensorConcept<TensorType2>() &&
          implementsStensorConcept<StensorType>() &&
          std::is_same<StensorNumType<StensorType>,
                       TensorNumType<TensorType2>>::value &&
          std::is_same<tfel::typetraits::base_type<TensorNumType<TensorType2>>,
                       TensorNumType<TensorType>>::value &&
          (TensorTraits<TensorType>::dime == TensorTraits<TensorType2>::dime) &&
          (TensorTraits<TensorType>::dime ==
           StensorTraits<StensorType>::dime) &&
          (TensorTraits<TensorType>::dime == 1u),
      void>
  polar_decomposition(TensorType&, StensorType&, const TensorType2&);

  /*!
   * \brief provide the polar decomposition of a tensor
   * \param[out] rotation
   * \param[out] stretch
   * \param[in]  F
   */
  template <typename TensorType, typename StensorType, typename TensorType2>
  std::enable_if_t<
      implementsTensorConcept<TensorType>() &&
          implementsTensorConcept<TensorType2>() &&
          implementsStensorConcept<StensorType>() &&
          std::is_same<StensorNumType<StensorType>,
                       TensorNumType<TensorType2>>::value &&
          std::is_same<tfel::typetraits::base_type<TensorNumType<TensorType2>>,
                       TensorNumType<TensorType>>::value &&
          (TensorTraits<TensorType>::dime == TensorTraits<TensorType2>::dime) &&
          (TensorTraits<TensorType>::dime ==
           StensorTraits<StensorType>::dime) &&
          ((TensorTraits<TensorType>::dime == 2u) ||
           (TensorTraits<TensorType>::dime == 3u)),
      void>
  polar_decomposition(TensorType&, StensorType&, const TensorType2&);

  // template<typename TensorType>
  // TFEL_MATH_INLINE auto
  // matrix_view(TensorType&& t)
  // -> std::enable_if_t<
  //   tfel::meta::Implements<typename
  //   std::decay<TensorType>::type,TensorConcept>::cond,
  //   Expr<tmatrix<3u,3u,TensorNumType<typename
  //   std::decay<decltype(t)>::type>>,
  //   MatrixViewFromTensorExpr<decltype(t)>>>::type;

  // /*!
  //  * \return a transposed view of  the tensor
  //  */
  // template<typename TensorType>
  // TFEL_MATH_INLINE auto
  // transpose(TensorType&& t)
  // -> std::enable_if_t<
  //   tfel::meta::Implements<typename
  //   std::decay<TensorType>::type,TensorConcept>::cond &&
  //   (TensorTraits<std::decay_t<TensorType>>::dime==1u),
  //   Expr<tensor<1u,TensorNumType<std::decay_t<decltype(t)>>>,
  // 	   TensorTransposeExpr1D<decltype(t)>>>::type;

  // /*!
  //  * \return a transposed view of  the tensor
  //  */
  // template<typename TensorType>
  // TFEL_MATH_INLINE auto
  // transpose(TensorType&& t)
  // -> std::enable_if_t<
  //   tfel::meta::Implements<typename
  //   std::decay<TensorType>::type,TensorConcept>::cond &&
  //   (TensorTraits<std::decay_t<TensorType>>::dime==2u),
  //   Expr<tensor<2u,TensorNumType<std::decay_t<decltype(t)>>>,
  // 	   TensorTransposeExpr2D<decltype(t)>>>::type;

  // /*!
  //  * \return a transposed view of  the tensor
  //  */
  // template<typename TensorType>
  // TFEL_MATH_INLINE auto
  // transpose(TensorType&& t)
  // -> std::enable_if_t<
  //   tfel::meta::Implements<typename
  //   std::decay<TensorType>::type,TensorConcept>::cond &&
  //   (TensorTraits<std::decay_t<TensorType>>::dime==3u),
  //   Expr<tensor<3u,TensorNumType<std::decay_t<decltype(t)>>>,
  // 	   TensorTransposeExpr3D<decltype(t)>>>::type;

}  // end of namespace tfel::math

#include "TFEL/Math/Tensor/TensorConcept.ixx"

#endif /* LIB_TFEL_MATH_TENSORCONCEPT_HXX */
