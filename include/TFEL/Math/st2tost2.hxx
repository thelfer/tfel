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
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx"
#include "TFEL/Math/ST2toST2/StensorSquareDerivative.hxx"

namespace tfel {

  namespace math {

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
     * \brief partial specialisation of the `ST2toST2Traits` class for
     * `st2tost2`.
     * \tparam N: space dimension
     * \tparam T: numeric type
     */
    template <unsigned short N, typename T>
    struct ST2toST2Traits<st2tost2<N, T>> {
      //! a simple alias
      typedef T NumType;
      //! a simple alias
      typedef unsigned short IndexType;
      //! a simple alias
      static constexpr unsigned short dime = N;
    };

    /*!
     * \brief a base for stensor or classes acting like stensor.
     * \param Child : child class
     * \param N     : spatial dimension
     * \param T     : numerical type
     */
    template <typename Child>
    struct st2tost2_base {
      /*!
       * Assignement operator
       */
      template <typename ST2toST2Type>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<ST2toST2Type,
                                 tfel::math::ST2toST2Concept>::cond &&
              ST2toST2Traits<Child>::dime ==
                  ST2toST2Traits<ST2toST2Type>::dime &&
              tfel::typetraits::IsAssignableTo<ST2toST2NumType<ST2toST2Type>,
                                               ST2toST2NumType<Child>>::cond,
          Child&>::type
      operator=(const ST2toST2Type&);
      //! Assignement operator
      template <typename ST2toST2Type>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<ST2toST2Type,
                                 tfel::math::ST2toST2Concept>::cond &&
              ST2toST2Traits<Child>::dime ==
                  ST2toST2Traits<ST2toST2Type>::dime &&
              tfel::typetraits::IsAssignableTo<ST2toST2NumType<ST2toST2Type>,
                                               ST2toST2NumType<Child>>::cond,
          Child&>::type
      operator+=(const ST2toST2Type&);
      //! Assignement operator
      template <typename ST2toST2Type>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<ST2toST2Type,
                                 tfel::math::ST2toST2Concept>::cond &&
              ST2toST2Traits<Child>::dime ==
                  ST2toST2Traits<ST2toST2Type>::dime &&
              tfel::typetraits::IsAssignableTo<ST2toST2NumType<ST2toST2Type>,
                                               ST2toST2NumType<Child>>::cond,
          Child&>::type
      operator-=(const ST2toST2Type&);
      /*!
       * operator*=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<ST2toST2NumType<Child>, T2, OpMult>::type,
                  ST2toST2NumType<Child>>::value,
          Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          tfel::typetraits::IsScalar<T2>::cond &&
              std::is_same<
                  typename ResultType<ST2toST2NumType<Child>, T2, OpDiv>::type,
                  ST2toST2NumType<Child>>::value,
          Child&>::type
      operator/=(const T2);
    };  // end of struct st2tost2_base

    template <unsigned short N, typename T>
    struct st2tost2 : public ST2toST2Concept<st2tost2<N, T>>,
                      public fsarray<StensorDimeToSize<N>::value *
                                         StensorDimeToSize<N>::value,
                                     T>,
                      public st2tost2_base<st2tost2<N, T>> {
      /*!
       * \param[in] s : tensor squared
       * \return the derivative of the square of a symmetric tensor
       */
      template <typename StensorType>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<StensorType,
                                 tfel::math::StensorConcept>::cond &&
              StensorTraits<StensorType>::dime == N &&
              tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                               T>::cond,
          Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>>::type
      dsquare(const StensorType&);
      /*!
       * \param[in] s : tensor squared
       * \param[in] C : derivative of s
       * \return the derivative of the square of a symmetric tensor
       */
      template <typename StensorType, typename ST2toST2Type>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<StensorType, StensorConcept>::cond &&
              tfel::meta::Implements<ST2toST2Type,
                                     tfel::math::ST2toST2Concept>::cond &&
              StensorTraits<StensorType>::dime == N &&
              ST2toST2Traits<ST2toST2Type>::dime == N &&
              tfel::typetraits::IsAssignableTo<
                  typename ComputeBinaryResult<StensorNumType<StensorType>,
                                               ST2toST2NumType<ST2toST2Type>,
                                               OpMult>::Result,
                  T>::cond,
          Expr<st2tost2<N, T>, StensorSquareDerivativeExpr<N>>>::type
      dsquare(const StensorType&, const ST2toST2Type&);
      /*!
       * convert a T2toST2 to a ST2toST2
       * \param[in] src : T2toST2 to be converted
       */
      template <typename T2toST2Type>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<T2toST2Type,
                                 tfel::math::T2toST2Concept>::cond &&
              T2toST2Traits<T2toST2Type>::dime == N &&
              tfel::typetraits::IsAssignableTo<
                  typename T2toST2Traits<T2toST2Type>::NumType,
                  T>::cond,
          Expr<st2tost2<N, T>, ConvertT2toST2ToST2toST2Expr<N>>>::type
      convert(const T2toST2Type&);
      /*!
       * build the equivalent st2tost2 from a rotation matrix
       * \param[in] r : rotation matrix
       */
      static tfel::math::st2tost2<N, tfel::typetraits::base_type<T>>
      fromRotationMatrix(
          const tmatrix<3u, 3u, typename tfel::typetraits::base_type<T>>&);
      /*!
       * \brief compute the derivative of the symmetric tensor product:
       * \f[
       * \frac{d}{ds1}\left(s1*s+s*s1\right)
       * \f]
       * \param[in] s: second tensor of the product
       */
      template <typename StensorType>
      static TFEL_MATH_INLINE typename std::enable_if<
          tfel::meta::Implements<StensorType,
                                 tfel::math::StensorConcept>::cond &&
              StensorTraits<StensorType>::dime == N &&
              tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                               T>::cond,
          tfel::math::st2tost2<N, T>>::type
      stpd(const StensorType&);
      /*!
       * This is a StensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! \brief default Constructor
      explicit constexpr st2tost2() = default;
      /*!
       * \brief default constructor
       * \param[in] init : value used to initialise the components of the
       * st2tost2
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr explicit st2tost2(const T2&);
      /*!
       * \brief default constructor
       * \param[in] init : values used to initialise the components of the
       * st2tost2
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr st2tost2(const std::initializer_list<T2>&);
      /*!
       * \brief Default Constructor.
       * \param const typename tfel::typetraits::BaseType<T>::type* const,
       *  pointer to a tabular used to initialise the components of
       *  the st2tost2. This tabular is left unchanged.
       */
      TFEL_MATH_INLINE
      explicit st2tost2(const tfel::typetraits::base_type<T>* const);
      //! copy Constructor
      constexpr st2tost2(const st2tost2<N, T>&) = default;
      // Copy Constructor
      template <typename T2, typename Op>
      TFEL_MATH_INLINE st2tost2(const Expr<st2tost2<N, T2>, Op>&);
      //! assignement operator
      st2tost2& operator=(const st2tost2&) = default;
      /*!
       * Import values
       */
      template <typename T2>
      TFEL_MATH_INLINE2 typename std::enable_if<
          tfel::typetraits::IsSafelyReinterpretCastableTo<
              T2,
              tfel::typetraits::base_type<T>>::cond,
          void>::type
      import(const T2* const);

