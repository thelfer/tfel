/*!
 * \file  include/TFEL/Math/T2toT2/TensorProductRightDerivativeExpr.hxx
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

#ifndef LIB_TFEL_MATH_TENSORPRODUCTRIGHTDERIVATIVEEXPR_HXX
#define LIB_TFEL_MATH_TENSORPRODUCTRIGHTDERIVATIVEEXPR_HXX

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"

namespace tfel {

  namespace math {

    /*!
     * Empty structure allowing partial specialisation
     */
    template <unsigned short N>
    struct TensorProductRightDerivativeExpr {
    };  // end of struct TensorProductRightDerivativeExpr

    /*!
     * Partial specialisation for 1D tensor
     */
    template <typename T2toT2ResultType>
    struct Expr<T2toT2ResultType, TensorProductRightDerivativeExpr<1u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, TensorProductRightDerivativeExpr<1u>>>,
          public fsarray<9u, typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2ResultType>::dime == 1u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : second tensor of the product
       */
      template <typename TensorType>
      Expr(const TensorType& A) {
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
        this->v[0] = A[0];
        this->v[4] = A[1];
        this->v[8] = A[2];
        this->v[1] = this->v[2] = value_type(0);
        this->v[3] = this->v[5] = value_type(0);
        this->v[6] = this->v[7] = value_type(0);
      }  // end of Expr
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template <typename TensorType, typename T2toT2Type>
      Expr(const TensorType& A, const T2toT2Type& C) {
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
        this->v[0] = C(0, 0) * A[0];
        this->v[1] = C(0, 1) * A[0];
        this->v[2] = C(0, 2) * A[0];
        this->v[3] = C(1, 0) * A[1];
        this->v[4] = C(1, 1) * A[1];
        this->v[5] = C(1, 2) * A[1];
        this->v[6] = C(2, 0) * A[2];
        this->v[7] = C(2, 1) * A[2];
        this->v[8] = C(2, 2) * A[2];
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
        // Expr<T2toT2ResultType,TensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D tensor
     */
    template <typename T2toT2ResultType>
    struct Expr<T2toT2ResultType, TensorProductRightDerivativeExpr<2u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, TensorProductRightDerivativeExpr<2u>>>,
          public fsarray<25u,
                         typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2ResultType>::dime == 2u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : second tensor of the product
       */
      template <typename TensorType>
      Expr(const TensorType& A) {
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
        this->v[0] = A[0];
        this->v[4] = A[3];
        this->v[1] = this->v[2] = this->v[3] = value_type(0);
        this->v[6] = A[1];
        this->v[8] = A[4];
        this->v[5] = this->v[7] = this->v[9] = value_type(0);
        this->v[12] = A[2];
        this->v[10] = this->v[11] = this->v[13] = this->v[14] = value_type(0);
        this->v[16] = A[3];
        this->v[18] = A[0];
        this->v[15] = this->v[17] = this->v[19] = value_type(0);
        this->v[20] = A[4];
        this->v[24] = A[1];
        this->v[21] = this->v[22] = this->v[23] = value_type(0);
      }  // end of Expr
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template <typename TensorType, typename T2toT2Type>
      Expr(const TensorType& A, const T2toT2Type& C) {
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
        this->v[0] = C(4, 0) * A[3] + C(0, 0) * A[0];
        this->v[1] = C(4, 1) * A[3] + C(0, 1) * A[0];
        this->v[2] = C(4, 2) * A[3] + C(0, 2) * A[0];
        this->v[3] = C(4, 3) * A[3] + C(0, 3) * A[0];
        this->v[4] = C(4, 4) * A[3] + C(0, 4) * A[0];
        this->v[5] = C(3, 0) * A[4] + C(1, 0) * A[1];
        this->v[6] = C(3, 1) * A[4] + C(1, 1) * A[1];
        this->v[7] = C(3, 2) * A[4] + C(1, 2) * A[1];
        this->v[8] = C(3, 3) * A[4] + C(1, 3) * A[1];
        this->v[9] = C(3, 4) * A[4] + C(1, 4) * A[1];
        this->v[10] = C(2, 0) * A[2];
        this->v[11] = C(2, 1) * A[2];
        this->v[12] = C(2, 2) * A[2];
        this->v[13] = C(2, 3) * A[2];
        this->v[14] = C(2, 4) * A[2];
        this->v[15] = C(1, 0) * A[3] + C(3, 0) * A[0];
        this->v[16] = C(1, 1) * A[3] + C(3, 1) * A[0];
        this->v[17] = C(1, 2) * A[3] + C(3, 2) * A[0];
        this->v[18] = C(1, 3) * A[3] + C(3, 3) * A[0];
        this->v[19] = C(1, 4) * A[3] + C(3, 4) * A[0];
        this->v[20] = C(0, 0) * A[4] + C(4, 0) * A[1];
        this->v[21] = C(0, 1) * A[4] + C(4, 1) * A[1];
        this->v[22] = C(0, 2) * A[4] + C(4, 2) * A[1];
        this->v[23] = C(0, 3) * A[4] + C(4, 3) * A[1];
        this->v[24] = C(0, 4) * A[4] + C(4, 4) * A[1];
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
        // Expr<T2toT2ResultType,TensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D tensor
     */
    template <typename T2toT2ResultType>
    struct Expr<T2toT2ResultType, TensorProductRightDerivativeExpr<3u>>
        : public T2toT2Concept<
              Expr<T2toT2ResultType, TensorProductRightDerivativeExpr<3u>>>,
          public fsarray<81u,
                         typename T2toT2Traits<T2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2ResultType>::dime == 3u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : second tensor of the product
       */
      template <typename TensorType>
      Expr(const TensorType& A) {
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
        this->v[0] = A[0];
        this->v[4] = A[3];
        this->v[6] = A[5];
        this->v[1] = this->v[2] = this->v[3] = this->v[5] = this->v[7] =
            this->v[8] = value_type(0);
        this->v[10] = A[1];
        this->v[12] = A[4];
        this->v[17] = A[7];
        this->v[9] = this->v[11] = this->v[13] = this->v[14] = this->v[15] =
            this->v[16] = value_type(0);
        this->v[20] = A[2];
        this->v[23] = A[6];
        this->v[25] = A[8];
        this->v[18] = this->v[19] = this->v[21] = this->v[22] = this->v[24] =
            this->v[26] = value_type(0);
        this->v[28] = A[3];
        this->v[30] = A[0];
        this->v[35] = A[5];
        this->v[27] = this->v[29] = this->v[31] = this->v[32] = this->v[33] =
            this->v[34] = value_type(0);
        this->v[36] = A[4];
        this->v[40] = A[1];
        this->v[42] = A[7];
        this->v[37] = this->v[38] = this->v[39] = this->v[41] = this->v[43] =
            this->v[44] = value_type(0);
        this->v[47] = A[5];
        this->v[50] = A[0];
        this->v[52] = A[3];
        this->v[45] = this->v[46] = this->v[48] = this->v[49] = this->v[51] =
            this->v[53] = value_type(0);
        this->v[54] = A[6];
        this->v[58] = A[8];
        this->v[60] = A[2];
        this->v[55] = this->v[56] = this->v[57] = this->v[59] = this->v[61] =
            this->v[62] = value_type(0);
        this->v[65] = A[7];
        this->v[68] = A[4];
        this->v[70] = A[1];
        this->v[63] = this->v[64] = this->v[66] = this->v[67] = this->v[69] =
            this->v[71] = value_type(0);
        this->v[73] = A[8];
        this->v[75] = A[6];
        this->v[80] = A[2];
        this->v[72] = this->v[74] = this->v[76] = this->v[77] = this->v[78] =
            this->v[79] = value_type(0);
      }  // end of Expr
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template <typename TensorType, typename T2toT2Type>
      Expr(const TensorType& A, const T2toT2Type& C) {
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
        this->v[0] = C(6, 0) * A[5] + C(4, 0) * A[3] + C(0, 0) * A[0];
        this->v[1] = C(6, 1) * A[5] + C(4, 1) * A[3] + C(0, 1) * A[0];
        this->v[2] = C(6, 2) * A[5] + C(4, 2) * A[3] + C(0, 2) * A[0];
        this->v[3] = C(6, 3) * A[5] + C(4, 3) * A[3] + C(0, 3) * A[0];
        this->v[4] = C(6, 4) * A[5] + C(4, 4) * A[3] + C(0, 4) * A[0];
        this->v[5] = C(6, 5) * A[5] + C(4, 5) * A[3] + C(0, 5) * A[0];
        this->v[6] = C(6, 6) * A[5] + C(4, 6) * A[3] + C(0, 6) * A[0];
        this->v[7] = C(6, 7) * A[5] + C(4, 7) * A[3] + C(0, 7) * A[0];
        this->v[8] = C(6, 8) * A[5] + C(4, 8) * A[3] + C(0, 8) * A[0];
        this->v[9] = C(8, 0) * A[7] + C(3, 0) * A[4] + C(1, 0) * A[1];
        this->v[10] = C(8, 1) * A[7] + C(3, 1) * A[4] + C(1, 1) * A[1];
        this->v[11] = C(8, 2) * A[7] + C(3, 2) * A[4] + C(1, 2) * A[1];
        this->v[12] = C(8, 3) * A[7] + C(3, 3) * A[4] + C(1, 3) * A[1];
        this->v[13] = C(8, 4) * A[7] + C(3, 4) * A[4] + C(1, 4) * A[1];
        this->v[14] = C(8, 5) * A[7] + C(3, 5) * A[4] + C(1, 5) * A[1];
        this->v[15] = C(8, 6) * A[7] + C(3, 6) * A[4] + C(1, 6) * A[1];
        this->v[16] = C(8, 7) * A[7] + C(3, 7) * A[4] + C(1, 7) * A[1];
        this->v[17] = C(8, 8) * A[7] + C(3, 8) * A[4] + C(1, 8) * A[1];
        this->v[18] = C(7, 0) * A[8] + C(5, 0) * A[6] + C(2, 0) * A[2];
        this->v[19] = C(7, 1) * A[8] + C(5, 1) * A[6] + C(2, 1) * A[2];
        this->v[20] = C(7, 2) * A[8] + C(5, 2) * A[6] + C(2, 2) * A[2];
        this->v[21] = C(7, 3) * A[8] + C(5, 3) * A[6] + C(2, 3) * A[2];
        this->v[22] = C(7, 4) * A[8] + C(5, 4) * A[6] + C(2, 4) * A[2];
        this->v[23] = C(7, 5) * A[8] + C(5, 5) * A[6] + C(2, 5) * A[2];
        this->v[24] = C(7, 6) * A[8] + C(5, 6) * A[6] + C(2, 6) * A[2];
        this->v[25] = C(7, 7) * A[8] + C(5, 7) * A[6] + C(2, 7) * A[2];
        this->v[26] = C(7, 8) * A[8] + C(5, 8) * A[6] + C(2, 8) * A[2];
        this->v[27] = C(8, 0) * A[5] + C(1, 0) * A[3] + C(3, 0) * A[0];
        this->v[28] = C(8, 1) * A[5] + C(1, 1) * A[3] + C(3, 1) * A[0];
        this->v[29] = C(8, 2) * A[5] + C(1, 2) * A[3] + C(3, 2) * A[0];
        this->v[30] = C(8, 3) * A[5] + C(1, 3) * A[3] + C(3, 3) * A[0];
        this->v[31] = C(8, 4) * A[5] + C(1, 4) * A[3] + C(3, 4) * A[0];
        this->v[32] = C(8, 5) * A[5] + C(1, 5) * A[3] + C(3, 5) * A[0];
        this->v[33] = C(8, 6) * A[5] + C(1, 6) * A[3] + C(3, 6) * A[0];
        this->v[34] = C(8, 7) * A[5] + C(1, 7) * A[3] + C(3, 7) * A[0];
        this->v[35] = C(8, 8) * A[5] + C(1, 8) * A[3] + C(3, 8) * A[0];
        this->v[36] = C(6, 0) * A[7] + C(0, 0) * A[4] + C(4, 0) * A[1];
        this->v[37] = C(6, 1) * A[7] + C(0, 1) * A[4] + C(4, 1) * A[1];
        this->v[38] = C(6, 2) * A[7] + C(0, 2) * A[4] + C(4, 2) * A[1];
        this->v[39] = C(6, 3) * A[7] + C(0, 3) * A[4] + C(4, 3) * A[1];
        this->v[40] = C(6, 4) * A[7] + C(0, 4) * A[4] + C(4, 4) * A[1];
        this->v[41] = C(6, 5) * A[7] + C(0, 5) * A[4] + C(4, 5) * A[1];
        this->v[42] = C(6, 6) * A[7] + C(0, 6) * A[4] + C(4, 6) * A[1];
        this->v[43] = C(6, 7) * A[7] + C(0, 7) * A[4] + C(4, 7) * A[1];
        this->v[44] = C(6, 8) * A[7] + C(0, 8) * A[4] + C(4, 8) * A[1];
        this->v[45] = C(2, 0) * A[5] + C(7, 0) * A[3] + C(5, 0) * A[0];
        this->v[46] = C(2, 1) * A[5] + C(7, 1) * A[3] + C(5, 1) * A[0];
        this->v[47] = C(2, 2) * A[5] + C(7, 2) * A[3] + C(5, 2) * A[0];
        this->v[48] = C(2, 3) * A[5] + C(7, 3) * A[3] + C(5, 3) * A[0];
        this->v[49] = C(2, 4) * A[5] + C(7, 4) * A[3] + C(5, 4) * A[0];
        this->v[50] = C(2, 5) * A[5] + C(7, 5) * A[3] + C(5, 5) * A[0];
        this->v[51] = C(2, 6) * A[5] + C(7, 6) * A[3] + C(5, 6) * A[0];
        this->v[52] = C(2, 7) * A[5] + C(7, 7) * A[3] + C(5, 7) * A[0];
        this->v[53] = C(2, 8) * A[5] + C(7, 8) * A[3] + C(5, 8) * A[0];
        this->v[54] = C(4, 0) * A[8] + C(0, 0) * A[6] + C(6, 0) * A[2];
        this->v[55] = C(4, 1) * A[8] + C(0, 1) * A[6] + C(6, 1) * A[2];
        this->v[56] = C(4, 2) * A[8] + C(0, 2) * A[6] + C(6, 2) * A[2];
        this->v[57] = C(4, 3) * A[8] + C(0, 3) * A[6] + C(6, 3) * A[2];
        this->v[58] = C(4, 4) * A[8] + C(0, 4) * A[6] + C(6, 4) * A[2];
        this->v[59] = C(4, 5) * A[8] + C(0, 5) * A[6] + C(6, 5) * A[2];
        this->v[60] = C(4, 6) * A[8] + C(0, 6) * A[6] + C(6, 6) * A[2];
        this->v[61] = C(4, 7) * A[8] + C(0, 7) * A[6] + C(6, 7) * A[2];
        this->v[62] = C(4, 8) * A[8] + C(0, 8) * A[6] + C(6, 8) * A[2];
        this->v[63] = C(2, 0) * A[7] + C(5, 0) * A[4] + C(7, 0) * A[1];
        this->v[64] = C(2, 1) * A[7] + C(5, 1) * A[4] + C(7, 1) * A[1];
        this->v[65] = C(2, 2) * A[7] + C(5, 2) * A[4] + C(7, 2) * A[1];
        this->v[66] = C(2, 3) * A[7] + C(5, 3) * A[4] + C(7, 3) * A[1];
        this->v[67] = C(2, 4) * A[7] + C(5, 4) * A[4] + C(7, 4) * A[1];
        this->v[68] = C(2, 5) * A[7] + C(5, 5) * A[4] + C(7, 5) * A[1];
        this->v[69] = C(2, 6) * A[7] + C(5, 6) * A[4] + C(7, 6) * A[1];
        this->v[70] = C(2, 7) * A[7] + C(5, 7) * A[4] + C(7, 7) * A[1];
        this->v[71] = C(2, 8) * A[7] + C(5, 8) * A[4] + C(7, 8) * A[1];
        this->v[72] = C(1, 0) * A[8] + C(3, 0) * A[6] + C(8, 0) * A[2];
        this->v[73] = C(1, 1) * A[8] + C(3, 1) * A[6] + C(8, 1) * A[2];
        this->v[74] = C(1, 2) * A[8] + C(3, 2) * A[6] + C(8, 2) * A[2];
        this->v[75] = C(1, 3) * A[8] + C(3, 3) * A[6] + C(8, 3) * A[2];
        this->v[76] = C(1, 4) * A[8] + C(3, 4) * A[6] + C(8, 4) * A[2];
        this->v[77] = C(1, 5) * A[8] + C(3, 5) * A[6] + C(8, 5) * A[2];
        this->v[78] = C(1, 6) * A[8] + C(3, 6) * A[6] + C(8, 6) * A[2];
        this->v[79] = C(1, 7) * A[8] + C(3, 7) * A[6] + C(8, 7) * A[2];
        this->v[80] = C(1, 8) * A[8] + C(3, 8) * A[6] + C(8, 8) * A[2];
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
        // Expr<T2toT2ResultType,TensorProductRightDerivativeExpr<1u> >

  }  // namespace math

}  // namespace tfel

#endif /* LIB_TFEL_MATH_TENSORPRODUCTRIGHTDERIVATIVEEXPR_HXX */
