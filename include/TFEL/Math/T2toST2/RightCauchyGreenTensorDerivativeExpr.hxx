/*!
 * \file  include/TFEL/Math/T2toST2/RightCauchyGreenTensorDerivativeExpr.hxx
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

#ifndef LIB_TFEL_MATH_RIGHTCAUCHYGREENTENSORDERIVATIVEEXPR_HXX
#define LIB_TFEL_MATH_RIGHTCAUCHYGREENTENSORDERIVATIVEEXPR_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"

namespace tfel::math {

  /*!
   * Empty structure allowing partial specialisation
   */
  template <unsigned short N>
  struct RightCauchyGreenTensorDerivativeExpr {
  };  // end of struct RightCauchyGreenTensorDerivativeExpr

  /*!
   * Partial specialisation for 1D tensor
   */
  template <typename T2toST2Type>
  struct Expr<T2toST2Type, RightCauchyGreenTensorDerivativeExpr<1u>>
      : public T2toST2ConceptBase<
            Expr<T2toST2Type, RightCauchyGreenTensorDerivativeExpr<1u>>>,
        public array_holder<9u, numeric_type<T2toST2Type>> {
    static_assert(getSpaceDimension<T2toST2Type>() == 1u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<T2toST2Type> value_type;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <TensorConcept TensorType>
    TFEL_MATH_INLINE2 Expr(const TensorType& F) {
      static_assert(getSpaceDimension<T2toST2Type>() ==
                    getSpaceDimension<TensorType>());
      static_assert(isAssignableTo<numeric_type<TensorType>,
                                   numeric_type<T2toST2Type>>());
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
    /*!
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct
      // Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> >

  /*!
   * Partial specialisation for 2D tensor
   */
  template <typename T2toST2Type>
  struct Expr<T2toST2Type, RightCauchyGreenTensorDerivativeExpr<2u>>
      : public T2toST2ConceptBase<
            Expr<T2toST2Type, RightCauchyGreenTensorDerivativeExpr<2u>>>,
        public array_holder<20u, numeric_type<T2toST2Type>> {
    static_assert(getSpaceDimension<T2toST2Type>() == 2u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<T2toST2Type> value_type;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <TensorConcept TensorType>
    TFEL_MATH_INLINE2 Expr(const TensorType& F) {
      static_assert(getSpaceDimension<T2toST2Type>() ==
                    getSpaceDimension<TensorType>());
      static_assert(isAssignableTo<numeric_type<TensorType>,
                                   numeric_type<T2toST2Type>>());
      constexpr auto cste = Cste<value_type>::sqrt2;
      constexpr value_type zero{0};
      this->v[0] = 2 * F[0];
      this->v[1] = this->v[2] = this->v[3] = zero;
      this->v[4] = 2 * F[4];
      this->v[5] = zero;
      this->v[6] = 2 * F[1];
      this->v[7] = zero;
      this->v[8] = 2 * F[3];
      this->v[9] = this->v[10] = this->v[11] = zero;
      this->v[12] = 2 * F[2];
      this->v[13] = this->v[14] = zero;
      this->v[15] = cste * F[3];
      this->v[16] = cste * F[4];
      this->v[17] = zero;
      this->v[18] = cste * F[0];
      this->v[19] = cste * F[1];
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
    /*!
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct
      // Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> >

  /*!
   * Partial specialisation for 3D tensor
   */
  template <typename T2toST2Type>
  struct Expr<T2toST2Type, RightCauchyGreenTensorDerivativeExpr<3u>>
      : public T2toST2ConceptBase<
            Expr<T2toST2Type, RightCauchyGreenTensorDerivativeExpr<3u>>>,
        public array_holder<54u, numeric_type<T2toST2Type>> {
    static_assert(getSpaceDimension<T2toST2Type>() == 3u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<T2toST2Type> value_type;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <TensorConcept TensorType>
    TFEL_MATH_INLINE2 Expr(const TensorType& F) {
      static_assert(getSpaceDimension<T2toST2Type>() ==
                    getSpaceDimension<TensorType>());
      static_assert(isAssignableTo<numeric_type<TensorType>,
                                   numeric_type<T2toST2Type>>());
      constexpr auto cste = Cste<value_type>::sqrt2;
      constexpr value_type zero{0};
      this->v[0] = 2 * F[0];
      this->v[1] = this->v[2] = this->v[3] = zero;
      this->v[4] = 2 * F[4];
      this->v[5] = zero;
      this->v[6] = 2 * F[6];
      this->v[7] = this->v[8] = this->v[9] = zero;
      this->v[10] = 2 * F[1];
      this->v[11] = zero;
      this->v[12] = 2 * F[3];
      this->v[13] = this->v[14] = this->v[15] = this->v[16] = zero;
      this->v[17] = 2 * F[8];
      this->v[18] = this->v[19] = zero;
      this->v[20] = 2 * F[2];
      this->v[21] = this->v[22] = zero;
      this->v[23] = 2 * F[5];
      this->v[24] = zero;
      this->v[25] = 2 * F[7];
      this->v[26] = zero;
      this->v[27] = cste * F[3];
      this->v[28] = cste * F[4];
      this->v[29] = zero;
      this->v[30] = cste * F[0];
      this->v[31] = cste * F[1];
      this->v[32] = zero;
      this->v[33] = cste * F[8];
      this->v[34] = zero;
      this->v[35] = cste * F[6];
      this->v[36] = cste * F[5];
      this->v[37] = zero;
      this->v[38] = cste * F[6];
      this->v[39] = zero;
      this->v[40] = cste * F[7];
      this->v[41] = cste * F[0];
      this->v[42] = cste * F[2];
      this->v[43] = cste * F[4];
      this->v[44] = zero;
      this->v[45] = zero;
      this->v[46] = cste * F[7];
      this->v[47] = cste * F[8];
      this->v[48] = cste * F[5];
      this->v[49] = zero;
      this->v[50] = cste * F[3];
      this->v[51] = zero;
      this->v[52] = cste * F[1];
      this->v[53] = cste * F[2];
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
    /*!
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct
      // Expr<T2toST2Type,RightCauchyGreenTensorDerivativeExpr<1u> >

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_RIGHTCAUCHYGREENTENSORDERIVATIVEEXPR_HXX */
