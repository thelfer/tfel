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
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Forward/t2tot2.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include "TFEL/Math/T2toT2/T2toT2ConceptOperations.hxx"

namespace tfel {

  namespace math {

    // forward declaration
    template <unsigned short N>
    struct TensorProductLeftDerivativeExpr;

    // forward declaration
    template <unsigned short N>
    struct TensorProductRightDerivativeExpr;

    /*
     * Partial specialisation for t2tot2
     */
    template <unsigned short N, typename T>
    struct T2toT2Traits<t2tot2<N, T>> {
      typedef T NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = N;
    };

    /*!
     * \brief a base for stensor or classes acting like stensor.
     * \param Child : child class
     */
    template <typename Child>
    struct t2tot2_base {
      /*!
       * Assignement operator
       */
      template <typename T2toT2Type>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<T2toT2Type, tfel::math::T2toT2Concept>::cond &&
              T2toT2Traits<Child>::dime == T2toT2Traits<T2toT2Type>::dime &&
              tfel::typetraits::IsAssignableTo<
                  typename T2toT2Traits<T2toT2Type>::NumType,
                  typename T2toT2Traits<Child>::NumType>::cond,
          Child&>::type
      operator=(const T2toT2Type&);
      //! Assignement operator
      template <typename T2toT2Type>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<T2toT2Type, tfel::math::T2toT2Concept>::cond &&
              T2toT2Traits<Child>::dime == T2toT2Traits<T2toT2Type>::dime &&
              tfel::typetraits::IsAssignableTo<
                  typename T2toT2Traits<T2toT2Type>::NumType,
                  typename T2toT2Traits<Child>::NumType>::cond,
          Child&>::type
      operator+=(const T2toT2Type&);
      //! Assignement operator
      template <typename T2toT2Type>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<T2toT2Type, tfel::math::T2toT2Concept>::cond &&
              T2toT2Traits<Child>::dime == T2toT2Traits<T2toT2Type>::dime &&
              tfel::typetraits::IsAssignableTo<
                  typename T2toT2Traits<T2toT2Type>::NumType,
                  typename T2toT2Traits<Child>::NumType>::cond,
          Child&>::type
      operator-=(const T2toT2Type&);
      /*!
       * operator*=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<typename T2toT2Traits<Child>::NumType,
                                      T2,
                                      OpMult>::type,
                  typename T2toT2Traits<Child>::NumType>::value,
          Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<typename T2toT2Traits<Child>::NumType,
                                      T2,
                                      OpDiv>::type,
                  typename T2toT2Traits<Child>::NumType>::value,
          Child&>::type
      operator/=(const T2);
    };  // end of struct t2tot2_base

    template <unsigned short N, typename T>
    struct t2tot2 : public T2toT2Concept<t2tot2<N, T>>,
                    public fsarray<TensorDimeToSize<N>::value *
                                       TensorDimeToSize<N>::value,
                                   T>,
                    public t2tot2_base<t2tot2<N, T>> {
      /*!
       * \return a t2tot2 which acts on a tensor as a rotation of the coordinate
       * system \param[in] r : rotation matrix
       */
      static tfel::math::t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
      fromRotationMatrix(const rotation_matrix<T>&);
      /*!
       * \param[in] B : second tensor of the product
       * \return the left part of the derivative of a tensor product
       */
      template <typename TensorType>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<TensorType, tfel::math::TensorConcept>::cond &&
              TensorTraits<TensorType>::dime == N &&
              tfel::typetraits::IsAssignableTo<
                  typename TensorTraits<TensorType>::NumType,
                  T>::cond,
          Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>::type
      tpld(const TensorType&);
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       * \return the left part of the derivative of a tensor product
       */
      template <typename TensorType, typename T2toT2Type>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<TensorType, tfel::math::TensorConcept>::cond &&
              tfel::meta::Implements<T2toT2Type,
                                     tfel::math::T2toT2Concept>::cond &&
              TensorTraits<TensorType>::dime == N &&
              T2toT2Traits<T2toT2Type>::dime == N &&
              tfel::typetraits::IsAssignableTo<
                  typename ComputeBinaryResult<
                      typename TensorTraits<TensorType>::NumType,
                      typename T2toT2Traits<T2toT2Type>::NumType,
                      OpMult>::Result,
                  T>::cond,
          Expr<t2tot2<N, T>, TensorProductLeftDerivativeExpr<N>>>::type
      tpld(const TensorType&, const T2toT2Type&);
      /*!
       * \param[in] A : first tensor of the product
       * \return the right part of the derivative of a tensor product
       */
      template <typename TensorType>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<TensorType, tfel::math::TensorConcept>::cond &&
              TensorTraits<TensorType>::dime == N &&
              tfel::typetraits::IsAssignableTo<
                  typename TensorTraits<TensorType>::NumType,
                  T>::cond,
          Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>::type
      tprd(const TensorType&);
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the first tensor
       * \return the right part of the derivative of a tensor product
       */
      template <typename TensorType, typename T2toT2Type>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<TensorType, tfel::math::TensorConcept>::cond &&
              tfel::meta::Implements<T2toT2Type,
                                     tfel::math::T2toT2Concept>::cond &&
              TensorTraits<TensorType>::dime == N &&
              T2toT2Traits<T2toT2Type>::dime == N &&
              tfel::typetraits::IsAssignableTo<
                  typename ComputeBinaryResult<
                      typename TensorTraits<TensorType>::NumType,
                      typename T2toT2Traits<T2toT2Type>::NumType,
                      OpMult>::Result,
                  T>::cond,
          Expr<t2tot2<N, T>, TensorProductRightDerivativeExpr<N>>>::type
      tprd(const TensorType&, const T2toT2Type&);
      /*!
       * \return the derivative of the transpose of a tensor with respect of
       * this tensor
       */
      static TFEL_MATH_INLINE TFEL_CONSTEXPR
          tfel::math::t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
          transpose_derivative();
      //! \return the identity
      static TFEL_MATH_INLINE TFEL_CONSTEXPR
          tfel::math::t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
          Id();
      //! \return Id2^Id2, where Id2 is the identity tensor
      static TFEL_MATH_INLINE TFEL_CONSTEXPR
          tfel::math::t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
          IxI();
      /*!
       * \return Id4-Id2^Id2/3, where Id4 is the identity of t2tot2 and
       * Id2 is the identity tensor
       */
      static TFEL_MATH_INLINE TFEL_CONSTEXPR
          tfel::math::t2tot2<N, typename tfel::typetraits::BaseType<T>::type>
          K();
      //! This is a T2toT2 concept requirement.
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \brief Default Constructor
       */
      TFEL_MATH_INLINE explicit constexpr t2tot2();
      /*!
       * \brief Default Constructor
       * \param T, value used to initialise the components of the t2tot2
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE explicit constexpr t2tot2(const T2&);
      /*!
       * \brief Copy Constructor
       */
      TFEL_MATH_INLINE constexpr t2tot2(const t2tot2<N, T>&);
      // Copy Constructor
      template <typename T2, typename Op>
      TFEL_MATH_INLINE t2tot2(const Expr<t2tot2<N, T2>, Op>&);
      /*!
       * \brief default constructor
       * \param[in] v : values
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr t2tot2(const std::initializer_list<T2>&);
      //! assignement operator
      TFEL_MATH_INLINE t2tot2& operator=(const t2tot2&);
      /*!
       * Import values
       */
      template <typename T2>
      TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsSafelyReinterpretCastableTo<
              T2,
              typename tfel::typetraits::BaseType<T>::type>::cond,
          void>::type
      import(const T2* const);

