/*!
 * \file  include/TFEL/Math/ST2toT2/TensorST2toT2ProductExpr.hxx
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

#ifndef LIB_TFEL_MATH_TENSORST2TOT2PRODUCTEXPR_HXX
#define LIB_TFEL_MATH_TENSORST2TOT2PRODUCTEXPR_HXX

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel {

  namespace math {

    //! Empty structure used for partial specialisation of the
    //! Expr class
    template <unsigned short N>
    struct TensorST2toT2ProductExpr {
    };  // end of struct TensorST2toT2ProductExpr

    /*!
     * Partial specialisation
     */
    template <typename StensorResultType>
    struct Expr<StensorResultType, TensorST2toT2ProductExpr<1u>>
        : public StensorConcept<
              Expr<StensorResultType, TensorST2toT2ProductExpr<1u>>>,
          public fsarray<
              StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value,
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
      template <typename TensorType, typename ST2toT2Type>
      TFEL_MATH_INLINE Expr(const TensorType& a, const ST2toT2Type& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2Type>::dime == 1u));
        //! a simple check
        TFEL_STATIC_ASSERT((TensorTraits<TensorType>::dime == 1u));
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
    };  // end of struct Expr<StensorResultType,T2STensorST2toT2ProductExp>

    /*!
     * Partial specialisation
     */
    template <typename StensorResultType>
    struct Expr<StensorResultType, TensorST2toT2ProductExpr<2u>>
        : public StensorConcept<
              Expr<StensorResultType, TensorST2toT2ProductExpr<2u>>>,
          public fsarray<
              StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value,
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
      template <typename TensorType, typename ST2toT2Type>
      TFEL_MATH_INLINE Expr(const TensorType& a, const ST2toT2Type& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2Type>::dime == 2u));
        //! a simple check
        TFEL_STATIC_ASSERT((TensorTraits<TensorType>::dime == 2u));
        this->v[0] = b(4, 0) * a(4) + b(3, 0) * a(3) + b(2, 0) * a(2) +
                     b(1, 0) * a(1) + b(0, 0) * a(0);
        this->v[1] = b(4, 1) * a(4) + b(3, 1) * a(3) + b(2, 1) * a(2) +
                     b(1, 1) * a(1) + b(0, 1) * a(0);
        this->v[2] = b(4, 2) * a(4) + b(3, 2) * a(3) + b(2, 2) * a(2) +
                     b(1, 2) * a(1) + b(0, 2) * a(0);
        this->v[3] = b(4, 3) * a(4) + b(3, 3) * a(3) + b(2, 3) * a(2) +
                     b(1, 3) * a(1) + b(0, 3) * a(0);
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
    };  // end of struct Expr<StensorResultType,T2STensorST2toT2ProductExp>

    /*!
     * Partial specialisation
     */
    template <typename StensorResultType>
    struct Expr<StensorResultType, TensorST2toT2ProductExpr<3u>>
        : public StensorConcept<
              Expr<StensorResultType, TensorST2toT2ProductExpr<3u>>>,
          public fsarray<
              StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value,
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
      template <typename TensorType, typename ST2toT2Type>
      TFEL_MATH_INLINE Expr(const TensorType& a, const ST2toT2Type& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2Type>::dime == 3u));
        //! a simple check
        TFEL_STATIC_ASSERT((TensorTraits<TensorType>::dime == 3u));
        this->v[0] = b(8, 0) * a(8) + b(7, 0) * a(7) + b(6, 0) * a(6) +
                     b(5, 0) * a(5) + b(4, 0) * a(4) + b(3, 0) * a(3) +
                     b(2, 0) * a(2) + b(1, 0) * a(1) + b(0, 0) * a(0);
        this->v[1] = b(8, 1) * a(8) + b(7, 1) * a(7) + b(6, 1) * a(6) +
                     b(5, 1) * a(5) + b(4, 1) * a(4) + b(3, 1) * a(3) +
                     b(2, 1) * a(2) + b(1, 1) * a(1) + b(0, 1) * a(0);
        this->v[2] = b(8, 2) * a(8) + b(7, 2) * a(7) + b(6, 2) * a(6) +
                     b(5, 2) * a(5) + b(4, 2) * a(4) + b(3, 2) * a(3) +
                     b(2, 2) * a(2) + b(1, 2) * a(1) + b(0, 2) * a(0);
        this->v[3] = b(8, 3) * a(8) + b(7, 3) * a(7) + b(6, 3) * a(6) +
                     b(5, 3) * a(5) + b(4, 3) * a(4) + b(3, 3) * a(3) +
                     b(2, 3) * a(2) + b(1, 3) * a(1) + b(0, 3) * a(0);
        this->v[4] = b(8, 4) * a(8) + b(7, 4) * a(7) + b(6, 4) * a(6) +
                     b(5, 4) * a(5) + b(4, 4) * a(4) + b(3, 4) * a(3) +
                     b(2, 4) * a(2) + b(1, 4) * a(1) + b(0, 4) * a(0);
        this->v[5] = b(8, 5) * a(8) + b(7, 5) * a(7) + b(6, 5) * a(6) +
                     b(5, 5) * a(5) + b(4, 5) * a(4) + b(3, 5) * a(3) +
                     b(2, 5) * a(2) + b(1, 5) * a(1) + b(0, 5) * a(0);
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
    };  // end of struct Expr<StensorResultType,T2STensorST2toT2ProductExp>

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSORST2TOT2PRODUCTEXPR_HXX */
