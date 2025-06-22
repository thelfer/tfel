/*!
 * \file  include/TFEL/Math/T2toST2/T2toST2TensorProductExpr.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2TENSORPRODUCTEXPR_HXX
#define LIB_TFEL_MATH_T2TOST2TENSORPRODUCTEXPR_HXX

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel {

  namespace math {

    //! Empty structure used for partial specialisation of the
    //! Expr class
    template <unsigned short N>
    struct T2toST2TensorProductExpr {
    };  // end of struct T2toST2TensorProductExpr

    /*!
     * Partial specialisation
     */
    template <typename StensorResultType>
    struct Expr<StensorResultType, T2toST2TensorProductExpr<1u>>
        : public StensorConcept<
              Expr<StensorResultType, T2toST2TensorProductExpr<1u>>>,
          public fsarray<3u,
                         typename StensorTraits<StensorResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename StensorTraits<StensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((StensorTraits<StensorResultType>::dime == 1u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename T2toST2Type, typename TensorType>
      TFEL_MATH_INLINE Expr(const T2toST2Type& a, const TensorType& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime == 1u));
        //! a simple check
        TFEL_STATIC_ASSERT((TensorTraits<TensorType>::dime == 1u));
        this->v[0] = a(0, 0) * b(0) + a(0, 1) * b(1) + a(0, 2) * b(2);
        this->v[1] = a(1, 0) * b(0) + a(1, 1) * b(1) + a(1, 2) * b(2);
        this->v[2] = a(2, 0) * b(0) + a(2, 1) * b(1) + a(2, 2) * b(2);
      }  // end of Expr
      //! using operator []
      using fsarray<3u, value_type>::operator[];
      /*!
       * \brief access operator
       * \param[in] i : index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i) const {
        return this->v[i];
      }  // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct Expr<StensorResultType,T2ST2toST2TensorProductExp>

    /*!
     * Partial specialisation
     */
    template <typename StensorResultType>
    struct Expr<StensorResultType, T2toST2TensorProductExpr<2u>>
        : public StensorConcept<
              Expr<StensorResultType, T2toST2TensorProductExpr<2u>>>,
          public fsarray<4u,
                         typename StensorTraits<StensorResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename StensorTraits<StensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((StensorTraits<StensorResultType>::dime == 2u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename T2toST2Type, typename TensorType>
      TFEL_MATH_INLINE Expr(const T2toST2Type& a, const TensorType& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime == 2u));
        //! a simple check
        TFEL_STATIC_ASSERT((TensorTraits<TensorType>::dime == 2u));
        this->v[0] = a(0, 4) * b[4] + a(0, 3) * b[3] + a(0, 2) * b[2] +
                     a(0, 1) * b[1] + a(0, 0) * b[0];
        this->v[1] = a(1, 4) * b[4] + a(1, 3) * b[3] + a(1, 2) * b[2] +
                     a(1, 1) * b[1] + a(1, 0) * b[0];
        this->v[2] = a(2, 4) * b[4] + a(2, 3) * b[3] + a(2, 2) * b[2] +
                     a(2, 1) * b[1] + a(2, 0) * b[0];
        this->v[3] = a(3, 4) * b[4] + a(3, 3) * b[3] + a(3, 2) * b[2] +
                     a(3, 1) * b[1] + a(3, 0) * b[0];
      }  // end of Expr
      //! using operator []
      using fsarray<4u, value_type>::operator[];
      /*!
       * \brief access operator
       * \param[in] i : index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i) const {
        return this->v[i];
      }  // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct Expr<StensorResultType,T2ST2toST2TensorProductExp>

    /*!
     * Partial specialisation
     */
    template <typename StensorResultType>
    struct Expr<StensorResultType, T2toST2TensorProductExpr<3u>>
        : public StensorConcept<
              Expr<StensorResultType, T2toST2TensorProductExpr<3u>>>,
          public fsarray<6u,
                         typename StensorTraits<StensorResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename StensorTraits<StensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((StensorTraits<StensorResultType>::dime == 3u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename T2toST2Type, typename TensorType>
      TFEL_MATH_INLINE Expr(const T2toST2Type& a, const TensorType& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime == 3u));
        //! a simple check
        TFEL_STATIC_ASSERT((TensorTraits<TensorType>::dime == 3u));
        this->v[0] = a(0, 8) * b[8] + a(0, 7) * b[7] + a(0, 6) * b[6] +
                     a(0, 5) * b[5] + a(0, 4) * b[4] + a(0, 3) * b[3] +
                     a(0, 2) * b[2] + a(0, 1) * b[1] + a(0, 0) * b[0];
        this->v[1] = a(1, 8) * b[8] + a(1, 7) * b[7] + a(1, 6) * b[6] +
                     a(1, 5) * b[5] + a(1, 4) * b[4] + a(1, 3) * b[3] +
                     a(1, 2) * b[2] + a(1, 1) * b[1] + a(1, 0) * b[0];
        this->v[2] = a(2, 8) * b[8] + a(2, 7) * b[7] + a(2, 6) * b[6] +
                     a(2, 5) * b[5] + a(2, 4) * b[4] + a(2, 3) * b[3] +
                     a(2, 2) * b[2] + a(2, 1) * b[1] + a(2, 0) * b[0];
        this->v[3] = a(3, 8) * b[8] + a(3, 7) * b[7] + a(3, 6) * b[6] +
                     a(3, 5) * b[5] + a(3, 4) * b[4] + a(3, 3) * b[3] +
                     a(3, 2) * b[2] + a(3, 1) * b[1] + a(3, 0) * b[0];
        this->v[4] = a(4, 8) * b[8] + a(4, 7) * b[7] + a(4, 6) * b[6] +
                     a(4, 5) * b[5] + a(4, 4) * b[4] + a(4, 3) * b[3] +
                     a(4, 2) * b[2] + a(4, 1) * b[1] + a(4, 0) * b[0];
        this->v[5] = a(5, 8) * b[8] + a(5, 7) * b[7] + a(5, 6) * b[6] +
                     a(5, 5) * b[5] + a(5, 4) * b[4] + a(5, 3) * b[3] +
                     a(5, 2) * b[2] + a(5, 1) * b[1] + a(5, 0) * b[0];
      }  // end of Expr
      //! using operator []
      using fsarray<6u, value_type>::operator[];
      /*!
       * \brief access operator
       * \param[in] i : index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i) const {
        return this->v[i];
      }  // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct Expr<StensorResultType,T2ST2toST2TensorProductExp>

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOST2TENSORPRODUCTEXPR_HXX */
