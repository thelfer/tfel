/*!
 * \file  include/TFEL/Math/T2toST2/LeftCauchyGreenTensorDerivativeExpr.hxx
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

#ifndef LIB_TFEL_MATH_LEFTCAUCHYGREENTENSORDERIVATIVEEXPR_HXX
#define LIB_TFEL_MATH_LEFTCAUCHYGREENTENSORDERIVATIVEEXPR_HXX

#include <cmath>
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"

namespace tfel {

  namespace math {

    /*!
     * Empty structure allowing partial specialisation
     */
    template <unsigned short N>
    struct LeftCauchyGreenTensorDerivativeExpr {
    };  // end of struct LeftCauchyGreenTensorDerivativeExpr

    /*!
     * Partial specialisation for 1D tensor
     */
    template <typename T2toST2Type>
    struct Expr<T2toST2Type, LeftCauchyGreenTensorDerivativeExpr<1u>>
        : public T2toST2Concept<
              Expr<T2toST2Type, LeftCauchyGreenTensorDerivativeExpr<1u>>>,
          public fsarray<9u, typename T2toST2Traits<T2toST2Type>::NumType> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple check
      TFEL_STATIC_ASSERT(T2toST2Traits<T2toST2Type>::dime == 1u);
      //! a simple alias
      typedef typename T2toST2Traits<T2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template <typename TensorType>
      TFEL_MATH_INLINE2 Expr(const TensorType& F) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                typename T2toST2Traits<T2toST2Type>::NumType>::cond));
        constexpr value_type zero{0};
        this->v[0] = 2 * F[0];
        this->v[1] = this->v[2] = this->v[3] = zero;
        this->v[4] = 2 * F[1];
        this->v[5] = this->v[6] = this->v[7] = zero;
        this->v[8] = 2 * F[2];
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i, const unsigned short j) const {
        return this->v[i * 3 + j];
      }  // end of operator()
    };   // end of struct
         // Expr<T2toST2Type,LeftCauchyGreenTensorDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D tensor
     */
    template <typename T2toST2Type>
    struct Expr<T2toST2Type, LeftCauchyGreenTensorDerivativeExpr<2u>>
        : public T2toST2Concept<
              Expr<T2toST2Type, LeftCauchyGreenTensorDerivativeExpr<2u>>>,
          public fsarray<20u, typename T2toST2Traits<T2toST2Type>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toST2Traits<T2toST2Type>::dime == 2u);
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toST2Traits<T2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template <typename TensorType>
      TFEL_MATH_INLINE2 Expr(const TensorType& F) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                typename T2toST2Traits<T2toST2Type>::NumType>::cond));
        constexpr const auto cste = Cste<value_type>::sqrt2;
        constexpr value_type zero{0};
        this->v[0] = 2 * F[0];
        this->v[1] = this->v[2] = zero;
        this->v[3] = 2 * F[3];
        this->v[4] = this->v[5] = zero;
        this->v[6] = 2 * F[1];
        this->v[7] = this->v[8] = zero;
        this->v[9] = 2 * F[4];
        this->v[10] = this->v[11] = zero;
        this->v[12] = 2 * F[2];
        this->v[13] = this->v[14] = zero;
        this->v[15] = cste * F[4];
        this->v[16] = cste * F[3];
        this->v[17] = zero;
        this->v[18] = cste * F[1];
        this->v[19] = cste * F[0];
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i, const unsigned short j) const {
        return this->v[i * 5 + j];
      }  // end of operator()
    };   // end of struct
         // Expr<T2toST2Type,LeftCauchyGreenTensorDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D tensor
     */
    template <typename T2toST2Type>
    struct Expr<T2toST2Type, LeftCauchyGreenTensorDerivativeExpr<3u>>
        : public T2toST2Concept<
              Expr<T2toST2Type, LeftCauchyGreenTensorDerivativeExpr<3u>>>,
          public fsarray<54u, typename T2toST2Traits<T2toST2Type>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toST2Traits<T2toST2Type>::dime == 3u);
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename T2toST2Traits<T2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template <typename TensorType>
      TFEL_MATH_INLINE2 Expr(const TensorType& F) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<TensorType, TensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime ==
                            TensorTraits<TensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename TensorTraits<TensorType>::NumType,
                typename T2toST2Traits<T2toST2Type>::NumType>::cond));
        constexpr const auto cste = Cste<value_type>::sqrt2;
        constexpr value_type zero{0};
        this->v[0] = 2 * F[0];
        this->v[1] = this->v[2] = zero;
        this->v[3] = 2 * F[3];
        this->v[4] = zero;
        this->v[5] = 2 * F[5];
        this->v[6] = this->v[7] = this->v[8] = this->v[9] = zero;
        this->v[10] = 2 * F[1];
        this->v[11] = this->v[12] = zero;
        this->v[13] = 2 * F[4];
        this->v[14] = this->v[15] = zero;
        this->v[16] = 2 * F[7];
        this->v[17] = this->v[18] = this->v[19] = zero;
        this->v[20] = 2 * F[2];
        this->v[21] = this->v[22] = this->v[23] = zero;
        this->v[24] = 2 * F[6];
        this->v[25] = zero;
        this->v[26] = 2 * F[8];
        this->v[27] = cste * F[4];
        this->v[28] = cste * F[3];
        this->v[29] = zero;
        this->v[30] = cste * F[1];
        this->v[31] = cste * F[0];
        this->v[32] = cste * F[7];
        this->v[33] = zero;
        this->v[34] = cste * F[5];
        this->v[35] = zero;
        this->v[36] = cste * F[6];
        this->v[37] = zero;
        this->v[38] = cste * F[5];
        this->v[39] = cste * F[8];
        this->v[40] = zero;
        this->v[41] = cste * F[2];
        this->v[42] = cste * F[0];
        this->v[43] = zero;
        this->v[44] = cste * F[3];
        this->v[45] = zero;
        this->v[46] = cste * F[8];
        this->v[47] = cste * F[7];
        this->v[48] = zero;
        this->v[49] = cste * F[6];
        this->v[50] = zero;
        this->v[51] = cste * F[4];
        this->v[52] = cste * F[2];
        this->v[53] = cste * F[1];
      }  // end of Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      TFEL_MATH_INLINE const value_type& operator()(
          const unsigned short i, const unsigned short j) const {
        return this->v[i * 9 + j];
      }  // end of operator()
    };   // end of struct
         // Expr<T2toST2Type,LeftCauchyGreenTensorDerivativeExpr<1u> >

  }  // namespace math

}  // namespace tfel

#endif /* LIB_TFEL_MATH_LEFTCAUCHYGREENTENSORDERIVATIVEEXPR_HXX */
