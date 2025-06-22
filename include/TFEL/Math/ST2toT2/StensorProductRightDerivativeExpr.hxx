/*!
 * \file   include/TFEL/Math/ST2toT2/StensorProductRightDerivativeExpr.hxx
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

#ifndef LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_HXX
#define LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_HXX

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"

namespace tfel {

  namespace math {

    /*!
     * Empty structure allowing partial specialisation
     */
    template <unsigned short N>
    struct StensorProductRightDerivativeExpr {
    };  // end of struct StensorProductRightDerivativeExpr

    /*!
     * Partial specialisation for 1D stensor
     */
    template <typename ST2toT2ResultType>
    struct Expr<ST2toT2ResultType, StensorProductRightDerivativeExpr<1u>>
        : public ST2toT2Concept<
              Expr<ST2toT2ResultType, StensorProductRightDerivativeExpr<1u>>>,
          public fsarray<9u,
                         typename ST2toT2Traits<ST2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toT2Traits<ST2toT2ResultType>::dime == 1u);
      //! a simple alias
      typedef typename ST2toT2Traits<ST2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] a : first stensor of the product
       */
      template <typename StensorType>
      Expr(const StensorType& a) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                typename ST2toT2Traits<ST2toT2ResultType>::NumType>::cond));
        const value_type zero = value_type(0);
        this->v[0] = a(0);
        this->v[4] = a(1);
        this->v[8] = a(2);
        this->v[1] = this->v[2] = zero;
        this->v[3] = this->v[5] = zero;
        this->v[6] = this->v[7] = zero;
      }  // end of Expr
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template <typename StensorType, typename ST2toST2Type>
      Expr(const StensorType& a, const ST2toST2Type& C) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename ST2toST2Traits<ST2toST2Type>::NumType,
                    OpMult>::Result,
                typename ST2toT2Traits<ST2toT2ResultType>::NumType>::cond));
        this->v[0] = C(0, 0) * a(0);
        this->v[1] = C(0, 1) * a(0);
        this->v[2] = C(0, 2) * a(0);
        this->v[3] = C(1, 0) * a(1);
        this->v[4] = C(1, 1) * a(1);
        this->v[5] = C(1, 2) * a(1);
        this->v[6] = C(2, 0) * a(2);
        this->v[7] = C(2, 1) * a(2);
        this->v[8] = C(2, 2) * a(2);
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
        // Expr<ST2toT2ResultType,StensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D stensor
     */
    template <typename ST2toT2ResultType>
    struct Expr<ST2toT2ResultType, StensorProductRightDerivativeExpr<2u>>
        : public ST2toT2Concept<
              Expr<ST2toT2ResultType, StensorProductRightDerivativeExpr<2u>>>,
          public fsarray<20u,
                         typename ST2toT2Traits<ST2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toT2Traits<ST2toT2ResultType>::dime == 2u);
      //! a simple alias
      typedef typename ST2toT2Traits<ST2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] a : first stensor of the product
       */
      template <typename StensorType>
      Expr(const StensorType& a) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                typename ST2toT2Traits<ST2toT2ResultType>::NumType>::cond));
        using tfel::typetraits::BaseType;
        typedef typename BaseType<value_type>::type real;
        constexpr const auto icste = Cste<real>::isqrt2;
        constexpr const auto zero = value_type(0);
        TFEL_CONSTEXPR const auto one_half = real(1) / 2;
        this->v[0u] = a(0);
        this->v[1u] = zero;
        this->v[2u] = zero;
        this->v[3u] = a(3) * one_half;
        this->v[4u] = zero;
        this->v[5u] = a(1);
        this->v[6u] = zero;
        this->v[7u] = a(3) * one_half;
        this->v[8u] = zero;
        this->v[9u] = zero;
        this->v[10u] = a(2);
        this->v[11u] = zero;
        this->v[12u] = zero;
        this->v[13u] = a(3) * icste;
        this->v[14u] = zero;
        this->v[15u] = a(0) * icste;
        this->v[16u] = a(3) * icste;
        this->v[17u] = zero;
        this->v[18u] = zero;
        this->v[19u] = a(1) * icste;
      }  // end of Expr
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template <typename StensorType, typename ST2toST2Type>
      Expr(const StensorType& a, const ST2toST2Type& C) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename ST2toST2Traits<ST2toST2Type>::NumType,
                    OpMult>::Result,
                typename ST2toT2Traits<ST2toT2ResultType>::NumType>::cond));
        using tfel::typetraits::BaseType;
        typedef typename BaseType<value_type>::type real;
        constexpr const auto icste = Cste<real>::isqrt2;
        TFEL_CONSTEXPR const auto one_half = real(1) / 2;
        this->v[0u] = C(3, 0) * a(3) * one_half + C(0, 0) * a(0);
        this->v[1u] = C(3, 1) * a(3) * one_half + C(0, 1) * a(0);
        this->v[2u] = C(3, 2) * a(3) * one_half + C(0, 2) * a(0);
        this->v[3u] = C(3, 3) * a(3) * one_half + C(0, 3) * a(0);
        this->v[4u] = C(3, 0) * a(3) * one_half + C(1, 0) * a(1);
        this->v[5u] = C(3, 1) * a(3) * one_half + C(1, 1) * a(1);
        this->v[6u] = C(3, 2) * a(3) * one_half + C(1, 2) * a(1);
        this->v[7u] = C(3, 3) * a(3) * one_half + C(1, 3) * a(1);
        this->v[8u] = C(2, 0) * a(2);
        this->v[9u] = C(2, 1) * a(2);
        this->v[10u] = C(2, 2) * a(2);
        this->v[11u] = C(2, 3) * a(2);
        this->v[12u] = (C(1, 0) * a(3) + C(3, 0) * a(0)) * icste;
        this->v[13u] = (C(1, 1) * a(3) + C(3, 1) * a(0)) * icste;
        this->v[14u] = (C(1, 2) * a(3) + C(3, 2) * a(0)) * icste;
        this->v[15u] = (C(1, 3) * a(3) + C(3, 3) * a(0)) * icste;
        this->v[16u] = (C(0, 0) * a(3) + C(3, 0) * a(1)) * icste;
        this->v[17u] = (C(0, 1) * a(3) + C(3, 1) * a(1)) * icste;
        this->v[18u] = (C(0, 2) * a(3) + C(3, 2) * a(1)) * icste;
        this->v[19u] = (C(0, 3) * a(3) + C(3, 3) * a(1)) * icste;
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
    };  // end of struct
        // Expr<ST2toT2ResultType,StensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D stensor
     */
    template <typename ST2toT2ResultType>
    struct Expr<ST2toT2ResultType, StensorProductRightDerivativeExpr<3u>>
        : public ST2toT2Concept<
              Expr<ST2toT2ResultType, StensorProductRightDerivativeExpr<3u>>>,
          public fsarray<54u,
                         typename ST2toT2Traits<ST2toT2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toT2Traits<ST2toT2ResultType>::dime == 3u);
      //! a simple alias
      typedef typename ST2toT2Traits<ST2toT2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] a : first stensor of the product
       */
      template <typename StensorType>
      Expr(const StensorType& a) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename StensorTraits<StensorType>::NumType,
                typename ST2toT2Traits<ST2toT2ResultType>::NumType>::cond));
        using tfel::typetraits::BaseType;
        typedef typename BaseType<value_type>::type real;
        constexpr const auto icste = Cste<real>::isqrt2;
        constexpr const auto zero = value_type(0);
        TFEL_CONSTEXPR const auto one_half = real(1) / 2;
        this->v[0u] = a(0);
        this->v[1u] = zero;
        this->v[2u] = zero;
        this->v[3u] = a(3) * one_half;
        this->v[4u] = a(4) * one_half;
        this->v[5u] = zero;
        this->v[6u] = zero;
        this->v[7u] = a(1);
        this->v[8u] = zero;
        this->v[9u] = a(3) * one_half;
        this->v[10u] = zero;
        this->v[11u] = a(5) * one_half;
        this->v[12u] = zero;
        this->v[13u] = zero;
        this->v[14u] = a(2);
        this->v[15u] = zero;
        this->v[16u] = a(4) * one_half;
        this->v[17u] = a(5) * one_half;
        this->v[18u] = zero;
        this->v[19u] = a(3) * icste;
        this->v[20u] = zero;
        this->v[21u] = a(0) * icste;
        this->v[22u] = zero;
        this->v[23u] = a(4) * one_half;
        this->v[24u] = a(3) * icste;
        this->v[25u] = zero;
        this->v[26u] = zero;
        this->v[27u] = a(1) * icste;
        this->v[28u] = a(5) * one_half;
        this->v[29u] = zero;
        this->v[30u] = zero;
        this->v[31u] = zero;
        this->v[32u] = a(4) * icste;
        this->v[33u] = zero;
        this->v[34u] = a(0) * icste;
        this->v[35u] = a(3) * one_half;
        this->v[36u] = a(4) * icste;
        this->v[37u] = zero;
        this->v[38u] = zero;
        this->v[39u] = a(5) * one_half;
        this->v[40u] = a(2) * icste;
        this->v[41u] = zero;
        this->v[42u] = zero;
        this->v[43u] = zero;
        this->v[44u] = a(5) * icste;
        this->v[45u] = zero;
        this->v[46u] = a(3) * one_half;
        this->v[47u] = a(1) * icste;
        this->v[48u] = zero;
        this->v[49u] = a(5) * icste;
        this->v[50u] = zero;
        this->v[51u] = a(4) * one_half;
        this->v[52u] = zero;
        this->v[53u] = a(2) * icste;
      }  // end of Expr
      /*!
       * \param[in] a : first stensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template <typename StensorType, typename ST2toST2Type>
      Expr(const StensorType& a, const ST2toST2Type& C) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<StensorType, StensorConcept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime ==
                            StensorTraits<StensorType>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    typename StensorTraits<StensorType>::NumType,
                    typename ST2toST2Traits<ST2toST2Type>::NumType,
                    OpMult>::Result,
                typename ST2toT2Traits<ST2toT2ResultType>::NumType>::cond));
        using tfel::typetraits::BaseType;
        typedef typename BaseType<value_type>::type real;
        constexpr const auto icste = Cste<real>::isqrt2;
        TFEL_CONSTEXPR const auto one_half = real(1) / 2;
        this->v[0u] =
            (C(4, 0) * a(4) + C(3, 0) * a(3)) * one_half + C(0, 0) * a(0);
        this->v[1u] =
            (C(4, 1) * a(4) + C(3, 1) * a(3)) * one_half + C(0, 1) * a(0);
        this->v[2u] =
            (C(4, 2) * a(4) + C(3, 2) * a(3)) * one_half + C(0, 2) * a(0);
        this->v[3u] =
            (C(4, 3) * a(4) + C(3, 3) * a(3)) * one_half + C(0, 3) * a(0);
        this->v[4u] =
            (C(4, 4) * a(4) + C(3, 4) * a(3)) * one_half + C(0, 4) * a(0);
        this->v[5u] =
            (C(4, 5) * a(4) + C(3, 5) * a(3)) * one_half + C(0, 5) * a(0);
        this->v[6u] =
            (C(5, 0) * a(5) + C(3, 0) * a(3)) * one_half + C(1, 0) * a(1);
        this->v[7u] =
            (C(5, 1) * a(5) + C(3, 1) * a(3)) * one_half + C(1, 1) * a(1);
        this->v[8u] =
            (C(5, 2) * a(5) + C(3, 2) * a(3)) * one_half + C(1, 2) * a(1);
        this->v[9u] =
            (C(5, 3) * a(5) + C(3, 3) * a(3)) * one_half + C(1, 3) * a(1);
        this->v[10u] =
            (C(5, 4) * a(5) + C(3, 4) * a(3)) * one_half + C(1, 4) * a(1);
        this->v[11u] =
            (C(5, 5) * a(5) + C(3, 5) * a(3)) * one_half + C(1, 5) * a(1);
        this->v[12u] =
            (C(5, 0) * a(5) + C(4, 0) * a(4)) * one_half + C(2, 0) * a(2);
        this->v[13u] =
            (C(5, 1) * a(5) + C(4, 1) * a(4)) * one_half + C(2, 1) * a(2);
        this->v[14u] =
            (C(5, 2) * a(5) + C(4, 2) * a(4)) * one_half + C(2, 2) * a(2);
        this->v[15u] =
            (C(5, 3) * a(5) + C(4, 3) * a(4)) * one_half + C(2, 3) * a(2);
        this->v[16u] =
            (C(5, 4) * a(5) + C(4, 4) * a(4)) * one_half + C(2, 4) * a(2);
        this->v[17u] =
            (C(5, 5) * a(5) + C(4, 5) * a(4)) * one_half + C(2, 5) * a(2);
        this->v[18u] = C(5, 0) * a(4) * one_half +
                       (C(1, 0) * a(3) + C(3, 0) * a(0)) * icste;
        this->v[19u] = C(5, 1) * a(4) * one_half +
                       (C(1, 1) * a(3) + C(3, 1) * a(0)) * icste;
        this->v[20u] = C(5, 2) * a(4) * one_half +
                       (C(1, 2) * a(3) + C(3, 2) * a(0)) * icste;
        this->v[21u] = C(5, 3) * a(4) * one_half +
                       (C(1, 3) * a(3) + C(3, 3) * a(0)) * icste;
        this->v[22u] = C(5, 4) * a(4) * one_half +
                       (C(1, 4) * a(3) + C(3, 4) * a(0)) * icste;
        this->v[23u] = C(5, 5) * a(4) * one_half +
                       (C(1, 5) * a(3) + C(3, 5) * a(0)) * icste;
        this->v[24u] = C(4, 0) * a(5) * one_half +
                       (C(0, 0) * a(3) + C(3, 0) * a(1)) * icste;
        this->v[25u] = C(4, 1) * a(5) * one_half +
                       (C(0, 1) * a(3) + C(3, 1) * a(1)) * icste;
        this->v[26u] = C(4, 2) * a(5) * one_half +
                       (C(0, 2) * a(3) + C(3, 2) * a(1)) * icste;
        this->v[27u] = C(4, 3) * a(5) * one_half +
                       (C(0, 3) * a(3) + C(3, 3) * a(1)) * icste;
        this->v[28u] = C(4, 4) * a(5) * one_half +
                       (C(0, 4) * a(3) + C(3, 4) * a(1)) * icste;
        this->v[29u] = C(4, 5) * a(5) * one_half +
                       (C(0, 5) * a(3) + C(3, 5) * a(1)) * icste;
        this->v[30u] = C(5, 0) * a(3) * one_half +
                       (C(2, 0) * a(4) + C(4, 0) * a(0)) * icste;
        this->v[31u] = C(5, 1) * a(3) * one_half +
                       (C(2, 1) * a(4) + C(4, 1) * a(0)) * icste;
        this->v[32u] = C(5, 2) * a(3) * one_half +
                       (C(2, 2) * a(4) + C(4, 2) * a(0)) * icste;
        this->v[33u] = C(5, 3) * a(3) * one_half +
                       (C(2, 3) * a(4) + C(4, 3) * a(0)) * icste;
        this->v[34u] = C(5, 4) * a(3) * one_half +
                       (C(2, 4) * a(4) + C(4, 4) * a(0)) * icste;
        this->v[35u] = C(5, 5) * a(3) * one_half +
                       (C(2, 5) * a(4) + C(4, 5) * a(0)) * icste;
        this->v[36u] = C(3, 0) * a(5) * one_half +
                       (C(0, 0) * a(4) + C(4, 0) * a(2)) * icste;
        this->v[37u] = C(3, 1) * a(5) * one_half +
                       (C(0, 1) * a(4) + C(4, 1) * a(2)) * icste;
        this->v[38u] = C(3, 2) * a(5) * one_half +
                       (C(0, 2) * a(4) + C(4, 2) * a(2)) * icste;
        this->v[39u] = C(3, 3) * a(5) * one_half +
                       (C(0, 3) * a(4) + C(4, 3) * a(2)) * icste;
        this->v[40u] = C(3, 4) * a(5) * one_half +
                       (C(0, 4) * a(4) + C(4, 4) * a(2)) * icste;
        this->v[41u] = C(3, 5) * a(5) * one_half +
                       (C(0, 5) * a(4) + C(4, 5) * a(2)) * icste;
        this->v[42u] = C(4, 0) * a(3) * one_half +
                       (C(2, 0) * a(5) + C(5, 0) * a(1)) * icste;
        this->v[43u] = C(4, 1) * a(3) * one_half +
                       (C(2, 1) * a(5) + C(5, 1) * a(1)) * icste;
        this->v[44u] = C(4, 2) * a(3) * one_half +
                       (C(2, 2) * a(5) + C(5, 2) * a(1)) * icste;
        this->v[45u] = C(4, 3) * a(3) * one_half +
                       (C(2, 3) * a(5) + C(5, 3) * a(1)) * icste;
        this->v[46u] = C(4, 4) * a(3) * one_half +
                       (C(2, 4) * a(5) + C(5, 4) * a(1)) * icste;
        this->v[47u] = C(4, 5) * a(3) * one_half +
                       (C(2, 5) * a(5) + C(5, 5) * a(1)) * icste;
        this->v[48u] = C(3, 0) * a(4) * one_half +
                       (C(1, 0) * a(5) + C(5, 0) * a(2)) * icste;
        this->v[49u] = C(3, 1) * a(4) * one_half +
                       (C(1, 1) * a(5) + C(5, 1) * a(2)) * icste;
        this->v[50u] = C(3, 2) * a(4) * one_half +
                       (C(1, 2) * a(5) + C(5, 2) * a(2)) * icste;
        this->v[51u] = C(3, 3) * a(4) * one_half +
                       (C(1, 3) * a(5) + C(5, 3) * a(2)) * icste;
        this->v[52u] = C(3, 4) * a(4) * one_half +
                       (C(1, 4) * a(5) + C(5, 4) * a(2)) * icste;
        this->v[53u] = C(3, 5) * a(4) * one_half +
                       (C(1, 5) * a(5) + C(5, 5) * a(2)) * icste;
      }  // end of Expr
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
    };  // end of struct
        // Expr<ST2toT2ResultType,StensorProductRightDerivativeExpr<1u> >

  }  // namespace math

}  // namespace tfel

#endif /* LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_H */
