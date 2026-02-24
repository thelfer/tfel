/*!
 * \file  include/TFEL/Math/ST2toST2/StensorST2toST2ProductExpr.hxx
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

#ifndef LIB_TFEL_MATH_STENSORST2TOST2PRODUCTEXPR_HXX
#define LIB_TFEL_MATH_STENSORST2TOST2PRODUCTEXPR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel::math {

  //! Empty structure used for partial specialisation of the
  //! Expr class
  template <unsigned short N>
  struct StensorST2toST2ProductExpr {
  };  // end of struct StensorST2toST2ProductExpr

  //!\brief partial specialisation in 1D
  template <StensorConcept StensorResultType>
  struct Expr<StensorResultType, StensorST2toST2ProductExpr<1u>>
      : public StensorConceptBase<
            Expr<StensorResultType, StensorST2toST2ProductExpr<1u>>>,
        public array_holder<
            StensorDimeToSize<getSpaceDimension<StensorResultType>()>::value,
            numeric_type<StensorResultType>> {
    static_assert(getSpaceDimension<StensorResultType>() == 1u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<StensorResultType> value_type;
    //! a simple check
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& a,
                                    const ST2toST2Type& b) noexcept {
      static_assert(getSpaceDimension<ST2toST2Type>() == 1u);
      static_assert(getSpaceDimension<StensorType>() == 1u);
      this->v[0] = b(0, 0) * a(0) + b(1, 0) * a(1) + b(2, 0) * a(2);
      this->v[1] = b(0, 1) * a(0) + b(1, 1) * a(1) + b(2, 1) * a(2);
      this->v[2] = b(0, 2) * a(0) + b(1, 2) * a(1) + b(2, 2) * a(2);
    }  // end of Expr
    /*!
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator[](
        const unsigned short i) const noexcept {
      return this->operator()(i);
    }
    /*!
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator()(
        const unsigned short i) const noexcept {
      return this->v[i];
    }  // end of operator()
    //! \return the runtime properties
    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }
  };  // end of struct Expr<StensorResultType,T2SStensorST2toST2ProductExp>

  //! \brief partial specialisation in 2D
  template <StensorConcept StensorResultType>
  struct Expr<StensorResultType, StensorST2toST2ProductExpr<2u>>
      : public StensorConceptBase<
            Expr<StensorResultType, StensorST2toST2ProductExpr<2u>>>,
        public array_holder<
            StensorDimeToSize<getSpaceDimension<StensorResultType>()>::value,
            numeric_type<StensorResultType>> {
    static_assert(getSpaceDimension<StensorResultType>() == 2u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<StensorResultType> value_type;
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& a,
                                    const ST2toST2Type& b) noexcept {
      static_assert(getSpaceDimension<ST2toST2Type>() == 2u);
      static_assert(getSpaceDimension<StensorType>() == 2u);
      this->v[0] =
          b(0, 0) * a(0) + b(1, 0) * a(1) + b(2, 0) * a(2) + b(3, 0) * a(3);
      this->v[1] =
          b(0, 1) * a(0) + b(1, 1) * a(1) + b(2, 1) * a(2) + b(3, 1) * a(3);
      this->v[2] =
          b(0, 2) * a(0) + b(1, 2) * a(1) + b(2, 2) * a(2) + b(3, 2) * a(3);
      this->v[3] =
          b(0, 3) * a(0) + b(1, 3) * a(1) + b(2, 3) * a(2) + b(3, 3) * a(3);
    }  // end of Expr
    /*!
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator[](
        const unsigned short i) const noexcept {
      return this->operator()(i);
    }
    /*!
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator()(
        const unsigned short i) const noexcept {
      return this->v[i];
    }  // end of operator()
    //! \return the runtime properties
    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }
  };  // end of struct Expr<StensorResultType,T2SStensorST2toST2ProductExp>

  //! \brief partial specialisation in 3D
  template <StensorConcept StensorResultType>
  struct Expr<StensorResultType, StensorST2toST2ProductExpr<3u>>
      : public StensorConceptBase<
            Expr<StensorResultType, StensorST2toST2ProductExpr<3u>>>,
        public array_holder<
            StensorDimeToSize<getSpaceDimension<StensorResultType>()>::value,
            numeric_type<StensorResultType>> {
    static_assert(getSpaceDimension<StensorResultType>() == 3u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<StensorResultType> value_type;
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <StensorConcept StensorType, ST2toST2Concept ST2toST2Type>
    TFEL_HOST_DEVICE constexpr Expr(const StensorType& a,
                                    const ST2toST2Type& b) noexcept {
      static_assert(getSpaceDimension<ST2toST2Type>() == 3u);
      static_assert(getSpaceDimension<StensorType>() == 3u);
      this->v[0] = b(0, 0) * a(0) + b(1, 0) * a(1) + b(2, 0) * a(2) +
                   b(3, 0) * a(3) + b(4, 0) * a(4) + b(5, 0) * a(5);
      this->v[1] = b(0, 1) * a(0) + b(1, 1) * a(1) + b(2, 1) * a(2) +
                   b(3, 1) * a(3) + b(4, 1) * a(4) + b(5, 1) * a(5);
      this->v[2] = b(0, 2) * a(0) + b(1, 2) * a(1) + b(2, 2) * a(2) +
                   b(3, 2) * a(3) + b(4, 2) * a(4) + b(5, 2) * a(5);
      this->v[3] = b(0, 3) * a(0) + b(1, 3) * a(1) + b(2, 3) * a(2) +
                   b(3, 3) * a(3) + b(4, 3) * a(4) + b(5, 3) * a(5);
      this->v[4] = b(0, 4) * a(0) + b(1, 4) * a(1) + b(2, 4) * a(2) +
                   b(3, 4) * a(3) + b(4, 4) * a(4) + b(5, 4) * a(5);
      this->v[5] = b(0, 5) * a(0) + b(1, 5) * a(1) + b(2, 5) * a(2) +
                   b(3, 5) * a(3) + b(4, 5) * a(4) + b(5, 5) * a(5);
    }  // end of Expr
    /*!
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator[](
        const unsigned short i) const noexcept {
      return this->operator()(i);
    }
    /*!
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_HOST_DEVICE constexpr const value_type& operator()(
        const unsigned short i) const noexcept {
      return this->v[i];
    }  // end of operator()
    //! \return the runtime properties
    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }
  };  // end of struct Expr<StensorResultType,T2SStensorST2toST2ProductExp>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORST2TOST2PRODUCTEXPR_HXX */
