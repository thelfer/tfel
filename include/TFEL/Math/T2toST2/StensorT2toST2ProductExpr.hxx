/*!
 * \file  include/TFEL/Math/T2toST2/StensorT2toST2ProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORT2TOST2PRODUCTEXPR_H_
#define LIB_TFEL_MATH_STENSORT2TOST2PRODUCTEXPR_H_

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel {

  namespace math {

    //! Empty structure used for partial specialisation of the
    //! Expr class
    template <unsigned short N>
    struct StensorT2toST2ProductExpr {
    };  // end of struct StensorT2toST2ProductExpr

    /*!
     * Partial specialisation
     */
    template <typename TensorResultType>
    struct Expr<TensorResultType, StensorT2toST2ProductExpr<1u>>
        : public TensorConcept<
              Expr<TensorResultType, StensorT2toST2ProductExpr<1u>>>,
          public fsarray<
              TensorDimeToSize<TensorTraits<TensorResultType>::dime>::value,
              typename TensorTraits<TensorResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename TensorTraits<TensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((TensorTraits<TensorResultType>::dime == 1u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename StensorType, typename T2toST2Type>
      TFEL_MATH_INLINE Expr(const StensorType& a, const T2toST2Type& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime == 1u));
        //! a simple check
        TFEL_STATIC_ASSERT((StensorTraits<StensorType>::dime == 1u));
        this->v[0] = b(0, 0) * a(0) + b(1, 0) * a(1) + b(2, 0) * a(2);
        this->v[1] = b(0, 1) * a(0) + b(1, 1) * a(1) + b(2, 1) * a(2);
        this->v[2] = b(0, 2) * a(0) + b(1, 2) * a(1) + b(2, 2) * a(2);
      }  // end of Expr
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
    };  // end of struct Expr<TensorResultType,T2SStensorT2toST2ProductExp>

    /*!
     * Partial specialisation
     */
    template <typename TensorResultType>
    struct Expr<TensorResultType, StensorT2toST2ProductExpr<2u>>
        : public TensorConcept<
              Expr<TensorResultType, StensorT2toST2ProductExpr<2u>>>,
          public fsarray<
              TensorDimeToSize<TensorTraits<TensorResultType>::dime>::value,
              typename TensorTraits<TensorResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename TensorTraits<TensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((TensorTraits<TensorResultType>::dime == 2u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename StensorType, typename T2toST2Type>
      TFEL_MATH_INLINE Expr(const StensorType& a, const T2toST2Type& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime == 2u));
        //! a simple check
        TFEL_STATIC_ASSERT((StensorTraits<StensorType>::dime == 2u));
        this->v[0] =
            b(3, 0) * a[3] + b(2, 0) * a[2] + b(1, 0) * a[1] + b(0, 0) * a[0];
        this->v[1] =
            b(3, 1) * a[3] + b(2, 1) * a[2] + b(1, 1) * a[1] + b(0, 1) * a[0];
        this->v[2] =
            b(3, 2) * a[3] + b(2, 2) * a[2] + b(1, 2) * a[1] + b(0, 2) * a[0];
        this->v[3] =
            b(3, 3) * a[3] + b(2, 3) * a[2] + b(1, 3) * a[1] + b(0, 3) * a[0];
        this->v[4] =
            b(3, 4) * a[3] + b(2, 4) * a[2] + b(1, 4) * a[1] + b(0, 4) * a[0];
      }  // end of Expr
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
    };  // end of struct Expr<TensorResultType,T2SStensorT2toST2ProductExp>

    /*!
     * Partial specialisation
     */
    template <typename TensorResultType>
    struct Expr<TensorResultType, StensorT2toST2ProductExpr<3u>>
        : public TensorConcept<
              Expr<TensorResultType, StensorT2toST2ProductExpr<3u>>>,
          public fsarray<
              TensorDimeToSize<TensorTraits<TensorResultType>::dime>::value,
              typename TensorTraits<TensorResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename TensorTraits<TensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((TensorTraits<TensorResultType>::dime == 3u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename StensorType, typename T2toST2Type>
      TFEL_MATH_INLINE Expr(const StensorType& a, const T2toST2Type& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime == 3u));
        //! a simple check
        TFEL_STATIC_ASSERT((StensorTraits<StensorType>::dime == 3u));
        this->v[0] = b(5, 0) * a[5] + b(4, 0) * a[4] + b(3, 0) * a[3] +
                     b(2, 0) * a[2] + b(1, 0) * a[1] + b(0, 0) * a[0];
        this->v[1] = b(5, 1) * a[5] + b(4, 1) * a[4] + b(3, 1) * a[3] +
                     b(2, 1) * a[2] + b(1, 1) * a[1] + b(0, 1) * a[0];
        this->v[2] = b(5, 2) * a[5] + b(4, 2) * a[4] + b(3, 2) * a[3] +
                     b(2, 2) * a[2] + b(1, 2) * a[1] + b(0, 2) * a[0];
        this->v[3] = b(5, 3) * a[5] + b(4, 3) * a[4] + b(3, 3) * a[3] +
                     b(2, 3) * a[2] + b(1, 3) * a[1] + b(0, 3) * a[0];
        this->v[4] = b(5, 4) * a[5] + b(4, 4) * a[4] + b(3, 4) * a[3] +
                     b(2, 4) * a[2] + b(1, 4) * a[1] + b(0, 4) * a[0];
        this->v[5] = b(5, 5) * a[5] + b(4, 5) * a[4] + b(3, 5) * a[3] +
                     b(2, 5) * a[2] + b(1, 5) * a[1] + b(0, 5) * a[0];
        this->v[6] = b(5, 6) * a[5] + b(4, 6) * a[4] + b(3, 6) * a[3] +
                     b(2, 6) * a[2] + b(1, 6) * a[1] + b(0, 6) * a[0];
        this->v[7] = b(5, 7) * a[5] + b(4, 7) * a[4] + b(3, 7) * a[3] +
                     b(2, 7) * a[2] + b(1, 7) * a[1] + b(0, 7) * a[0];
        this->v[8] = b(5, 8) * a[5] + b(4, 8) * a[4] + b(3, 8) * a[3] +
                     b(2, 8) * a[2] + b(1, 8) * a[1] + b(0, 8) * a[0];
      }  // end of Expr
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
    };  // end of struct Expr<TensorResultType,T2SStensorT2toST2ProductExp>

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STENSORT2TOST2PRODUCTEXPR_H_ */
