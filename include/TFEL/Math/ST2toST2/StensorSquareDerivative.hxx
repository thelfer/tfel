/*!
 * \file  StensorSquareDerivative.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06 avril 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORSQUAREDERIVATIVE_H
#define LIB_TFEL_MATH_STENSORSQUAREDERIVATIVE_H

#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel::math {

  /*!
   * Empty structure allowing partial specialisation
   */
  template <unsigned short N>
  struct StensorSquareDerivativeExpr {
  };  // end of struct StensorSquareDerivativeExpr

  /*!
   * Partial specialisation for 1D tensor
   */
  template <typename ST2toST2ResultType>
  struct Expr<ST2toST2ResultType, StensorSquareDerivativeExpr<1u>>
      : public ST2toST2Concept<
            Expr<ST2toST2ResultType, StensorSquareDerivativeExpr<1u>>>,
        public array_holder<9u, numeric_type<ST2toST2ResultType>> {
    static_assert(getSpaceDimension<ST2toST2ResultType>() == 1u);
    //! a simple alias
    typedef numeric_type<ST2toST2ResultType> value_type;
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <typename StensorType>
    Expr(const StensorType& B) {
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(getSpaceDimension<ST2toST2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(isAssignableTo<numeric_type<StensorType>,
                                   numeric_type<ST2toST2ResultType>>());
      constexpr value_type zero = value_type(0);
      this->v[0] = 2 * B[0];
      this->v[4] = 2 * B[1];
      this->v[8] = 2 * B[2];
      this->v[1] = this->v[2] = zero;
      this->v[3] = this->v[5] = zero;
      this->v[6] = this->v[7] = zero;
    }  // end of Expr
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     */
    template <typename StensorType, typename ST2toST2Type>
    Expr(const StensorType& s, const ST2toST2Type& C) {
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(implementsST2toST2Concept<ST2toST2Type>());
      static_assert(getSpaceDimension<ST2toST2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() ==
                    getSpaceDimension<StensorType>());
      static_assert(
          isAssignableTo<typename ComputeBinaryResult<
                             numeric_type<StensorType>,
                             numeric_type<ST2toST2Type>, OpMult>::Result,
                         numeric_type<ST2toST2ResultType>>());
      this->v[0] = 2 * C(0, 0) * s(0);
      this->v[1] = 2 * C(0, 1) * s(0);
      this->v[2] = 2 * C(0, 2) * s(0);
      this->v[3] = 2 * C(1, 0) * s(1);
      this->v[4] = 2 * C(1, 1) * s(1);
      this->v[5] = 2 * C(1, 2) * s(1);
      this->v[6] = 2 * C(2, 0) * s(2);
      this->v[7] = 2 * C(2, 1) * s(2);
      this->v[8] = 2 * C(2, 2) * s(2);
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
  };  // end of struct Expr<ST2toST2ResultType,StensorSquareDerivativeExpr<1u> >

  /*!
   * Partial specialisation for 2D tensor
   */
  template <typename ST2toST2ResultType>
  struct Expr<ST2toST2ResultType, StensorSquareDerivativeExpr<2u>>
      : public ST2toST2Concept<
            Expr<ST2toST2ResultType, StensorSquareDerivativeExpr<2u>>>,
        public array_holder<16u, numeric_type<ST2toST2ResultType>> {
    static_assert(getSpaceDimension<ST2toST2ResultType>() == 2u);
    //! a simple alias
    typedef numeric_type<ST2toST2ResultType> value_type;
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <typename StensorType>
    Expr(const StensorType& s) {
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(getSpaceDimension<ST2toST2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(isAssignableTo<numeric_type<StensorType>,
                                   numeric_type<ST2toST2ResultType>>());
      constexpr value_type zero = value_type(0);
      this->v[0] = 2 * s(0);
      this->v[1] = zero;
      this->v[2] = zero;
      this->v[3] = s(3);
      this->v[4] = zero;
      this->v[5] = 2 * s(1);
      this->v[6] = zero;
      this->v[7] = s(3);
      this->v[8] = zero;
      this->v[9] = zero;
      this->v[10] = 2 * s(2);
      this->v[11] = zero;
      this->v[12] = s(3);
      this->v[13] = s(3);
      this->v[14] = zero;
      this->v[15] = s(1) + s(0);
    }  // end of Expr
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     */
    template <typename StensorType, typename ST2toST2Type>
    Expr(const StensorType& s, const ST2toST2Type& C) {
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(implementsST2toST2Concept<ST2toST2Type>());
      static_assert(getSpaceDimension<ST2toST2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() ==
                    getSpaceDimension<StensorType>());
      static_assert(
          isAssignableTo<typename ComputeBinaryResult<
                             numeric_type<StensorType>,
                             numeric_type<ST2toST2Type>, OpMult>::Result,
                         numeric_type<ST2toST2ResultType>>());
      this->v[0] = C(3, 0) * s(3) + 2 * C(0, 0) * s(0);
      this->v[1] = C(3, 1) * s(3) + 2 * C(0, 1) * s(0);
      this->v[2] = C(3, 2) * s(3) + 2 * C(0, 2) * s(0);
      this->v[3] = C(3, 3) * s(3) + 2 * C(0, 3) * s(0);
      this->v[4] = C(3, 0) * s(3) + 2 * C(1, 0) * s(1);
      this->v[5] = C(3, 1) * s(3) + 2 * C(1, 1) * s(1);
      this->v[6] = C(3, 2) * s(3) + 2 * C(1, 2) * s(1);
      this->v[7] = C(3, 3) * s(3) + 2 * C(1, 3) * s(1);
      this->v[8] = 2 * C(2, 0) * s(2);
      this->v[9] = 2 * C(2, 1) * s(2);
      this->v[10] = 2 * C(2, 2) * s(2);
      this->v[11] = 2 * C(2, 3) * s(2);
      this->v[12] = C(1, 0) * s(3) + C(0, 0) * s(3) + C(3, 0) * (s(1) + s(0));
      this->v[13] = C(1, 1) * s(3) + C(0, 1) * s(3) + C(3, 1) * (s(1) + s(0));
      this->v[14] = C(1, 2) * s(3) + C(0, 2) * s(3) + C(3, 2) * (s(1) + s(0));
      this->v[15] = C(1, 3) * s(3) + C(0, 3) * s(3) + C(3, 3) * (s(1) + s(0));
    }  // end of Expr
    /*!
     * \brief access operator
     * \param[in] i : line   index
     * \param[in] j : column index
     */
    const value_type& operator()(const unsigned short i,
                                 const unsigned short j) const {
      return this->v[i * 4 + j];
    }  // end of operator()
    /*!
     * \return the runtime properties of this object
     * In this case, the number of lines and columns
     * are deduced from the template parameter
     */
    RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<ST2toST2ResultType,StensorSquareDerivativeExpr<2u> >

  /*!
   * Partial specialisation for 3D tensor
   */
  template <typename ST2toST2ResultType>
  struct Expr<ST2toST2ResultType, StensorSquareDerivativeExpr<3u>>
      : public ST2toST2Concept<
            Expr<ST2toST2ResultType, StensorSquareDerivativeExpr<3u>>>,
        public array_holder<36u, numeric_type<ST2toST2ResultType>> {
    static_assert(getSpaceDimension<ST2toST2ResultType>() == 3u);
    //! a simple alias
    typedef numeric_type<ST2toST2ResultType> value_type;
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <typename StensorType>
    Expr(const StensorType& s) {
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(getSpaceDimension<ST2toST2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(isAssignableTo<numeric_type<StensorType>,
                                   numeric_type<ST2toST2ResultType>>());
      using real = base_type<value_type>;
      constexpr const auto zero = value_type(0);
      constexpr const auto one_half = real(1) / 2;
      constexpr auto icste = Cste<value_type>::isqrt2;
      this->v[0] = 2 * s(0);
      this->v[1] = zero;
      this->v[2] = zero;
      this->v[3] = s(3);
      this->v[4] = s(4);
      this->v[5] = zero;
      this->v[6] = zero;
      this->v[7] = 2 * s(1);
      this->v[8] = zero;
      this->v[9] = s(3);
      this->v[10] = zero;
      this->v[11] = s(5);
      this->v[12] = zero;
      this->v[13] = zero;
      this->v[14] = 2 * s(2);
      this->v[15] = zero;
      this->v[16] = s(4);
      this->v[17] = s(5);
      this->v[18] = s(3);
      this->v[19] = s(3);
      this->v[20] = zero;
      this->v[21] = (2 * s(1) + 2 * s(0)) * one_half;
      this->v[22] = (s(5)) * icste;
      this->v[23] = (s(4)) * icste;
      this->v[24] = s(4);
      this->v[25] = zero;
      this->v[26] = s(4);
      this->v[27] = s(5) * icste;
      this->v[28] = (2 * s(2) + 2 * s(0)) * one_half;
      this->v[29] = s(3) * icste;
      this->v[30] = zero;
      this->v[31] = s(5);
      this->v[32] = s(5);
      this->v[33] = s(4) * icste;
      this->v[34] = s(3) * icste;
      this->v[35] = (2 * s(2) + 2 * s(1)) * one_half;
    }  // end of Expr
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     */
    template <typename StensorType, typename ST2toST2Type>
    Expr(const StensorType& s, const ST2toST2Type& C) {
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(implementsST2toST2Concept<ST2toST2Type>());
      static_assert(getSpaceDimension<ST2toST2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() ==
                    getSpaceDimension<StensorType>());
      static_assert(
          isAssignableTo<typename ComputeBinaryResult<
                             numeric_type<StensorType>,
                             numeric_type<ST2toST2Type>, OpMult>::Result,
                         numeric_type<ST2toST2ResultType>>());
      using real = base_type<value_type>;
      constexpr const auto one_half = real(1) / real(2);
      constexpr auto cste = Cste<real>::sqrt2;
      this->v[0] = C(4, 0) * s(4) + C(3, 0) * s(3) + 2 * C(0, 0) * s(0);
      this->v[1] = C(4, 1) * s(4) + C(3, 1) * s(3) + 2 * C(0, 1) * s(0);
      this->v[2] = C(4, 2) * s(4) + C(3, 2) * s(3) + 2 * C(0, 2) * s(0);
      this->v[3] = C(4, 3) * s(4) + C(3, 3) * s(3) + 2 * C(0, 3) * s(0);
      this->v[4] = C(4, 4) * s(4) + C(3, 4) * s(3) + 2 * C(0, 4) * s(0);
      this->v[5] = C(4, 5) * s(4) + C(3, 5) * s(3) + 2 * C(0, 5) * s(0);
      this->v[6] = C(5, 0) * s(5) + C(3, 0) * s(3) + 2 * C(1, 0) * s(1);
      this->v[7] = C(5, 1) * s(5) + C(3, 1) * s(3) + 2 * C(1, 1) * s(1);
      this->v[8] = C(5, 2) * s(5) + C(3, 2) * s(3) + 2 * C(1, 2) * s(1);
      this->v[9] = C(5, 3) * s(5) + C(3, 3) * s(3) + 2 * C(1, 3) * s(1);
      this->v[10] = C(5, 4) * s(5) + C(3, 4) * s(3) + 2 * C(1, 4) * s(1);
      this->v[11] = C(5, 5) * s(5) + C(3, 5) * s(3) + 2 * C(1, 5) * s(1);
      this->v[12] = C(5, 0) * s(5) + C(4, 0) * s(4) + 2 * C(2, 0) * s(2);
      this->v[13] = C(5, 1) * s(5) + C(4, 1) * s(4) + 2 * C(2, 1) * s(2);
      this->v[14] = C(5, 2) * s(5) + C(4, 2) * s(4) + 2 * C(2, 2) * s(2);
      this->v[15] = C(5, 3) * s(5) + C(4, 3) * s(4) + 2 * C(2, 3) * s(2);
      this->v[16] = C(5, 4) * s(5) + C(4, 4) * s(4) + 2 * C(2, 4) * s(2);
      this->v[17] = C(5, 5) * s(5) + C(4, 5) * s(4) + 2 * C(2, 5) * s(2);
      this->v[18] = (cste * C(4, 0) * s(5) + cste * C(5, 0) * s(4) +
                     (2 * C(1, 0) + 2 * C(0, 0)) * s(3) + 2 * C(3, 0) * s(1) +
                     2 * C(3, 0) * s(0)) *
                    one_half;
      this->v[19] = (cste * C(4, 1) * s(5) + cste * C(5, 1) * s(4) +
                     (2 * C(1, 1) + 2 * C(0, 1)) * s(3) + 2 * C(3, 1) * s(1) +
                     2 * C(3, 1) * s(0)) *
                    one_half;
      this->v[20] = (cste * C(4, 2) * s(5) + cste * C(5, 2) * s(4) +
                     (2 * C(1, 2) + 2 * C(0, 2)) * s(3) + 2 * C(3, 2) * s(1) +
                     2 * C(3, 2) * s(0)) *
                    one_half;
      this->v[21] = (cste * C(4, 3) * s(5) + cste * C(5, 3) * s(4) +
                     (2 * C(1, 3) + 2 * C(0, 3)) * s(3) + 2 * C(3, 3) * s(1) +
                     2 * C(3, 3) * s(0)) *
                    one_half;
      this->v[22] = (cste * C(4, 4) * s(5) + cste * C(5, 4) * s(4) +
                     (2 * C(1, 4) + 2 * C(0, 4)) * s(3) + 2 * C(3, 4) * s(1) +
                     2 * C(3, 4) * s(0)) *
                    one_half;
      this->v[23] = (cste * C(4, 5) * s(5) + cste * C(5, 5) * s(4) +
                     (2 * C(1, 5) + 2 * C(0, 5)) * s(3) + 2 * C(3, 5) * s(1) +
                     2 * C(3, 5) * s(0)) *
                    one_half;
      this->v[24] =
          (cste * C(3, 0) * s(5) + (2 * C(2, 0) + 2 * C(0, 0)) * s(4) +
           cste * C(5, 0) * s(3) + 2 * C(4, 0) * s(2) + 2 * C(4, 0) * s(0)) *
          one_half;
      this->v[25] =
          (cste * C(3, 1) * s(5) + (2 * C(2, 1) + 2 * C(0, 1)) * s(4) +
           cste * C(5, 1) * s(3) + 2 * C(4, 1) * s(2) + 2 * C(4, 1) * s(0)) *
          one_half;
      this->v[26] =
          (cste * C(3, 2) * s(5) + (2 * C(2, 2) + 2 * C(0, 2)) * s(4) +
           cste * C(5, 2) * s(3) + 2 * C(4, 2) * s(2) + 2 * C(4, 2) * s(0)) *
          one_half;
      this->v[27] =
          (cste * C(3, 3) * s(5) + (2 * C(2, 3) + 2 * C(0, 3)) * s(4) +
           cste * C(5, 3) * s(3) + 2 * C(4, 3) * s(2) + 2 * C(4, 3) * s(0)) *
          one_half;
      this->v[28] =
          (cste * C(3, 4) * s(5) + (2 * C(2, 4) + 2 * C(0, 4)) * s(4) +
           cste * C(5, 4) * s(3) + 2 * C(4, 4) * s(2) + 2 * C(4, 4) * s(0)) *
          one_half;
      this->v[29] =
          (cste * C(3, 5) * s(5) + (2 * C(2, 5) + 2 * C(0, 5)) * s(4) +
           cste * C(5, 5) * s(3) + 2 * C(4, 5) * s(2) + 2 * C(4, 5) * s(0)) *
          one_half;
      this->v[30] =
          ((2 * C(2, 0) + 2 * C(1, 0)) * s(5) + cste * C(3, 0) * s(4) +
           cste * C(4, 0) * s(3) + 2 * C(5, 0) * s(2) + 2 * C(5, 0) * s(1)) *
          one_half;
      this->v[31] =
          ((2 * C(2, 1) + 2 * C(1, 1)) * s(5) + cste * C(3, 1) * s(4) +
           cste * C(4, 1) * s(3) + 2 * C(5, 1) * s(2) + 2 * C(5, 1) * s(1)) *
          one_half;
      this->v[32] =
          ((2 * C(2, 2) + 2 * C(1, 2)) * s(5) + cste * C(3, 2) * s(4) +
           cste * C(4, 2) * s(3) + 2 * C(5, 2) * s(2) + 2 * C(5, 2) * s(1)) *
          one_half;
      this->v[33] =
          ((2 * C(2, 3) + 2 * C(1, 3)) * s(5) + cste * C(3, 3) * s(4) +
           cste * C(4, 3) * s(3) + 2 * C(5, 3) * s(2) + 2 * C(5, 3) * s(1)) *
          one_half;
      this->v[34] =
          ((2 * C(2, 4) + 2 * C(1, 4)) * s(5) + cste * C(3, 4) * s(4) +
           cste * C(4, 4) * s(3) + 2 * C(5, 4) * s(2) + 2 * C(5, 4) * s(1)) *
          one_half;
      this->v[35] =
          ((2 * C(2, 5) + 2 * C(1, 5)) * s(5) + cste * C(3, 5) * s(4) +
           cste * C(4, 5) * s(3) + 2 * C(5, 5) * s(2) + 2 * C(5, 5) * s(1)) *
          one_half;
    }
    /*!
     * \brief access operator
     * \param[in] i : line   index
     * \param[in] j : column index
     */
    const value_type& operator()(const unsigned short i,
                                 const unsigned short j) const {
      return this->v[i * 6 + j];
    }  // end of operator()
    /*!
     * \return the runtime properties of this object
     * In this case, the number of lines and columns
     * are deduced from the template parameter
     */
    RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<ST2toST2ResultType,StensorSquareDerivativeExpr<3u> >

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORSQUAREDERIVATIVE_H */
