/*!
 * \file  include/TFEL/Math/ST2toT2/StensorProductLeftDerivativeExpr.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 04 juin 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_HXX
#define LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_HXX

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel::math {

  //! \brief empty structure allowing partial specialisation
  template <unsigned short N>
  struct StensorProductLeftDerivativeExpr {
  };  // end of struct StensorProductLeftDerivativeExpr

  //! \brief partial specialisation in 2D
  template <typename ST2toT2ResultType>
  struct Expr<ST2toT2ResultType, StensorProductLeftDerivativeExpr<1u>>
      : public ST2toT2ConceptBase<
            Expr<ST2toT2ResultType, StensorProductLeftDerivativeExpr<1u>>>,
        public array_holder<9u, numeric_type<ST2toT2ResultType>> {
    static_assert(getSpaceDimension<ST2toT2ResultType>() == 1u);
    //! a simple alias
    typedef numeric_type<ST2toT2ResultType> value_type;
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <StensorConcept StensorType>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& b) noexcept {
      static_assert(getSpaceDimension<ST2toT2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(isAssignableTo<numeric_type<StensorType>,
                                   numeric_type<ST2toT2ResultType>>());
      const value_type zero = value_type(0);
      this->v[0] = b(0);
      this->v[4] = b(1);
      this->v[8] = b(2);
      this->v[1] = this->v[2] = zero;
      this->v[3] = this->v[5] = zero;
      this->v[6] = this->v[7] = zero;
    }  // end of Expr
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     */
    template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& b,
                                    const ST2toST2Type& C) noexcept {
      static_assert(getSpaceDimension<ST2toT2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() ==
                    getSpaceDimension<StensorType>());
      static_assert(
          isAssignableTo<typename ComputeBinaryResult<
                             numeric_type<StensorType>,
                             numeric_type<ST2toST2Type>, OpMult>::Result,
                         numeric_type<ST2toT2ResultType>>());
      this->v[0] = C(0, 0) * b(0);
      this->v[1] = C(0, 1) * b(0);
      this->v[2] = C(0, 2) * b(0);
      this->v[3] = C(1, 0) * b(1);
      this->v[4] = C(1, 1) * b(1);
      this->v[5] = C(1, 2) * b(1);
      this->v[6] = C(2, 0) * b(2);
      this->v[7] = C(2, 1) * b(2);
      this->v[8] = C(2, 2) * b(2);
    }  // end of Expr
    /*!
     * \brief access operator
     * \param[in] i : line   index
     * \param[in] j : column index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator()(
        const unsigned short i, const unsigned short j) const noexcept {
      return this->v[i * 3 + j];
    }  // end of operator()
    /*!
     * \return the runtime properties of this object
     * In this case, the number of lines and columns
     * are deduced from the template parameter
     */
    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }
  };  // end of Expr<ST2toT2ResultType,StensorProductLeftDerivativeExpr<1u>>

  //! \brief partial specialisation in 2D
  template <typename ST2toT2ResultType>
  struct Expr<ST2toT2ResultType, StensorProductLeftDerivativeExpr<2u>>
      : public ST2toT2ConceptBase<
            Expr<ST2toT2ResultType, StensorProductLeftDerivativeExpr<2u>>>,
        public array_holder<20u, numeric_type<ST2toT2ResultType>> {
    static_assert(getSpaceDimension<ST2toT2ResultType>() == 2u);
    //! a simple alias
    typedef numeric_type<ST2toT2ResultType> value_type;
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <StensorConcept StensorType>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& b) noexcept {
      static_assert(getSpaceDimension<ST2toT2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(isAssignableTo<numeric_type<StensorType>,
                                   numeric_type<ST2toT2ResultType>>());
      using real = base_type<ST2toT2ResultType>;
      constexpr auto icste = Cste<real>::isqrt2;
      const value_type zero = value_type(0);
      const real one_half = real(1) / 2;
      this->v[0u] = b(0);
      this->v[1u] = zero;
      this->v[2u] = zero;
      this->v[3u] = b(3) * one_half;
      this->v[4u] = zero;
      this->v[5u] = b(1);
      this->v[6u] = zero;
      this->v[7u] = b(3) * one_half;
      this->v[8u] = zero;
      this->v[9u] = zero;
      this->v[10u] = b(2);
      this->v[11u] = zero;
      this->v[12u] = b(3) * icste;
      this->v[13u] = zero;
      this->v[14u] = zero;
      this->v[15u] = b(1) * icste;
      this->v[16u] = zero;
      this->v[17u] = b(3) * icste;
      this->v[18u] = zero;
      this->v[19u] = b(0) * icste;
    }  // end of Expr
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     */
    template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& b,
                                    const ST2toST2Type& C) noexcept {
      static_assert(getSpaceDimension<ST2toT2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() ==
                    getSpaceDimension<StensorType>());
      static_assert(
          isAssignableTo<typename ComputeBinaryResult<
                             numeric_type<StensorType>,
                             numeric_type<ST2toST2Type>, OpMult>::Result,
                         numeric_type<ST2toT2ResultType>>());
      using real = base_type<ST2toT2ResultType>;
      constexpr auto icste = Cste<real>::isqrt2;
      const real one_half = real(1) / 2;
      this->v[0u] = (C(3, 0) * b(3)) * one_half + C(0, 0) * b(0);
      this->v[1u] = (C(3, 1) * b(3)) * one_half + C(0, 1) * b(0);
      this->v[2u] = (C(3, 2) * b(3)) * one_half + C(0, 2) * b(0);
      this->v[3u] = (C(3, 3) * b(3)) * one_half + C(0, 3) * b(0);
      this->v[4u] = (C(3, 0) * b(3)) * one_half + C(1, 0) * b(1);
      this->v[5u] = (C(3, 1) * b(3)) * one_half + C(1, 1) * b(1);
      this->v[6u] = (C(3, 2) * b(3)) * one_half + C(1, 2) * b(1);
      this->v[7u] = (C(3, 3) * b(3)) * one_half + C(1, 3) * b(1);
      this->v[8u] = C(2, 0) * b(2);
      this->v[9u] = C(2, 1) * b(2);
      this->v[10u] = C(2, 2) * b(2);
      this->v[11u] = C(2, 3) * b(2);
      this->v[12u] = (C(0, 0) * b(3) + C(3, 0) * b(1)) * icste;
      this->v[13u] = (C(0, 1) * b(3) + C(3, 1) * b(1)) * icste;
      this->v[14u] = (C(0, 2) * b(3) + C(3, 2) * b(1)) * icste;
      this->v[15u] = (C(0, 3) * b(3) + C(3, 3) * b(1)) * icste;
      this->v[16u] = (C(1, 0) * b(3) + C(3, 0) * b(0)) * icste;
      this->v[17u] = (C(1, 1) * b(3) + C(3, 1) * b(0)) * icste;
      this->v[18u] = (C(1, 2) * b(3) + C(3, 2) * b(0)) * icste;
      this->v[19u] = (C(1, 3) * b(3) + C(3, 3) * b(0)) * icste;
    }  // end of Expr
    /*!
     * \brief access operator
     * \param[in] i : line   index
     * \param[in] j : column index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator()(
        const unsigned short i, const unsigned short j) const noexcept {
      return this->v[i * 4 + j];
    }  // end of operator()
    /*!
     * \return the runtime properties of this object
     * In this case, the number of lines and columns
     * are deduced from the template parameter
     */
    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }
  };  // end of Expr<ST2toT2ResultType,StensorProductLeftDerivativeExpr<2u>>

  //! \brief partial specialisation in 3D
  template <typename ST2toT2ResultType>
  struct Expr<ST2toT2ResultType, StensorProductLeftDerivativeExpr<3u>>
      : public ST2toT2ConceptBase<
            Expr<ST2toT2ResultType, StensorProductLeftDerivativeExpr<3u>>>,
        public array_holder<54u, numeric_type<ST2toT2ResultType>> {
    static_assert(getSpaceDimension<ST2toT2ResultType>() == 3u);
    //! a simple alias
    typedef numeric_type<ST2toT2ResultType> value_type;
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \param[in] B : second tensor of the product
     */
    template <StensorConcept StensorType>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& b) noexcept {
      static_assert(getSpaceDimension<ST2toT2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(isAssignableTo<numeric_type<StensorType>,
                                   numeric_type<ST2toT2ResultType>>());
      using real = base_type<ST2toT2ResultType>;
      constexpr auto icste = Cste<real>::isqrt2;
      const value_type zero = value_type(0);
      const real one_half = real(1) / 2;
      this->v[0u] = b(0);
      this->v[1u] = zero;
      this->v[2u] = zero;
      this->v[3u] = b(3) * one_half;
      this->v[4u] = b(4) * one_half;
      this->v[5u] = zero;
      this->v[6u] = zero;
      this->v[7u] = b(1);
      this->v[8u] = zero;
      this->v[9u] = b(3) * one_half;
      this->v[10u] = zero;
      this->v[11u] = b(5) * one_half;
      this->v[12u] = zero;
      this->v[13u] = zero;
      this->v[14u] = b(2);
      this->v[15u] = zero;
      this->v[16u] = b(4) * one_half;
      this->v[17u] = b(5) * one_half;
      this->v[18u] = b(3) * icste;
      this->v[19u] = zero;
      this->v[20u] = zero;
      this->v[21u] = b(1) * icste;
      this->v[22u] = b(5) * one_half;
      this->v[23u] = zero;
      this->v[24u] = zero;
      this->v[25u] = b(3) * icste;
      this->v[26u] = zero;
      this->v[27u] = b(0) * icste;
      this->v[28u] = zero;
      this->v[29u] = b(4) * one_half;
      this->v[30u] = b(4) * icste;
      this->v[31u] = zero;
      this->v[32u] = zero;
      this->v[33u] = b(5) * one_half;
      this->v[34u] = b(2) * icste;
      this->v[35u] = zero;
      this->v[36u] = zero;
      this->v[37u] = zero;
      this->v[38u] = b(4) * icste;
      this->v[39u] = zero;
      this->v[40u] = b(0) * icste;
      this->v[41u] = b(3) * one_half;
      this->v[42u] = zero;
      this->v[43u] = b(5) * icste;
      this->v[44u] = zero;
      this->v[45u] = b(4) * one_half;
      this->v[46u] = zero;
      this->v[47u] = b(2) * icste;
      this->v[48u] = zero;
      this->v[49u] = zero;
      this->v[50u] = b(5) * icste;
      this->v[51u] = zero;
      this->v[52u] = b(3) * one_half;
      this->v[53u] = b(1) * icste;
    }  // end of Expr
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     */
    template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& b,
                                    const ST2toST2Type& C) noexcept {
      static_assert(getSpaceDimension<ST2toT2ResultType>() ==
                    getSpaceDimension<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() ==
                    getSpaceDimension<StensorType>());
      static_assert(
          isAssignableTo<typename ComputeBinaryResult<
                             numeric_type<StensorType>,
                             numeric_type<ST2toST2Type>, OpMult>::Result,
                         numeric_type<ST2toT2ResultType>>());
      using real = base_type<ST2toT2ResultType>;
      constexpr real one_half = real(1) / 2;
      constexpr auto icste = Cste<real>::isqrt2;
      this->v[0u] =
          (C(4, 0) * b(4) + C(3, 0) * b(3)) * one_half + C(0, 0) * b(0);
      this->v[1u] =
          (C(4, 1) * b(4) + C(3, 1) * b(3)) * one_half + C(0, 1) * b(0);
      this->v[2u] =
          (C(4, 2) * b(4) + C(3, 2) * b(3)) * one_half + C(0, 2) * b(0);
      this->v[3u] =
          (C(4, 3) * b(4) + C(3, 3) * b(3)) * one_half + C(0, 3) * b(0);
      this->v[4u] =
          (C(4, 4) * b(4) + C(3, 4) * b(3)) * one_half + C(0, 4) * b(0);
      this->v[5u] =
          (C(4, 5) * b(4) + C(3, 5) * b(3)) * one_half + C(0, 5) * b(0);
      this->v[6u] =
          (C(5, 0) * b(5) + C(3, 0) * b(3)) * one_half + C(1, 0) * b(1);
      this->v[7u] =
          (C(5, 1) * b(5) + C(3, 1) * b(3)) * one_half + C(1, 1) * b(1);
      this->v[8u] =
          (C(5, 2) * b(5) + C(3, 2) * b(3)) * one_half + C(1, 2) * b(1);
      this->v[9u] =
          (C(5, 3) * b(5) + C(3, 3) * b(3)) * one_half + C(1, 3) * b(1);
      this->v[10u] =
          (C(5, 4) * b(5) + C(3, 4) * b(3)) * one_half + C(1, 4) * b(1);
      this->v[11u] =
          (C(5, 5) * b(5) + C(3, 5) * b(3)) * one_half + C(1, 5) * b(1);
      this->v[12u] =
          (C(5, 0) * b(5) + C(4, 0) * b(4)) * one_half + C(2, 0) * b(2);
      this->v[13u] =
          (C(5, 1) * b(5) + C(4, 1) * b(4)) * one_half + C(2, 1) * b(2);
      this->v[14u] =
          (C(5, 2) * b(5) + C(4, 2) * b(4)) * one_half + C(2, 2) * b(2);
      this->v[15u] =
          (C(5, 3) * b(5) + C(4, 3) * b(4)) * one_half + C(2, 3) * b(2);
      this->v[16u] =
          (C(5, 4) * b(5) + C(4, 4) * b(4)) * one_half + C(2, 4) * b(2);
      this->v[17u] =
          (C(5, 5) * b(5) + C(4, 5) * b(4)) * one_half + C(2, 5) * b(2);
      this->v[18u] = (C(4, 0) * b(5) + C(0, 0) * b(3) + C(3, 0) * b(1)) * icste;
      this->v[19u] = (C(4, 1) * b(5)) * one_half +
                     (C(0, 1) * b(3) + C(3, 1) * b(1)) * icste;
      this->v[20u] = (C(4, 2) * b(5)) * one_half +
                     (C(0, 2) * b(3) + C(3, 2) * b(1)) * icste;
      this->v[21u] = (C(4, 3) * b(5)) * one_half +
                     (C(0, 3) * b(3) + C(3, 3) * b(1)) * icste;
      this->v[22u] = (C(4, 4) * b(5)) * one_half +
                     (C(0, 4) * b(3) + C(3, 4) * b(1)) * icste;
      this->v[23u] = (C(4, 5) * b(5)) * one_half +
                     (C(0, 5) * b(3) + C(3, 5) * b(1)) * icste;
      this->v[24u] = (C(5, 0) * b(4)) * one_half +
                     (C(1, 0) * b(3) + C(3, 0) * b(0)) * icste;
      this->v[25u] = (C(5, 1) * b(4)) * one_half +
                     (C(1, 1) * b(3) + C(3, 1) * b(0)) * icste;
      this->v[26u] = (C(5, 2) * b(4)) * one_half +
                     (C(1, 2) * b(3) + C(3, 2) * b(0)) * icste;
      this->v[27u] = (C(5, 3) * b(4)) * one_half +
                     (C(1, 3) * b(3) + C(3, 3) * b(0)) * icste;
      this->v[28u] = (C(5, 4) * b(4)) * one_half +
                     (C(1, 4) * b(3) + C(3, 4) * b(0)) * icste;
      this->v[29u] = (C(5, 5) * b(4)) * one_half +
                     (C(1, 5) * b(3) + C(3, 5) * b(0)) * icste;
      this->v[30u] = (C(3, 0) * b(5)) * one_half +
                     (C(0, 0) * b(4) + C(4, 0) * b(2)) * icste;
      this->v[31u] = (C(3, 1) * b(5)) * one_half +
                     (C(0, 1) * b(4) + C(4, 1) * b(2)) * icste;
      this->v[32u] = (C(3, 2) * b(5)) * one_half +
                     (C(0, 2) * b(4) + C(4, 2) * b(2)) * icste;
      this->v[33u] = (C(3, 3) * b(5)) * one_half +
                     (C(0, 3) * b(4) + C(4, 3) * b(2)) * icste;
      this->v[34u] = (C(3, 4) * b(5)) * one_half +
                     (C(0, 4) * b(4) + C(4, 4) * b(2)) * icste;
      this->v[35u] = (C(3, 5) * b(5)) * one_half +
                     (C(0, 5) * b(4) + C(4, 5) * b(2)) * icste;
      this->v[36u] = (C(5, 0) * b(3)) * one_half +
                     (C(2, 0) * b(4) + C(4, 0) * b(0)) * icste;
      this->v[37u] = (C(5, 1) * b(3)) * one_half +
                     (C(2, 1) * b(4) + C(4, 1) * b(0)) * icste;
      this->v[38u] = (C(5, 2) * b(3)) * one_half +
                     (C(2, 2) * b(4) + C(4, 2) * b(0)) * icste;
      this->v[39u] = (C(5, 3) * b(3)) * one_half +
                     (C(2, 3) * b(4) + C(4, 3) * b(0)) * icste;
      this->v[40u] = (C(5, 4) * b(3)) * one_half +
                     (C(2, 4) * b(4) + C(4, 4) * b(0)) * icste;
      this->v[41u] = (C(5, 5) * b(3)) * one_half +
                     (C(2, 5) * b(4) + C(4, 5) * b(0)) * icste;
      this->v[42u] = (C(3, 0) * b(4)) * one_half +
                     (C(1, 0) * b(5) + C(5, 0) * b(2)) * icste;
      this->v[43u] = (C(3, 1) * b(4)) * one_half +
                     (C(1, 1) * b(5) + C(5, 1) * b(2)) * icste;
      this->v[44u] = (C(3, 2) * b(4)) * one_half +
                     (C(1, 2) * b(5) + C(5, 2) * b(2)) * icste;
      this->v[45u] = (C(3, 3) * b(4)) * one_half +
                     (C(1, 3) * b(5) + C(5, 3) * b(2)) * icste;
      this->v[46u] = (C(3, 4) * b(4)) * one_half +
                     (C(1, 4) * b(5) + C(5, 4) * b(2)) * icste;
      this->v[47u] = (C(3, 5) * b(4)) * one_half +
                     (C(1, 5) * b(5) + C(5, 5) * b(2)) * icste;
      this->v[48u] = (C(4, 0) * b(3)) * one_half +
                     (C(2, 0) * b(5) + C(5, 0) * b(1)) * icste;
      this->v[49u] = (C(4, 1) * b(3)) * one_half +
                     (C(2, 1) * b(5) + C(5, 1) * b(1)) * icste;
      this->v[50u] = (C(4, 2) * b(3)) * one_half +
                     (C(2, 2) * b(5) + C(5, 2) * b(1)) * icste;
      this->v[51u] = (C(4, 3) * b(3)) * one_half +
                     (C(2, 3) * b(5) + C(5, 3) * b(1)) * icste;
      this->v[52u] = (C(4, 4) * b(3)) * one_half +
                     (C(2, 4) * b(5) + C(5, 4) * b(1)) * icste;
      this->v[53u] = (C(4, 5) * b(3)) * one_half +
                     (C(2, 5) * b(5) + C(5, 5) * b(1)) * icste;
    }
    /*!
     * \brief access operator
     * \param[in] i : line   index
     * \param[in] j : column index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator()(
        const unsigned short i, const unsigned short j) const noexcept {
      return this->v[i * 6 + j];
    }  // end of operator()
    /*!
     * \return the runtime properties of this object
     * In this case, the number of lines and columns
     * are deduced from the template parameter
     */
    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }
  };  // end of Expr<ST2toT2ResultType,StensorProductLeftDerivativeExpr<3u>>

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_H */