      /*!
       * Assignement operator
       */
      using t2tot2_base<t2tot2>::operator=;

      //! access operator
      TFEL_MATH_INLINE constexpr const T& operator()(
          const unsigned short, const unsigned short) const;
      //! access operator
      TFEL_MATH_INLINE T& operator()(const unsigned short,
                                     const unsigned short);
      /*!
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const;

      template <typename InputIterator>
      TFEL_MATH_INLINE2 void copy(const InputIterator src);

    };  // end of struct t2tot2

    /*!
     * \return change the basis of a t2tot2
     * \param[in] s : t2tot2
     * \param[in] r : rotation matrix
     */
    template <typename T2toT2Type>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<T2toT2Type, T2toT2Concept>::cond,
        t2tot2<T2toT2Traits<T2toT2Type>::dime,
               typename T2toT2Traits<T2toT2Type>::NumType>>::type
    change_basis(const T2toT2Type&,
                 const rotation_matrix<T2toT2NumType<T2toT2Type>>&);

    /*!
     * \return compute the derivative of the velocity gradient
     * \param[in] F : deformation gradient
     */
    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        t2tot2<TensorTraits<TensorType>::dime,
               typename TensorTraits<TensorType>::NumType>>::type
    computeVelocityGradientDerivative(const TensorType&);

    /*!
     * \return compute the derivative of the spin rate
     * \param[in] F : deformation gradient
     */
    template <typename TensorType>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond,
        t2tot2<TensorTraits<TensorType>::dime,
               typename TensorTraits<TensorType>::NumType>>::type
    computeSpinRateDerivative(const TensorType&);
    /*!
     * \brief compute the second derivative of the determinant of a
     * symmetric tensor
     * \param[in] s: tensor
     */
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 1u &&
            tfel::typetraits::IsScalar<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tot2<1u, typename TensorTraits<TensorType>::NumType>>::type
    computeDeterminantSecondDerivative(const TensorType&);
    /*!
     * \brief compute the second derivative of the determinant of a
     * symmetric tensor
     * \param[in] s: tensor
     */
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 2u &&
            tfel::typetraits::IsScalar<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tot2<2u, typename TensorTraits<TensorType>::NumType>>::type
    computeDeterminantSecondDerivative(const TensorType&);
    /*!
     * \brief compute the second derivative of the determinant of a
     * symmetric tensor
     * \param[in] s: tensor
     */
    template <typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            TensorTraits<TensorType>::dime == 3u &&
            tfel::typetraits::IsScalar<
                typename TensorTraits<TensorType>::NumType>::cond,
        t2tot2<3u, typename TensorTraits<TensorType>::NumType>>::type
    computeDeterminantSecondDerivative(const TensorType&);

  }  // end of namespace math

  namespace typetraits {

    template <unsigned short N, typename T2, typename T>
    struct IsAssignableTo<tfel::math::t2tot2<N, T2>, tfel::math::t2tot2<N, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsAssignableTo<T2, T>::cond;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#include "TFEL/Math/T2toT2/t2tot2.ixx"
#include "TFEL/Math/T2toT2/t2tot2ResultType.hxx"

#endif /* LIB_TFEL_MATH_T2TOT2_HXX */
