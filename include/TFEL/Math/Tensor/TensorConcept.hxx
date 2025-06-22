/*!
 * \file   include/TFEL/Math/Tensor/TensorConcept.hxx
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

namespace tfel {

  namespace math {

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
    template <class T>
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

    //! paratial specialisation for tensors
    template <typename Type>
    struct ConceptRebind<TensorTag, Type> {
      using type = TensorConcept<Type>;
    };
    /*!
     * \brief a meta function returning the concrete tensor type
     * that the given type is meant to represent.
     *
     * For a standard tensor type, the type is returned.
     * See the partial specialisation for an expression of type `Expr`.
     *
     * \tparam T: input type
     */
    template <typename T>
    struct TensorType {
      //! the result
      typedef T type;
    };

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        typename tfel::typetraits::AbsType<TensorNumType<TensorType>>::type>::
        type
        abs(const TensorType&);

    template <class T>
    TFEL_MATH_INLINE
        typename std::enable_if<tfel::meta::Implements<T, TensorConcept>::cond,
                                TensorNumType<T>>::type
        trace(const T& s);

    template <class T>
    TFEL_MATH_INLINE typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 1u)),
        stensor<1u, TensorNumType<T>>>::type
    syme(const T&);

    template <class T>
    TFEL_MATH_INLINE typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 2u)),
        stensor<2u, TensorNumType<T>>>::type
    syme(const T&);

    template <class T>
    TFEL_MATH_INLINE typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 3u)),
        stensor<3u, TensorNumType<T>>>::type
    syme(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<1u, TensorNumType<T>>>::type
    computeRightCauchyGreenTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<2u, TensorNumType<T>>>::type
    computeRightCauchyGreenTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<3u, TensorNumType<T>>>::type
    computeRightCauchyGreenTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<1u, TensorNumType<T>>>::type
    computeLeftCauchyGreenTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<2u, TensorNumType<T>>>::type
    computeLeftCauchyGreenTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<3u, TensorNumType<T>>>::type
    computeLeftCauchyGreenTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<1u, TensorNumType<T>>>::type
    computeGreenLagrangeTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<2u, TensorNumType<T>>>::type
    computeGreenLagrangeTensor(const T&);

    template <class T>
    typename std::enable_if<
        ((tfel::meta::Implements<T, TensorConcept>::cond) &&
         (TensorTraits<T>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T>>::cond)),
        stensor<3u, TensorNumType<T>>>::type
    computeGreenLagrangeTensor(const T&);

    /*!
     * compute the product:
     * \[F.s.F^{T}\]
     * \param[in] p: a symmetric tensor
     * \param[in] F: a tensor
     */
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    pushForward(const T&, const T2&);
    /*!
     * compute the product:
     * \[F.s.F^{T}\]
     * \param[in] p: a symmetric tensor
     * \param[in] F: a tensor
     */
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    pushForward(const T&, const T2&);
    /*!
     * compute the product:
     * \[F.s.F^{T}\]
     * \param[in] p: a symmetric tensor
     * \param[in] F: a tensor
     */
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    pushForward(const T&, const T2&);
    /*!
     * compute the product:
     * \[F.s.F^{T}\]
     * \param[in] p: a symmetric tensor
     * \param[in] F: a tensor
     * \note the same as pushForward
     */
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    push_forward(const T&, const T2&);
    /*!
     * compute the product:
     * \[F.s.F^{T}\]
     * \param[in] p: a symmetric tensor
     * \param[in] F: a tensor
     * \note the same as pushForward
     */
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    push_forward(const T&, const T2&);
    /*!
     * compute the product:
     * \[F.s.F^{T}\]
     * \param[in] p: a symmetric tensor
     * \param[in] F: a tensor
     * \note the same as pushForward
     */
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    push_forward(const T&, const T2&);

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T&, const T2&);

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T&, const T2&);

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T&, const T2&);

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&, const T2&);

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&, const T2&);

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, TensorConcept>::cond) &&
         (TensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<TensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&, const T2&);

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u,
        typename ComputeUnaryResult<TensorNumType<TensorType>,
                                    Power<3>>::Result>::type
    det(const TensorType&);

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u,
        typename ComputeUnaryResult<TensorNumType<TensorType>,
                                    Power<3>>::Result>::type
    det(const TensorType&);

    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u,
        typename ComputeUnaryResult<TensorNumType<TensorType>,
                                    Power<3>>::Result>::type
    det(const TensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] : determinant derivative
     * \param[in]  : argument
     */
    template <typename TensorResultType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorResultType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u &&
            TensorTraits<TensorResultType>::dime == 1u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<TensorNumType<TensorType>,
                                            Power<2>>::Result,
                TensorNumType<TensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(TensorResultType&, const TensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] : determinant derivative
     * \param[in]  : argument
     */
    template <typename TensorResultType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorResultType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u &&
            TensorTraits<TensorResultType>::dime == 2u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<TensorNumType<TensorType>,
                                            Power<2>>::Result,
                TensorNumType<TensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(TensorResultType&, const TensorType&);
    /*!
     * \brief compute the derivative of the determinant with respect
     * to its argument.
     * \param[out] : determinant derivative
     * \param[in]  : argument
     */
    template <typename TensorResultType, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorResultType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u &&
            TensorTraits<TensorResultType>::dime == 3u &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<TensorNumType<TensorType>,
                                            Power<2>>::Result,
                TensorNumType<TensorResultType>>::cond,
        void>::type
    computeDeterminantDerivative(TensorResultType&, const TensorType&);
    /*!
     * \brief provide the polar decomposition of a tensor
     * \param[out] rotation
     * \param[out] stretch
     * \param[in]  F
     */
    template <typename TensorType, typename StensorType, typename TensorType2>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType2, TensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            std::is_same<StensorNumType<StensorType>,
                         TensorNumType<TensorType2>>::value &&
            std::is_same<
                tfel::typetraits::base_type<TensorNumType<TensorType2>>,
                TensorNumType<TensorType>>::value &&
            (TensorTraits<TensorType>::dime ==
             TensorTraits<TensorType2>::dime) &&
            (TensorTraits<TensorType>::dime ==
             StensorTraits<StensorType>::dime) &&
            (TensorTraits<TensorType>::dime == 1u),
        void>::type
    polar_decomposition(TensorType&, StensorType&, const TensorType2&);

    /*!
     * \brief provide the polar decomposition of a tensor
     * \param[out] rotation
     * \param[out] stretch
     * \param[in]  F
     */
    template <typename TensorType, typename StensorType, typename TensorType2>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            tfel::meta::Implements<TensorType2, TensorConcept>::cond &&
            tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            std::is_same<StensorNumType<StensorType>,
                         TensorNumType<TensorType2>>::value &&
            std::is_same<
                tfel::typetraits::base_type<TensorNumType<TensorType2>>,
                TensorNumType<TensorType>>::value &&
            (TensorTraits<TensorType>::dime ==
             TensorTraits<TensorType2>::dime) &&
            (TensorTraits<TensorType>::dime ==
             StensorTraits<StensorType>::dime) &&
            ((TensorTraits<TensorType>::dime == 2u) ||
             (TensorTraits<TensorType>::dime == 3u)),
        void>::type
    polar_decomposition(TensorType&, StensorType&, const TensorType2&);

    // template<typename TensorType>
    // TFEL_MATH_INLINE auto
    // matrix_view(TensorType&& t)
    // -> typename std::enable_if<
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
    // -> typename std::enable_if<
    //   tfel::meta::Implements<typename
    //   std::decay<TensorType>::type,TensorConcept>::cond &&
    //   (TensorTraits<typename std::decay<TensorType>::type>::dime==1u),
    //   Expr<tensor<1u,TensorNumType<typename std::decay<decltype(t)>::type>>,
    // 	   TensorTransposeExpr1D<decltype(t)>>>::type;

    // /*!
    //  * \return a transposed view of  the tensor
    //  */
    // template<typename TensorType>
    // TFEL_MATH_INLINE auto
    // transpose(TensorType&& t)
    // -> typename std::enable_if<
    //   tfel::meta::Implements<typename
    //   std::decay<TensorType>::type,TensorConcept>::cond &&
    //   (TensorTraits<typename std::decay<TensorType>::type>::dime==2u),
    //   Expr<tensor<2u,TensorNumType<typename std::decay<decltype(t)>::type>>,
    // 	   TensorTransposeExpr2D<decltype(t)>>>::type;

    // /*!
    //  * \return a transposed view of  the tensor
    //  */
    // template<typename TensorType>
    // TFEL_MATH_INLINE auto
    // transpose(TensorType&& t)
    // -> typename std::enable_if<
    //   tfel::meta::Implements<typename
    //   std::decay<TensorType>::type,TensorConcept>::cond &&
    //   (TensorTraits<typename std::decay<TensorType>::type>::dime==3u),
    //   Expr<tensor<3u,TensorNumType<typename std::decay<decltype(t)>::type>>,
    // 	   TensorTransposeExpr3D<decltype(t)>>>::type;

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Tensor/TensorConcept.ixx"

#endif /* LIB_TFEL_MATH_TENSORCONCEPT_HXX */
