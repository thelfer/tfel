/*!
 * \file  include/TFEL/Math/ST2toST2/ST2toST2StensorProductExpr.hxx
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

#ifndef LIB_TFEL_MATH_ST2TOST2STENSORPRODUCTEXPR_HXX
#define LIB_TFEL_MATH_ST2TOST2STENSORPRODUCTEXPR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/fsarray.hxx"

namespace tfel::math {

  //! Empty structure used for partial specialisation of the
  //! Expr class
  template <unsigned short N>
  struct ST2toST2StensorProductExpr {
  };  // end of struct ST2toST2StensorProductExpr

  /*!
   * Partial specialisation
   */
  template <typename StensorResultType>
  struct Expr<StensorResultType, ST2toST2StensorProductExpr<1u>>
      : public StensorConcept<
            Expr<StensorResultType, ST2toST2StensorProductExpr<1u>>>,
        public array_holder<
            StensorDimeToSize<getSpaceDimension<StensorResultType>()>::value,
            numeric_type<StensorResultType>> {
    static_assert(getSpaceDimension<StensorResultType>() == 1u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<StensorResultType> value_type;
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <typename ST2toST2Type, typename StensorType>
    TFEL_MATH_INLINE Expr(const ST2toST2Type& a, const StensorType& b) {
      static_assert(implementsST2toST2Concept<ST2toST2Type>());
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() == 1u);
      static_assert(getSpaceDimension<StensorType>() == 1u);
      this->v[0] = a(0, 0) * b(0) + a(0, 1) * b(1) + a(0, 2) * b(2);
      this->v[1] = a(1, 0) * b(0) + a(1, 1) * b(1) + a(1, 2) * b(2);
      this->v[2] = a(2, 0) * b(0) + a(2, 1) * b(1) + a(2, 2) * b(2);
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
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_MATH_INLINE const value_type& operator[](
        const unsigned short i) const {
      return this->v[i];
    }  // end of operator[]
    /*!
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<StensorResultType,T2SST2toST2StensorProductExp>

  /*!
   * Partial specialisation
   */
  template <typename StensorResultType>
  struct Expr<StensorResultType, ST2toST2StensorProductExpr<2u>>
      : public StensorConcept<
            Expr<StensorResultType, ST2toST2StensorProductExpr<2u>>>,
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
    template <typename ST2toST2Type, typename StensorType>
    TFEL_MATH_INLINE Expr(const ST2toST2Type& a, const StensorType& b) {
      static_assert(implementsST2toST2Concept<ST2toST2Type>());
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() == 2u);
      static_assert(getSpaceDimension<StensorType>() == 2u);
      this->v[0] =
          a(0, 0) * b(0) + a(0, 1) * b(1) + a(0, 2) * b(2) + a(0, 3) * b(3);
      this->v[1] =
          a(1, 0) * b(0) + a(1, 1) * b(1) + a(1, 2) * b(2) + a(1, 3) * b(3);
      this->v[2] =
          a(2, 0) * b(0) + a(2, 1) * b(1) + a(2, 2) * b(2) + a(2, 3) * b(3);
      this->v[3] =
          a(3, 0) * b(0) + a(3, 1) * b(1) + a(3, 2) * b(2) + a(3, 3) * b(3);
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
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_MATH_INLINE const value_type& operator[](
        const unsigned short i) const {
      return this->v[i];
    }  // end of operator[]
    /*!
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<StensorResultType,T2SST2toST2StensorProductExp>

  /*!
   * Partial specialisation
   */
  template <typename StensorResultType>
  struct Expr<StensorResultType, ST2toST2StensorProductExpr<3u>>
      : public StensorConcept<
            Expr<StensorResultType, ST2toST2StensorProductExpr<3u>>>,
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
    template <typename ST2toST2Type, typename StensorType>
    TFEL_MATH_INLINE Expr(const ST2toST2Type& a, const StensorType& b) {
      static_assert(implementsST2toST2Concept<ST2toST2Type>());
      static_assert(implementsStensorConcept<StensorType>());
      static_assert(getSpaceDimension<ST2toST2Type>() == 3u);
      static_assert(getSpaceDimension<StensorType>() == 3u);
      this->v[0] = a(0, 0) * b(0) + a(0, 1) * b(1) + a(0, 2) * b(2) +
                   a(0, 3) * b(3) + a(0, 4) * b(4) + a(0, 5) * b(5);
      this->v[1] = a(1, 0) * b(0) + a(1, 1) * b(1) + a(1, 2) * b(2) +
                   a(1, 3) * b(3) + a(1, 4) * b(4) + a(1, 5) * b(5);
      this->v[2] = a(2, 0) * b(0) + a(2, 1) * b(1) + a(2, 2) * b(2) +
                   a(2, 3) * b(3) + a(2, 4) * b(4) + a(2, 5) * b(5);
      this->v[3] = a(3, 0) * b(0) + a(3, 1) * b(1) + a(3, 2) * b(2) +
                   a(3, 3) * b(3) + a(3, 4) * b(4) + a(3, 5) * b(5);
      this->v[4] = a(4, 0) * b(0) + a(4, 1) * b(1) + a(4, 2) * b(2) +
                   a(4, 3) * b(3) + a(4, 4) * b(4) + a(4, 5) * b(5);
      this->v[5] = a(5, 0) * b(0) + a(5, 1) * b(1) + a(5, 2) * b(2) +
                   a(5, 3) * b(3) + a(5, 4) * b(4) + a(5, 5) * b(5);
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
     * \brief access operator
     * \param[in] i : index
     */
    TFEL_MATH_INLINE const value_type& operator[](
        const unsigned short i) const {
      return this->v[i];
    }  // end of operator[]
    /*!
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<StensorResultType,T2SST2toST2StensorProductExp>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOST2STENSORPRODUCTEXPR_HXX */