      /*!
       * Assignement operator
       */
      using st2tost2_base<st2tost2>::operator=;

      //! access operator
      TFEL_MATH_INLINE const T& operator()(const unsigned short,
                                           const unsigned short) const;
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

      static TFEL_MATH_INLINE constexpr st2tost2 Id();
      static TFEL_MATH_INLINE constexpr st2tost2 IxI();
      static TFEL_MATH_INLINE constexpr st2tost2 K();
      static TFEL_MATH_INLINE constexpr st2tost2 M();
      static TFEL_MATH_INLINE constexpr st2tost2 J();

     private:
      /*!
       * A simple check
       */
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));

      enum ParticularSt2toSt2 {
        ST2TOST2_IDENTITY,
        ST2TOST2_J,
        ST2TOST2_K,
        ST2TOST2_IxI
      };  // end of enum ParticularSt2toSt2

      st2tost2(const ParticularSt2toSt2);

    };  // end of struct st2tost2

    /*!
     * \return change the basis of a st2tost2
     * \param[in] s : st2tost2
     * \param[in] r : rotation matrix
     */
    template <typename ST2toST2Type>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond,
        st2tost2<ST2toST2Traits<ST2toST2Type>::dime,
                 ST2toST2NumType<ST2toST2Type>>>::type
    change_basis(
        const ST2toST2Type&,
        const tfel::math::tmatrix<
            3u,
            3u,
            tfel::typetraits::base_type<ST2toST2NumType<ST2toST2Type>>>&);
    /*!
     * \return the invert of a st2tost2
     * \param[in] s : st2tost2 to be inverted
     */
    template <typename ST2toST2Type>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond,
        st2tost2<
            ST2toST2Traits<ST2toST2Type>::dime,
            typename ComputeBinaryResult<typename tfel::typetraits::base_type<
                                             ST2toST2NumType<ST2toST2Type>>,
                                         ST2toST2NumType<ST2toST2Type>,
                                         OpDiv>::Result>>::type
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
    typename std::enable_if<
        tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            ST2toST2Traits<ST2toST2Type>::dime ==
                TensorTraits<TensorType>::dime,
        st2tost2<ST2toST2Traits<ST2toST2Type>::dime,
                 typename ComputeBinaryResult<ST2toST2NumType<ST2toST2Type>,
                                              TensorNumType<TensorType>,
                                              OpMult>::Result>>::type
    push_forward(const ST2toST2Type&, const TensorType&);

    template <typename ST2toST2Type, typename TensorType>
    typename std::enable_if<
        tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond &&
            tfel::meta::Implements<TensorType, TensorConcept>::cond &&
            ST2toST2Traits<ST2toST2Type>::dime ==
                TensorTraits<TensorType>::dime,
        st2tost2<ST2toST2Traits<ST2toST2Type>::dime,
                 typename ComputeBinaryResult<ST2toST2NumType<ST2toST2Type>,
                                              TensorNumType<TensorType>,
                                              OpMult>::Result>>::type
    pull_back(const ST2toST2Type&, const TensorType&);
    /*!
     * \brief compute the second derivative of the determinant of a
     * symmetric tensor
     * \param[in] s: tensor
     */
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 1u &&
            tfel::typetraits::IsScalar<StensorNumType<StensorType>>::cond,
        st2tost2<1u, StensorNumType<StensorType>>>::type
    computeDeterminantSecondDerivative(const StensorType&);
    /*!
     * \brief compute the second derivative of the determinant of a
     * symmetric tensor
     * \param[in] s: tensor
     */
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 2u &&
            tfel::typetraits::IsScalar<StensorNumType<StensorType>>::cond,
        st2tost2<2u, StensorNumType<StensorType>>>::type
    computeDeterminantSecondDerivative(const StensorType&);
    /*!
     * \brief compute the second derivative of the determinant of a
     * symmetric tensor
     * \param[in] s: tensor
     */
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 3u &&
            tfel::typetraits::IsScalar<StensorNumType<StensorType>>::cond,
        st2tost2<3u, StensorNumType<StensorType>>>::type
    computeDeterminantSecondDerivative(const StensorType&);

  }  // end of namespace math

  namespace typetraits {

    template <unsigned short N, typename T2, typename T>
    struct IsAssignableTo<tfel::math::st2tost2<N, T2>,
                          tfel::math::st2tost2<N, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsAssignableTo<T2, T>::cond;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#include "TFEL/Math/ST2toST2/st2tost2.ixx"
#include "TFEL/Math/ST2toST2/st2tost2ResultType.hxx"

#endif /* LIB_TFEL_MATH_ST2TOST2_HXX */
