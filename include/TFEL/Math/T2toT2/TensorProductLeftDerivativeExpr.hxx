/*!
 * \file  include/TFEL/Math/T2toT2/TensorProductLeftDerivativeExpr.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 04 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORPRODUCTLEFTDERIVATIVEEXPR_HXX
#define LIB_TFEL_MATH_TENSORPRODUCTLEFTDERIVATIVEEXPR_HXX

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"

namespace tfel {

  namespace math {

    /*!
     * Empty structure allowing partial specialisation
     */
    template <unsigned short N>
    struct TensorProductLeftDerivativeExpr {
    };  // end of struct TensorProductLeftDerivativeExpr

    /*!
     * Partial specialisation for 1D tensor
     */
    template <typename T2toT2ResultType>
    struct Expr<T2toT2ResultType, TensorProductLeftDerivativeExpr<1u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, TensorProductLeftDerivativeExpr<1u>>>,
          public fsarray<9u, typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2ResultType>::dime == 1u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] B : second tensor of the product
       */
      template <typename TensorType>
      Expr(const TensorType& B) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                typename T2toT2Traits<T2toT2ResultType>::NumType>::cond));
        this->v[0] = B[0];
        this->v[4] = B[1];
        this->v[8] = B[2];
        this->v[1] = this->v[2] = value_type(0);
        this->v[3] = this->v[5] = value_type(0);
        this->v[6] = this->v[7] = value_type(0);
      }  // end of Expr
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       */
      template <typename TensorType, typename T2toT2Type>
      Expr(const TensorType& B, const T2toT2Type& C) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toT2Type,
                                    tfel::math::T2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (T2toT2Traits<T2toT2Type>::dime == TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename TensorTraits<TensorType>::NumType,
                    typename T2toT2Traits<T2toT2Type>::NumType, OpMult>::Result,
                typename T2toT2Traits<T2toT2ResultType>::NumType>::cond));
        this->v[0] = C(0, 0) * B[0];
        this->v[1] = C(0, 1) * B[0];
        this->v[2] = C(0, 2) * B[0];
        this->v[3] = C(1, 0) * B[1];
        this->v[4] = C(1, 1) * B[1];
        this->v[5] = C(1, 2) * B[1];
        this->v[6] = C(2, 0) * B[2];
        this->v[7] = C(2, 1) * B[2];
        this->v[8] = C(2, 2) * B[2];
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type& operator()(const unsigned short i,
                                   const unsigned short j) const {
        return this->v[i * 3 + j];
      }  // end of operator()
      /*!
       * \return the runtime properties of this object
       * In this case, the number of lines and columns
       * are deduced from the template parameter
       */
      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct
        // Expr<T2toT2ResultType,TensorProductLeftDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D tensor
     */
    template <typename T2toT2ResultType>
    struct Expr<T2toT2ResultType, TensorProductLeftDerivativeExpr<2u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, TensorProductLeftDerivativeExpr<2u>>>,
          public fsarray<25u,
                         typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2ResultType>::dime == 2u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] B : second tensor of the product
       */
      template <typename TensorType>
      Expr(const TensorType& B) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                typename T2toT2Traits<T2toT2ResultType>::NumType>::cond));
        this->v[0] = B[0];
        this->v[1] = value_type(0);
        this->v[2] = value_type(0);
        this->v[3] = B[4];
        this->v[4] = value_type(0);
        this->v[5] = value_type(0);
        this->v[6] = B[1];
        this->v[7] = value_type(0);
        this->v[8] = value_type(0);
        this->v[9] = B[3];
        this->v[10] = value_type(0);
        this->v[11] = value_type(0);
        this->v[12] = B[2];
        this->v[13] = value_type(0);
        this->v[14] = value_type(0);
        this->v[15] = B[3];
        this->v[16] = value_type(0);
        this->v[17] = value_type(0);
        this->v[18] = B[1];
        this->v[19] = value_type(0);
        this->v[20] = value_type(0);
        this->v[21] = B[4];
        this->v[22] = value_type(0);
        this->v[23] = value_type(0);
        this->v[24] = B[0];
      }  // end of Expr
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       */
      template <typename TensorType, typename T2toT2Type>
      Expr(const TensorType& B, const T2toT2Type& C) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toT2Type,
                                    tfel::math::T2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (T2toT2Traits<T2toT2Type>::dime == TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename TensorTraits<TensorType>::NumType,
                    typename T2toT2Traits<T2toT2Type>::NumType, OpMult>::Result,
                typename T2toT2Traits<T2toT2ResultType>::NumType>::cond));
        this->v[0] = C(3, 0) * B(4) + C(0, 0) * B(0);
        this->v[1] = C(3, 1) * B(4) + C(0, 1) * B(0);
        this->v[2] = C(3, 2) * B(4) + C(0, 2) * B(0);
        this->v[3] = C(3, 3) * B(4) + C(0, 3) * B(0);
        this->v[4] = C(3, 4) * B(4) + C(0, 4) * B(0);
        this->v[5] = C(4, 0) * B(3) + C(1, 0) * B(1);
        this->v[6] = C(4, 1) * B(3) + C(1, 1) * B(1);
        this->v[7] = C(4, 2) * B(3) + C(1, 2) * B(1);
        this->v[8] = C(4, 3) * B(3) + C(1, 3) * B(1);
        this->v[9] = C(4, 4) * B(3) + C(1, 4) * B(1);
        this->v[10] = C(2, 0) * B(2);
        this->v[11] = C(2, 1) * B(2);
        this->v[12] = C(2, 2) * B(2);
        this->v[13] = C(2, 3) * B(2);
        this->v[14] = C(2, 4) * B(2);
        this->v[15] = C(0, 0) * B(3) + C(3, 0) * B(1);
        this->v[16] = C(0, 1) * B(3) + C(3, 1) * B(1);
        this->v[17] = C(0, 2) * B(3) + C(3, 2) * B(1);
        this->v[18] = C(0, 3) * B(3) + C(3, 3) * B(1);
        this->v[19] = C(0, 4) * B(3) + C(3, 4) * B(1);
        this->v[20] = C(1, 0) * B(4) + C(4, 0) * B(0);
        this->v[21] = C(1, 1) * B(4) + C(4, 1) * B(0);
        this->v[22] = C(1, 2) * B(4) + C(4, 2) * B(0);
        this->v[23] = C(1, 3) * B(4) + C(4, 3) * B(0);
        this->v[24] = C(1, 4) * B(4) + C(4, 4) * B(0);
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type& operator()(const unsigned short i,
                                   const unsigned short j) const {
        return this->v[i * 5 + j];
      }  // end of operator()
      /*!
       * \return the runtime properties of this object
       * In this case, the number of lines and columns
       * are deduced from the template parameter
       */
      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct
        // Expr<T2toT2ResultType,TensorProductLeftDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D tensor
     */
    template <typename T2toT2ResultType>
    struct Expr<T2toT2ResultType, TensorProductLeftDerivativeExpr<3u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, TensorProductLeftDerivativeExpr<3u>>>,
          public fsarray<81u,
                         typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2ResultType>::dime == 3u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] B : second tensor of the product
       */
      template <typename TensorType>
      Expr(const TensorType& B) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                typename T2toT2Traits<T2toT2ResultType>::NumType>::cond));
        this->v[0] = B[0];
        this->v[3] = B[4];
        this->v[5] = B[6];
        this->v[1] = this->v[2] = this->v[4] = this->v[6] = this->v[7] =
            this->v[8] = value_type(0);
        this->v[10] = B[1];
        this->v[13] = B[3];
        this->v[16] = B[8];
        this->v[9] = this->v[11] = this->v[12] = this->v[14] = this->v[15] =
            this->v[17] = value_type(0);
        this->v[20] = B[2];
        this->v[24] = B[5];
        this->v[26] = B[7];
        this->v[18] = this->v[19] = this->v[21] = this->v[22] = this->v[23] =
            this->v[25] = value_type(0);
        this->v[27] = B[3];
        this->v[30] = B[1];
        this->v[32] = B[8];
        this->v[28] = this->v[29] = this->v[31] = this->v[33] = this->v[34] =
            this->v[35] = value_type(0);
        this->v[37] = B[4];
        this->v[40] = B[0];
        this->v[43] = B[6];
        this->v[36] = this->v[38] = this->v[39] = this->v[41] = this->v[42] =
            this->v[44] = value_type(0);
        this->v[45] = B[5];
        this->v[48] = B[7];
        this->v[50] = B[2];
        this->v[46] = this->v[47] = this->v[49] = this->v[51] = this->v[52] =
            this->v[53] = value_type(0);
        this->v[56] = B[6];
        this->v[60] = B[0];
        this->v[62] = B[4];
        this->v[54] = this->v[55] = this->v[57] = this->v[58] = this->v[59] =
            this->v[61] = value_type(0);
        this->v[64] = B[7];
        this->v[67] = B[5];
        this->v[70] = B[2];
        this->v[63] = this->v[65] = this->v[66] = this->v[68] = this->v[69] =
            this->v[71] = value_type(0);
        this->v[74] = B[8];
        this->v[78] = B[3];
        this->v[80] = B[1];
        this->v[72] = this->v[73] = this->v[75] = this->v[76] = this->v[77] =
            this->v[79] = value_type(0);
      }  // end of Expr
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       */
      template <typename TensorType, typename T2toT2Type>
      Expr(const TensorType& B, const T2toT2Type& C) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toT2Type,
                                    tfel::math::T2toT2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2ResultType>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (T2toT2Traits<T2toT2Type>::dime == TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename TensorTraits<TensorType>::NumType,
                    typename T2toT2Traits<T2toT2Type>::NumType, OpMult>::Result,
                typename T2toT2Traits<T2toT2ResultType>::NumType>::cond));
        this->v[0] = C(5, 0) * B[6] + C(3, 0) * B[4] + C(0, 0) * B[0];
        this->v[1] = C(5, 1) * B[6] + C(3, 1) * B[4] + C(0, 1) * B[0];
        this->v[2] = C(5, 2) * B[6] + C(3, 2) * B[4] + C(0, 2) * B[0];
        this->v[3] = C(5, 3) * B[6] + C(3, 3) * B[4] + C(0, 3) * B[0];
        this->v[4] = C(5, 4) * B[6] + C(3, 4) * B[4] + C(0, 4) * B[0];
        this->v[5] = C(5, 5) * B[6] + C(3, 5) * B[4] + C(0, 5) * B[0];
        this->v[6] = C(5, 6) * B[6] + C(3, 6) * B[4] + C(0, 6) * B[0];
        this->v[7] = C(5, 7) * B[6] + C(3, 7) * B[4] + C(0, 7) * B[0];
        this->v[8] = C(5, 8) * B[6] + C(3, 8) * B[4] + C(0, 8) * B[0];
        this->v[9] = C(7, 0) * B[8] + C(4, 0) * B[3] + C(1, 0) * B[1];
        this->v[10] = C(7, 1) * B[8] + C(4, 1) * B[3] + C(1, 1) * B[1];
        this->v[11] = C(7, 2) * B[8] + C(4, 2) * B[3] + C(1, 2) * B[1];
        this->v[12] = C(7, 3) * B[8] + C(4, 3) * B[3] + C(1, 3) * B[1];
        this->v[13] = C(7, 4) * B[8] + C(4, 4) * B[3] + C(1, 4) * B[1];
        this->v[14] = C(7, 5) * B[8] + C(4, 5) * B[3] + C(1, 5) * B[1];
        this->v[15] = C(7, 6) * B[8] + C(4, 6) * B[3] + C(1, 6) * B[1];
        this->v[16] = C(7, 7) * B[8] + C(4, 7) * B[3] + C(1, 7) * B[1];
        this->v[17] = C(7, 8) * B[8] + C(4, 8) * B[3] + C(1, 8) * B[1];
        this->v[18] = C(8, 0) * B[7] + C(6, 0) * B[5] + C(2, 0) * B[2];
        this->v[19] = C(8, 1) * B[7] + C(6, 1) * B[5] + C(2, 1) * B[2];
        this->v[20] = C(8, 2) * B[7] + C(6, 2) * B[5] + C(2, 2) * B[2];
        this->v[21] = C(8, 3) * B[7] + C(6, 3) * B[5] + C(2, 3) * B[2];
        this->v[22] = C(8, 4) * B[7] + C(6, 4) * B[5] + C(2, 4) * B[2];
        this->v[23] = C(8, 5) * B[7] + C(6, 5) * B[5] + C(2, 5) * B[2];
        this->v[24] = C(8, 6) * B[7] + C(6, 6) * B[5] + C(2, 6) * B[2];
        this->v[25] = C(8, 7) * B[7] + C(6, 7) * B[5] + C(2, 7) * B[2];
        this->v[26] = C(8, 8) * B[7] + C(6, 8) * B[5] + C(2, 8) * B[2];
        this->v[27] = C(5, 0) * B[8] + C(0, 0) * B[3] + C(3, 0) * B[1];
        this->v[28] = C(5, 1) * B[8] + C(0, 1) * B[3] + C(3, 1) * B[1];
        this->v[29] = C(5, 2) * B[8] + C(0, 2) * B[3] + C(3, 2) * B[1];
        this->v[30] = C(5, 3) * B[8] + C(0, 3) * B[3] + C(3, 3) * B[1];
        this->v[31] = C(5, 4) * B[8] + C(0, 4) * B[3] + C(3, 4) * B[1];
        this->v[32] = C(5, 5) * B[8] + C(0, 5) * B[3] + C(3, 5) * B[1];
        this->v[33] = C(5, 6) * B[8] + C(0, 6) * B[3] + C(3, 6) * B[1];
        this->v[34] = C(5, 7) * B[8] + C(0, 7) * B[3] + C(3, 7) * B[1];
        this->v[35] = C(5, 8) * B[8] + C(0, 8) * B[3] + C(3, 8) * B[1];
        this->v[36] = C(7, 0) * B[6] + C(1, 0) * B[4] + C(4, 0) * B[0];
        this->v[37] = C(7, 1) * B[6] + C(1, 1) * B[4] + C(4, 1) * B[0];
        this->v[38] = C(7, 2) * B[6] + C(1, 2) * B[4] + C(4, 2) * B[0];
        this->v[39] = C(7, 3) * B[6] + C(1, 3) * B[4] + C(4, 3) * B[0];
        this->v[40] = C(7, 4) * B[6] + C(1, 4) * B[4] + C(4, 4) * B[0];
        this->v[41] = C(7, 5) * B[6] + C(1, 5) * B[4] + C(4, 5) * B[0];
        this->v[42] = C(7, 6) * B[6] + C(1, 6) * B[4] + C(4, 6) * B[0];
        this->v[43] = C(7, 7) * B[6] + C(1, 7) * B[4] + C(4, 7) * B[0];
        this->v[44] = C(7, 8) * B[6] + C(1, 8) * B[4] + C(4, 8) * B[0];
        this->v[45] = C(3, 0) * B[7] + C(0, 0) * B[5] + C(5, 0) * B[2];
        this->v[46] = C(3, 1) * B[7] + C(0, 1) * B[5] + C(5, 1) * B[2];
        this->v[47] = C(3, 2) * B[7] + C(0, 2) * B[5] + C(5, 2) * B[2];
        this->v[48] = C(3, 3) * B[7] + C(0, 3) * B[5] + C(5, 3) * B[2];
        this->v[49] = C(3, 4) * B[7] + C(0, 4) * B[5] + C(5, 4) * B[2];
        this->v[50] = C(3, 5) * B[7] + C(0, 5) * B[5] + C(5, 5) * B[2];
        this->v[51] = C(3, 6) * B[7] + C(0, 6) * B[5] + C(5, 6) * B[2];
        this->v[52] = C(3, 7) * B[7] + C(0, 7) * B[5] + C(5, 7) * B[2];
        this->v[53] = C(3, 8) * B[7] + C(0, 8) * B[5] + C(5, 8) * B[2];
        this->v[54] = C(2, 0) * B[6] + C(8, 0) * B[4] + C(6, 0) * B[0];
        this->v[55] = C(2, 1) * B[6] + C(8, 1) * B[4] + C(6, 1) * B[0];
        this->v[56] = C(2, 2) * B[6] + C(8, 2) * B[4] + C(6, 2) * B[0];
        this->v[57] = C(2, 3) * B[6] + C(8, 3) * B[4] + C(6, 3) * B[0];
        this->v[58] = C(2, 4) * B[6] + C(8, 4) * B[4] + C(6, 4) * B[0];
        this->v[59] = C(2, 5) * B[6] + C(8, 5) * B[4] + C(6, 5) * B[0];
        this->v[60] = C(2, 6) * B[6] + C(8, 6) * B[4] + C(6, 6) * B[0];
        this->v[61] = C(2, 7) * B[6] + C(8, 7) * B[4] + C(6, 7) * B[0];
        this->v[62] = C(2, 8) * B[6] + C(8, 8) * B[4] + C(6, 8) * B[0];
        this->v[63] = C(1, 0) * B[7] + C(4, 0) * B[5] + C(7, 0) * B[2];
        this->v[64] = C(1, 1) * B[7] + C(4, 1) * B[5] + C(7, 1) * B[2];
        this->v[65] = C(1, 2) * B[7] + C(4, 2) * B[5] + C(7, 2) * B[2];
        this->v[66] = C(1, 3) * B[7] + C(4, 3) * B[5] + C(7, 3) * B[2];
        this->v[67] = C(1, 4) * B[7] + C(4, 4) * B[5] + C(7, 4) * B[2];
        this->v[68] = C(1, 5) * B[7] + C(4, 5) * B[5] + C(7, 5) * B[2];
        this->v[69] = C(1, 6) * B[7] + C(4, 6) * B[5] + C(7, 6) * B[2];
        this->v[70] = C(1, 7) * B[7] + C(4, 7) * B[5] + C(7, 7) * B[2];
        this->v[71] = C(1, 8) * B[7] + C(4, 8) * B[5] + C(7, 8) * B[2];
        this->v[72] = C(2, 0) * B[8] + C(6, 0) * B[3] + C(8, 0) * B[1];
        this->v[73] = C(2, 1) * B[8] + C(6, 1) * B[3] + C(8, 1) * B[1];
        this->v[74] = C(2, 2) * B[8] + C(6, 2) * B[3] + C(8, 2) * B[1];
        this->v[75] = C(2, 3) * B[8] + C(6, 3) * B[3] + C(8, 3) * B[1];
        this->v[76] = C(2, 4) * B[8] + C(6, 4) * B[3] + C(8, 4) * B[1];
        this->v[77] = C(2, 5) * B[8] + C(6, 5) * B[3] + C(8, 5) * B[1];
        this->v[78] = C(2, 6) * B[8] + C(6, 6) * B[3] + C(8, 6) * B[1];
        this->v[79] = C(2, 7) * B[8] + C(6, 7) * B[3] + C(8, 7) * B[1];
        this->v[80] = C(2, 8) * B[8] + C(6, 8) * B[3] + C(8, 8) * B[1];
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type& operator()(const unsigned short i,
                                   const unsigned short j) const {
        return this->v[i * 9 + j];
      }  // end of operator()
      /*!
       * \return the runtime properties of this object
       * In this case, the number of lines and columns
       * are deduced from the template parameter
       */
      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }
    };  // end of struct
        // Expr<T2toT2ResultType,TensorProductLeftDerivativeExpr<1u> >

  }  // namespace math

}  // namespace tfel

#endif /* LIB_TFEL_MATH_TENSORPRODUCTLEFTDERIVATIVEEXPR_HXX */
