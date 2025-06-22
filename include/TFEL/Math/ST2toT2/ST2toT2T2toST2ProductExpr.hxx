/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2T2toST2ProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \date   16 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2T2TOST2PRODUCTEXPR_H_
#define LIB_TFEL_MATH_ST2TOT2T2TOST2PRODUCTEXPR_H_

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2Concept.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"

namespace tfel {

  namespace math {

    //! Empty structure used for partial specialisation of the
    //! Expr class
    template <unsigned short N>
    struct TFEL_VISIBILITY_LOCAL ST2toT2T2toST2ProductExpr {
    };  // end of struct ST2toT2T2toST2ProductExpr

    /*!
     * Partial specialisation
     */
    template <typename T2toT2ResultType>
    struct TFEL_VISIBILITY_LOCAL
        Expr<T2toT2ResultType, ST2toT2T2toST2ProductExpr<1u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, ST2toT2T2toST2ProductExpr<1u>>>,
          public fsarray<
              TensorDimeToSize<T2toT2Traits<T2toT2ResultType>::dime>::value *
                  TensorDimeToSize<T2toT2Traits<T2toT2ResultType>::dime>::value,
              typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime == 1u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename ST2toT2Type, typename T2toST2Type2>
      TFEL_MATH_INLINE Expr(const ST2toT2Type& a, const T2toST2Type2& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type2, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2Type>::dime == 1u));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type2>::dime == 1u));
        this->v[0] = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0) + a(0, 2) * b(2, 0);
        this->v[1] = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1) + a(0, 2) * b(2, 1);
        this->v[2] = a(0, 0) * b(0, 2) + a(0, 1) * b(1, 2) + a(0, 2) * b(2, 2);
        this->v[3] = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0) + a(1, 2) * b(2, 0);
        this->v[4] = a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1) + a(1, 2) * b(2, 1);
        this->v[5] = a(1, 0) * b(0, 2) + a(1, 1) * b(1, 2) + a(1, 2) * b(2, 2);
        this->v[6] = a(2, 0) * b(0, 0) + a(2, 1) * b(1, 0) + a(2, 2) * b(2, 0);
        this->v[7] = a(2, 0) * b(0, 1) + a(2, 1) * b(1, 1) + a(2, 2) * b(2, 1);
        this->v[8] = a(2, 0) * b(0, 2) + a(2, 1) * b(1, 2) + a(2, 2) * b(2, 2);
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i, const unsigned short j) const {
        return this->v[i * 3 + j];
      }  // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct Expr<T2toT2ResultType,ST2toT2T2toST2ProductExpr>

    /*!
     * Partial specialisation
     */
    template <typename T2toT2ResultType>
    struct TFEL_VISIBILITY_LOCAL
        Expr<T2toT2ResultType, ST2toT2T2toST2ProductExpr<2u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, ST2toT2T2toST2ProductExpr<2u>>>,
          public fsarray<
              TensorDimeToSize<T2toT2Traits<T2toT2ResultType>::dime>::value *
                  TensorDimeToSize<T2toT2Traits<T2toT2ResultType>::dime>::value,
              typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime == 2u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename ST2toT2Type, typename T2toST2Type2>
      TFEL_MATH_INLINE Expr(const ST2toT2Type& a, const T2toST2Type2& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type2, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2Type>::dime == 2u));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type2>::dime == 2u));
        this->v[0] = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0) + a(0, 2) * b(2, 0) +
                     a(0, 3) * b(3, 0);
        this->v[1] = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1) + a(0, 2) * b(2, 1) +
                     a(0, 3) * b(3, 1);
        this->v[2] = a(0, 0) * b(0, 2) + a(0, 1) * b(1, 2) + a(0, 2) * b(2, 2) +
                     a(0, 3) * b(3, 2);
        this->v[3] = a(0, 0) * b(0, 3) + a(0, 1) * b(1, 3) + a(0, 2) * b(2, 3) +
                     a(0, 3) * b(3, 3);
        this->v[4] = a(0, 0) * b(0, 4) + a(0, 1) * b(1, 4) + a(0, 2) * b(2, 4) +
                     a(0, 3) * b(3, 4);
        this->v[5] = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0) + a(1, 2) * b(2, 0) +
                     a(1, 3) * b(3, 0);
        this->v[6] = a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1) + a(1, 2) * b(2, 1) +
                     a(1, 3) * b(3, 1);
        this->v[7] = a(1, 0) * b(0, 2) + a(1, 1) * b(1, 2) + a(1, 2) * b(2, 2) +
                     a(1, 3) * b(3, 2);
        this->v[8] = a(1, 0) * b(0, 3) + a(1, 1) * b(1, 3) + a(1, 2) * b(2, 3) +
                     a(1, 3) * b(3, 3);
        this->v[9] = a(1, 0) * b(0, 4) + a(1, 1) * b(1, 4) + a(1, 2) * b(2, 4) +
                     a(1, 3) * b(3, 4);
        this->v[10] = a(2, 0) * b(0, 0) + a(2, 1) * b(1, 0) +
                      a(2, 2) * b(2, 0) + a(2, 3) * b(3, 0);
        this->v[11] = a(2, 0) * b(0, 1) + a(2, 1) * b(1, 1) +
                      a(2, 2) * b(2, 1) + a(2, 3) * b(3, 1);
        this->v[12] = a(2, 0) * b(0, 2) + a(2, 1) * b(1, 2) +
                      a(2, 2) * b(2, 2) + a(2, 3) * b(3, 2);
        this->v[13] = a(2, 0) * b(0, 3) + a(2, 1) * b(1, 3) +
                      a(2, 2) * b(2, 3) + a(2, 3) * b(3, 3);
        this->v[14] = a(2, 0) * b(0, 4) + a(2, 1) * b(1, 4) +
                      a(2, 2) * b(2, 4) + a(2, 3) * b(3, 4);
        this->v[15] = a(3, 0) * b(0, 0) + a(3, 1) * b(1, 0) +
                      a(3, 2) * b(2, 0) + a(3, 3) * b(3, 0);
        this->v[16] = a(3, 0) * b(0, 1) + a(3, 1) * b(1, 1) +
                      a(3, 2) * b(2, 1) + a(3, 3) * b(3, 1);
        this->v[17] = a(3, 0) * b(0, 2) + a(3, 1) * b(1, 2) +
                      a(3, 2) * b(2, 2) + a(3, 3) * b(3, 2);
        this->v[18] = a(3, 0) * b(0, 3) + a(3, 1) * b(1, 3) +
                      a(3, 2) * b(2, 3) + a(3, 3) * b(3, 3);
        this->v[19] = a(3, 0) * b(0, 4) + a(3, 1) * b(1, 4) +
                      a(3, 2) * b(2, 4) + a(3, 3) * b(3, 4);
        this->v[20] = a(4, 0) * b(0, 0) + a(4, 1) * b(1, 0) +
                      a(4, 2) * b(2, 0) + a(4, 3) * b(3, 0);
        this->v[21] = a(4, 0) * b(0, 1) + a(4, 1) * b(1, 1) +
                      a(4, 2) * b(2, 1) + a(4, 3) * b(3, 1);
        this->v[22] = a(4, 0) * b(0, 2) + a(4, 1) * b(1, 2) +
                      a(4, 2) * b(2, 2) + a(4, 3) * b(3, 2);
        this->v[23] = a(4, 0) * b(0, 3) + a(4, 1) * b(1, 3) +
                      a(4, 2) * b(2, 3) + a(4, 3) * b(3, 3);
        this->v[24] = a(4, 0) * b(0, 4) + a(4, 1) * b(1, 4) +
                      a(4, 2) * b(2, 4) + a(4, 3) * b(3, 4);
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i, const unsigned short j) const {
        return this->v[i * 5 + j];
      }  // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct Expr<T2toT2ResultType,ST2toT2T2toST2ProductExpr>

    /*!
     * Partial specialisation
     */
    template <typename T2toT2ResultType>
    struct TFEL_VISIBILITY_LOCAL
        Expr<T2toT2ResultType, ST2toT2T2toST2ProductExpr<3u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, ST2toT2T2toST2ProductExpr<3u>>>,
          public fsarray<
              TensorDimeToSize<T2toT2Traits<T2toT2ResultType>::dime>::value *
                  TensorDimeToSize<T2toT2Traits<T2toT2ResultType>::dime>::value,
              typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime == 3u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template <typename ST2toT2Type, typename T2toST2Type2>
      TFEL_MATH_INLINE Expr(const ST2toT2Type& a, const T2toST2Type2& b) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toT2Type, ST2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type2, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2Type>::dime == 3u));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type2>::dime == 3u));
        this->v[0] = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0) + a(0, 2) * b(2, 0) +
                     a(0, 3) * b(3, 0) + a(0, 4) * b(4, 0) + a(0, 5) * b(5, 0);
        this->v[1] = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1) + a(0, 2) * b(2, 1) +
                     a(0, 3) * b(3, 1) + a(0, 4) * b(4, 1) + a(0, 5) * b(5, 1);
        this->v[2] = a(0, 0) * b(0, 2) + a(0, 1) * b(1, 2) + a(0, 2) * b(2, 2) +
                     a(0, 3) * b(3, 2) + a(0, 4) * b(4, 2) + a(0, 5) * b(5, 2);
        this->v[3] = a(0, 0) * b(0, 3) + a(0, 1) * b(1, 3) + a(0, 2) * b(2, 3) +
                     a(0, 3) * b(3, 3) + a(0, 4) * b(4, 3) + a(0, 5) * b(5, 3);
        this->v[4] = a(0, 0) * b(0, 4) + a(0, 1) * b(1, 4) + a(0, 2) * b(2, 4) +
                     a(0, 3) * b(3, 4) + a(0, 4) * b(4, 4) + a(0, 5) * b(5, 4);
        this->v[5] = a(0, 0) * b(0, 5) + a(0, 1) * b(1, 5) + a(0, 2) * b(2, 5) +
                     a(0, 3) * b(3, 5) + a(0, 4) * b(4, 5) + a(0, 5) * b(5, 5);
        this->v[6] = a(0, 0) * b(0, 6) + a(0, 1) * b(1, 6) + a(0, 2) * b(2, 6) +
                     a(0, 3) * b(3, 6) + a(0, 4) * b(4, 6) + a(0, 5) * b(5, 6);
        this->v[7] = a(0, 0) * b(0, 7) + a(0, 1) * b(1, 7) + a(0, 2) * b(2, 7) +
                     a(0, 3) * b(3, 7) + a(0, 4) * b(4, 7) + a(0, 5) * b(5, 7);
        this->v[8] = a(0, 0) * b(0, 8) + a(0, 1) * b(1, 8) + a(0, 2) * b(2, 8) +
                     a(0, 3) * b(3, 8) + a(0, 4) * b(4, 8) + a(0, 5) * b(5, 8);
        this->v[9] = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0) + a(1, 2) * b(2, 0) +
                     a(1, 3) * b(3, 0) + a(1, 4) * b(4, 0) + a(1, 5) * b(5, 0);
        this->v[10] = a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1) +
                      a(1, 2) * b(2, 1) + a(1, 3) * b(3, 1) +
                      a(1, 4) * b(4, 1) + a(1, 5) * b(5, 1);
        this->v[11] = a(1, 0) * b(0, 2) + a(1, 1) * b(1, 2) +
                      a(1, 2) * b(2, 2) + a(1, 3) * b(3, 2) +
                      a(1, 4) * b(4, 2) + a(1, 5) * b(5, 2);
        this->v[12] = a(1, 0) * b(0, 3) + a(1, 1) * b(1, 3) +
                      a(1, 2) * b(2, 3) + a(1, 3) * b(3, 3) +
                      a(1, 4) * b(4, 3) + a(1, 5) * b(5, 3);
        this->v[13] = a(1, 0) * b(0, 4) + a(1, 1) * b(1, 4) +
                      a(1, 2) * b(2, 4) + a(1, 3) * b(3, 4) +
                      a(1, 4) * b(4, 4) + a(1, 5) * b(5, 4);
        this->v[14] = a(1, 0) * b(0, 5) + a(1, 1) * b(1, 5) +
                      a(1, 2) * b(2, 5) + a(1, 3) * b(3, 5) +
                      a(1, 4) * b(4, 5) + a(1, 5) * b(5, 5);
        this->v[15] = a(1, 0) * b(0, 6) + a(1, 1) * b(1, 6) +
                      a(1, 2) * b(2, 6) + a(1, 3) * b(3, 6) +
                      a(1, 4) * b(4, 6) + a(1, 5) * b(5, 6);
        this->v[16] = a(1, 0) * b(0, 7) + a(1, 1) * b(1, 7) +
                      a(1, 2) * b(2, 7) + a(1, 3) * b(3, 7) +
                      a(1, 4) * b(4, 7) + a(1, 5) * b(5, 7);
        this->v[17] = a(1, 0) * b(0, 8) + a(1, 1) * b(1, 8) +
                      a(1, 2) * b(2, 8) + a(1, 3) * b(3, 8) +
                      a(1, 4) * b(4, 8) + a(1, 5) * b(5, 8);
        this->v[18] = a(2, 0) * b(0, 0) + a(2, 1) * b(1, 0) +
                      a(2, 2) * b(2, 0) + a(2, 3) * b(3, 0) +
                      a(2, 4) * b(4, 0) + a(2, 5) * b(5, 0);
        this->v[19] = a(2, 0) * b(0, 1) + a(2, 1) * b(1, 1) +
                      a(2, 2) * b(2, 1) + a(2, 3) * b(3, 1) +
                      a(2, 4) * b(4, 1) + a(2, 5) * b(5, 1);
        this->v[20] = a(2, 0) * b(0, 2) + a(2, 1) * b(1, 2) +
                      a(2, 2) * b(2, 2) + a(2, 3) * b(3, 2) +
                      a(2, 4) * b(4, 2) + a(2, 5) * b(5, 2);
        this->v[21] = a(2, 0) * b(0, 3) + a(2, 1) * b(1, 3) +
                      a(2, 2) * b(2, 3) + a(2, 3) * b(3, 3) +
                      a(2, 4) * b(4, 3) + a(2, 5) * b(5, 3);
        this->v[22] = a(2, 0) * b(0, 4) + a(2, 1) * b(1, 4) +
                      a(2, 2) * b(2, 4) + a(2, 3) * b(3, 4) +
                      a(2, 4) * b(4, 4) + a(2, 5) * b(5, 4);
        this->v[23] = a(2, 0) * b(0, 5) + a(2, 1) * b(1, 5) +
                      a(2, 2) * b(2, 5) + a(2, 3) * b(3, 5) +
                      a(2, 4) * b(4, 5) + a(2, 5) * b(5, 5);
        this->v[24] = a(2, 0) * b(0, 6) + a(2, 1) * b(1, 6) +
                      a(2, 2) * b(2, 6) + a(2, 3) * b(3, 6) +
                      a(2, 4) * b(4, 6) + a(2, 5) * b(5, 6);
        this->v[25] = a(2, 0) * b(0, 7) + a(2, 1) * b(1, 7) +
                      a(2, 2) * b(2, 7) + a(2, 3) * b(3, 7) +
                      a(2, 4) * b(4, 7) + a(2, 5) * b(5, 7);
        this->v[26] = a(2, 0) * b(0, 8) + a(2, 1) * b(1, 8) +
                      a(2, 2) * b(2, 8) + a(2, 3) * b(3, 8) +
                      a(2, 4) * b(4, 8) + a(2, 5) * b(5, 8);
        this->v[27] = a(3, 0) * b(0, 0) + a(3, 1) * b(1, 0) +
                      a(3, 2) * b(2, 0) + a(3, 3) * b(3, 0) +
                      a(3, 4) * b(4, 0) + a(3, 5) * b(5, 0);
        this->v[28] = a(3, 0) * b(0, 1) + a(3, 1) * b(1, 1) +
                      a(3, 2) * b(2, 1) + a(3, 3) * b(3, 1) +
                      a(3, 4) * b(4, 1) + a(3, 5) * b(5, 1);
        this->v[29] = a(3, 0) * b(0, 2) + a(3, 1) * b(1, 2) +
                      a(3, 2) * b(2, 2) + a(3, 3) * b(3, 2) +
                      a(3, 4) * b(4, 2) + a(3, 5) * b(5, 2);
        this->v[30] = a(3, 0) * b(0, 3) + a(3, 1) * b(1, 3) +
                      a(3, 2) * b(2, 3) + a(3, 3) * b(3, 3) +
                      a(3, 4) * b(4, 3) + a(3, 5) * b(5, 3);
        this->v[31] = a(3, 0) * b(0, 4) + a(3, 1) * b(1, 4) +
                      a(3, 2) * b(2, 4) + a(3, 3) * b(3, 4) +
                      a(3, 4) * b(4, 4) + a(3, 5) * b(5, 4);
        this->v[32] = a(3, 0) * b(0, 5) + a(3, 1) * b(1, 5) +
                      a(3, 2) * b(2, 5) + a(3, 3) * b(3, 5) +
                      a(3, 4) * b(4, 5) + a(3, 5) * b(5, 5);
        this->v[33] = a(3, 0) * b(0, 6) + a(3, 1) * b(1, 6) +
                      a(3, 2) * b(2, 6) + a(3, 3) * b(3, 6) +
                      a(3, 4) * b(4, 6) + a(3, 5) * b(5, 6);
        this->v[34] = a(3, 0) * b(0, 7) + a(3, 1) * b(1, 7) +
                      a(3, 2) * b(2, 7) + a(3, 3) * b(3, 7) +
                      a(3, 4) * b(4, 7) + a(3, 5) * b(5, 7);
        this->v[35] = a(3, 0) * b(0, 8) + a(3, 1) * b(1, 8) +
                      a(3, 2) * b(2, 8) + a(3, 3) * b(3, 8) +
                      a(3, 4) * b(4, 8) + a(3, 5) * b(5, 8);
        this->v[36] = a(4, 0) * b(0, 0) + a(4, 1) * b(1, 0) +
                      a(4, 2) * b(2, 0) + a(4, 3) * b(3, 0) +
                      a(4, 4) * b(4, 0) + a(4, 5) * b(5, 0);
        this->v[37] = a(4, 0) * b(0, 1) + a(4, 1) * b(1, 1) +
                      a(4, 2) * b(2, 1) + a(4, 3) * b(3, 1) +
                      a(4, 4) * b(4, 1) + a(4, 5) * b(5, 1);
        this->v[38] = a(4, 0) * b(0, 2) + a(4, 1) * b(1, 2) +
                      a(4, 2) * b(2, 2) + a(4, 3) * b(3, 2) +
                      a(4, 4) * b(4, 2) + a(4, 5) * b(5, 2);
        this->v[39] = a(4, 0) * b(0, 3) + a(4, 1) * b(1, 3) +
                      a(4, 2) * b(2, 3) + a(4, 3) * b(3, 3) +
                      a(4, 4) * b(4, 3) + a(4, 5) * b(5, 3);
        this->v[40] = a(4, 0) * b(0, 4) + a(4, 1) * b(1, 4) +
                      a(4, 2) * b(2, 4) + a(4, 3) * b(3, 4) +
                      a(4, 4) * b(4, 4) + a(4, 5) * b(5, 4);
        this->v[41] = a(4, 0) * b(0, 5) + a(4, 1) * b(1, 5) +
                      a(4, 2) * b(2, 5) + a(4, 3) * b(3, 5) +
                      a(4, 4) * b(4, 5) + a(4, 5) * b(5, 5);
        this->v[42] = a(4, 0) * b(0, 6) + a(4, 1) * b(1, 6) +
                      a(4, 2) * b(2, 6) + a(4, 3) * b(3, 6) +
                      a(4, 4) * b(4, 6) + a(4, 5) * b(5, 6);
        this->v[43] = a(4, 0) * b(0, 7) + a(4, 1) * b(1, 7) +
                      a(4, 2) * b(2, 7) + a(4, 3) * b(3, 7) +
                      a(4, 4) * b(4, 7) + a(4, 5) * b(5, 7);
        this->v[44] = a(4, 0) * b(0, 8) + a(4, 1) * b(1, 8) +
                      a(4, 2) * b(2, 8) + a(4, 3) * b(3, 8) +
                      a(4, 4) * b(4, 8) + a(4, 5) * b(5, 8);
        this->v[45] = a(5, 0) * b(0, 0) + a(5, 1) * b(1, 0) +
                      a(5, 2) * b(2, 0) + a(5, 3) * b(3, 0) +
                      a(5, 4) * b(4, 0) + a(5, 5) * b(5, 0);
        this->v[46] = a(5, 0) * b(0, 1) + a(5, 1) * b(1, 1) +
                      a(5, 2) * b(2, 1) + a(5, 3) * b(3, 1) +
                      a(5, 4) * b(4, 1) + a(5, 5) * b(5, 1);
        this->v[47] = a(5, 0) * b(0, 2) + a(5, 1) * b(1, 2) +
                      a(5, 2) * b(2, 2) + a(5, 3) * b(3, 2) +
                      a(5, 4) * b(4, 2) + a(5, 5) * b(5, 2);
        this->v[48] = a(5, 0) * b(0, 3) + a(5, 1) * b(1, 3) +
                      a(5, 2) * b(2, 3) + a(5, 3) * b(3, 3) +
                      a(5, 4) * b(4, 3) + a(5, 5) * b(5, 3);
        this->v[49] = a(5, 0) * b(0, 4) + a(5, 1) * b(1, 4) +
                      a(5, 2) * b(2, 4) + a(5, 3) * b(3, 4) +
                      a(5, 4) * b(4, 4) + a(5, 5) * b(5, 4);
        this->v[50] = a(5, 0) * b(0, 5) + a(5, 1) * b(1, 5) +
                      a(5, 2) * b(2, 5) + a(5, 3) * b(3, 5) +
                      a(5, 4) * b(4, 5) + a(5, 5) * b(5, 5);
        this->v[51] = a(5, 0) * b(0, 6) + a(5, 1) * b(1, 6) +
                      a(5, 2) * b(2, 6) + a(5, 3) * b(3, 6) +
                      a(5, 4) * b(4, 6) + a(5, 5) * b(5, 6);
        this->v[52] = a(5, 0) * b(0, 7) + a(5, 1) * b(1, 7) +
                      a(5, 2) * b(2, 7) + a(5, 3) * b(3, 7) +
                      a(5, 4) * b(4, 7) + a(5, 5) * b(5, 7);
        this->v[53] = a(5, 0) * b(0, 8) + a(5, 1) * b(1, 8) +
                      a(5, 2) * b(2, 8) + a(5, 3) * b(3, 8) +
                      a(5, 4) * b(4, 8) + a(5, 5) * b(5, 8);
        this->v[54] = a(6, 0) * b(0, 0) + a(6, 1) * b(1, 0) +
                      a(6, 2) * b(2, 0) + a(6, 3) * b(3, 0) +
                      a(6, 4) * b(4, 0) + a(6, 5) * b(5, 0);
        this->v[55] = a(6, 0) * b(0, 1) + a(6, 1) * b(1, 1) +
                      a(6, 2) * b(2, 1) + a(6, 3) * b(3, 1) +
                      a(6, 4) * b(4, 1) + a(6, 5) * b(5, 1);
        this->v[56] = a(6, 0) * b(0, 2) + a(6, 1) * b(1, 2) +
                      a(6, 2) * b(2, 2) + a(6, 3) * b(3, 2) +
                      a(6, 4) * b(4, 2) + a(6, 5) * b(5, 2);
        this->v[57] = a(6, 0) * b(0, 3) + a(6, 1) * b(1, 3) +
                      a(6, 2) * b(2, 3) + a(6, 3) * b(3, 3) +
                      a(6, 4) * b(4, 3) + a(6, 5) * b(5, 3);
        this->v[58] = a(6, 0) * b(0, 4) + a(6, 1) * b(1, 4) +
                      a(6, 2) * b(2, 4) + a(6, 3) * b(3, 4) +
                      a(6, 4) * b(4, 4) + a(6, 5) * b(5, 4);
        this->v[59] = a(6, 0) * b(0, 5) + a(6, 1) * b(1, 5) +
                      a(6, 2) * b(2, 5) + a(6, 3) * b(3, 5) +
                      a(6, 4) * b(4, 5) + a(6, 5) * b(5, 5);
        this->v[60] = a(6, 0) * b(0, 6) + a(6, 1) * b(1, 6) +
                      a(6, 2) * b(2, 6) + a(6, 3) * b(3, 6) +
                      a(6, 4) * b(4, 6) + a(6, 5) * b(5, 6);
        this->v[61] = a(6, 0) * b(0, 7) + a(6, 1) * b(1, 7) +
                      a(6, 2) * b(2, 7) + a(6, 3) * b(3, 7) +
                      a(6, 4) * b(4, 7) + a(6, 5) * b(5, 7);
        this->v[62] = a(6, 0) * b(0, 8) + a(6, 1) * b(1, 8) +
                      a(6, 2) * b(2, 8) + a(6, 3) * b(3, 8) +
                      a(6, 4) * b(4, 8) + a(6, 5) * b(5, 8);
        this->v[63] = a(7, 0) * b(0, 0) + a(7, 1) * b(1, 0) +
                      a(7, 2) * b(2, 0) + a(7, 3) * b(3, 0) +
                      a(7, 4) * b(4, 0) + a(7, 5) * b(5, 0);
        this->v[64] = a(7, 0) * b(0, 1) + a(7, 1) * b(1, 1) +
                      a(7, 2) * b(2, 1) + a(7, 3) * b(3, 1) +
                      a(7, 4) * b(4, 1) + a(7, 5) * b(5, 1);
        this->v[65] = a(7, 0) * b(0, 2) + a(7, 1) * b(1, 2) +
                      a(7, 2) * b(2, 2) + a(7, 3) * b(3, 2) +
                      a(7, 4) * b(4, 2) + a(7, 5) * b(5, 2);
        this->v[66] = a(7, 0) * b(0, 3) + a(7, 1) * b(1, 3) +
                      a(7, 2) * b(2, 3) + a(7, 3) * b(3, 3) +
                      a(7, 4) * b(4, 3) + a(7, 5) * b(5, 3);
        this->v[67] = a(7, 0) * b(0, 4) + a(7, 1) * b(1, 4) +
                      a(7, 2) * b(2, 4) + a(7, 3) * b(3, 4) +
                      a(7, 4) * b(4, 4) + a(7, 5) * b(5, 4);
        this->v[68] = a(7, 0) * b(0, 5) + a(7, 1) * b(1, 5) +
                      a(7, 2) * b(2, 5) + a(7, 3) * b(3, 5) +
                      a(7, 4) * b(4, 5) + a(7, 5) * b(5, 5);
        this->v[69] = a(7, 0) * b(0, 6) + a(7, 1) * b(1, 6) +
                      a(7, 2) * b(2, 6) + a(7, 3) * b(3, 6) +
                      a(7, 4) * b(4, 6) + a(7, 5) * b(5, 6);
        this->v[70] = a(7, 0) * b(0, 7) + a(7, 1) * b(1, 7) +
                      a(7, 2) * b(2, 7) + a(7, 3) * b(3, 7) +
                      a(7, 4) * b(4, 7) + a(7, 5) * b(5, 7);
        this->v[71] = a(7, 0) * b(0, 8) + a(7, 1) * b(1, 8) +
                      a(7, 2) * b(2, 8) + a(7, 3) * b(3, 8) +
                      a(7, 4) * b(4, 8) + a(7, 5) * b(5, 8);
        this->v[72] = a(8, 0) * b(0, 0) + a(8, 1) * b(1, 0) +
                      a(8, 2) * b(2, 0) + a(8, 3) * b(3, 0) +
                      a(8, 4) * b(4, 0) + a(8, 5) * b(5, 0);
        this->v[73] = a(8, 0) * b(0, 1) + a(8, 1) * b(1, 1) +
                      a(8, 2) * b(2, 1) + a(8, 3) * b(3, 1) +
                      a(8, 4) * b(4, 1) + a(8, 5) * b(5, 1);
        this->v[74] = a(8, 0) * b(0, 2) + a(8, 1) * b(1, 2) +
                      a(8, 2) * b(2, 2) + a(8, 3) * b(3, 2) +
                      a(8, 4) * b(4, 2) + a(8, 5) * b(5, 2);
        this->v[75] = a(8, 0) * b(0, 3) + a(8, 1) * b(1, 3) +
                      a(8, 2) * b(2, 3) + a(8, 3) * b(3, 3) +
                      a(8, 4) * b(4, 3) + a(8, 5) * b(5, 3);
        this->v[76] = a(8, 0) * b(0, 4) + a(8, 1) * b(1, 4) +
                      a(8, 2) * b(2, 4) + a(8, 3) * b(3, 4) +
                      a(8, 4) * b(4, 4) + a(8, 5) * b(5, 4);
        this->v[77] = a(8, 0) * b(0, 5) + a(8, 1) * b(1, 5) +
                      a(8, 2) * b(2, 5) + a(8, 3) * b(3, 5) +
                      a(8, 4) * b(4, 5) + a(8, 5) * b(5, 5);
        this->v[78] = a(8, 0) * b(0, 6) + a(8, 1) * b(1, 6) +
                      a(8, 2) * b(2, 6) + a(8, 3) * b(3, 6) +
                      a(8, 4) * b(4, 6) + a(8, 5) * b(5, 6);
        this->v[79] = a(8, 0) * b(0, 7) + a(8, 1) * b(1, 7) +
                      a(8, 2) * b(2, 7) + a(8, 3) * b(3, 7) +
                      a(8, 4) * b(4, 7) + a(8, 5) * b(5, 7);
        this->v[80] = a(8, 0) * b(0, 8) + a(8, 1) * b(1, 8) +
                      a(8, 2) * b(2, 8) + a(8, 3) * b(3, 8) +
                      a(8, 4) * b(4, 8) + a(8, 5) * b(5, 8);
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i, const unsigned short j) const {
        return this->v[i * 9 + j];
      }  // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct Expr<T2toT2ResultType,ST2toT2T2toST2ProductExpr>

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_ST2TOT2T2TOST2PRODUCTEXPR_H_ */
