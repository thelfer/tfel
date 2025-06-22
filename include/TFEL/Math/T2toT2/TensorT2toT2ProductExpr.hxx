/*!
 * \file  include/TFEL/Math/T2toT2/TensorT2toT2ProductExpr.hxx
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

#ifndef LIB_TFEL_MATH_TENSORT2TOT2PRODUCTEXPR_HXX
#define LIB_TFEL_MATH_TENSORT2TOT2PRODUCTEXPR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel::math {

  //! Empty structure used for partial specialisation of the
  //! Expr class
  template <unsigned short N>
  struct TensorT2toT2ProductExpr {};  // end of struct TensorT2toT2ProductExpr

  /*!
   * Partial specialisation
   */
  template <typename TensorResultType>
  struct Expr<TensorResultType, TensorT2toT2ProductExpr<1u>>
      : public TensorConcept<
            Expr<TensorResultType, TensorT2toT2ProductExpr<1u>>>,
        public array_holder<
            TensorDimeToSize<getSpaceDimension<TensorResultType>()>::value,
            numeric_type<TensorResultType>> {
    static_assert(getSpaceDimension<TensorResultType>() == 1u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<TensorResultType> value_type;
    //! a simple check
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <typename TensorType, typename T2toT2Type>
    TFEL_MATH_INLINE Expr(const TensorType& a, const T2toT2Type& b) {
      static_assert(getSpaceDimension<T2toT2Type>() == 1u);
      static_assert(implementsT2toT2Concept<T2toT2Type>());
      static_assert(implementsTensorConcept<TensorType>());
      static_assert(getSpaceDimension<TensorType>() == 1u);
      this->v[0] = b(0, 0) * a(0) + b(1, 0) * a(1) + b(2, 0) * a(2);
      this->v[1] = b(0, 1) * a(0) + b(1, 1) * a(1) + b(2, 1) * a(2);
      this->v[2] = b(0, 2) * a(0) + b(1, 2) * a(1) + b(2, 2) * a(2);
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
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<TensorResultType,T2STensorT2toT2ProductExp>

  /*!
   * Partial specialisation
   */
  template <typename TensorResultType>
  struct Expr<TensorResultType, TensorT2toT2ProductExpr<2u>>
      : public TensorConcept<
            Expr<TensorResultType, TensorT2toT2ProductExpr<2u>>>,
        public array_holder<
            TensorDimeToSize<getSpaceDimension<TensorResultType>()>::value,
            numeric_type<TensorResultType>> {
    static_assert(getSpaceDimension<TensorResultType>() == 2u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<TensorResultType> value_type;
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <typename TensorType, typename T2toT2Type>
    TFEL_MATH_INLINE Expr(const TensorType& a, const T2toT2Type& b) {
      static_assert(implementsT2toT2Concept<T2toT2Type>());
      static_assert(implementsTensorConcept<TensorType>());
      static_assert(getSpaceDimension<T2toT2Type>() == 2u);
      static_assert(getSpaceDimension<TensorType>() == 2u);
      this->v[0] = b(4, 0) * a[4] + b(3, 0) * a[3] + b(2, 0) * a[2] +
                   b(1, 0) * a[1] + b(0, 0) * a[0];
      this->v[1] = b(4, 1) * a[4] + b(3, 1) * a[3] + b(2, 1) * a[2] +
                   b(1, 1) * a[1] + b(0, 1) * a[0];
      this->v[2] = b(4, 2) * a[4] + b(3, 2) * a[3] + b(2, 2) * a[2] +
                   b(1, 2) * a[1] + b(0, 2) * a[0];
      this->v[3] = b(4, 3) * a[4] + b(3, 3) * a[3] + b(2, 3) * a[2] +
                   b(1, 3) * a[1] + b(0, 3) * a[0];
      this->v[4] = b(4, 4) * a[4] + b(3, 4) * a[3] + b(2, 4) * a[2] +
                   b(1, 4) * a[1] + b(0, 4) * a[0];
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
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<TensorResultType,T2STensorT2toT2ProductExp>

  /*!
   * Partial specialisation
   */
  template <typename TensorResultType>
  struct Expr<TensorResultType, TensorT2toT2ProductExpr<3u>>
      : public TensorConcept<
            Expr<TensorResultType, TensorT2toT2ProductExpr<3u>>>,
        public array_holder<
            TensorDimeToSize<getSpaceDimension<TensorResultType>()>::value,
            numeric_type<TensorResultType>> {
    static_assert(getSpaceDimension<TensorResultType>() == 3u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<TensorResultType> value_type;
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <typename TensorType, typename T2toT2Type>
    TFEL_MATH_INLINE Expr(const TensorType& a, const T2toT2Type& b) {
      static_assert(implementsT2toT2Concept<T2toT2Type>());
      static_assert(implementsTensorConcept<TensorType>());
      static_assert(getSpaceDimension<T2toT2Type>() == 3u);
      static_assert(getSpaceDimension<TensorType>() == 3u);
      this->v[0] = b(8, 0) * a[8] + b(7, 0) * a[7] + b(6, 0) * a[6] +
                   b(5, 0) * a[5] + b(4, 0) * a[4] + b(3, 0) * a[3] +
                   b(2, 0) * a[2] + b(1, 0) * a[1] + b(0, 0) * a[0];
      this->v[1] = b(8, 1) * a[8] + b(7, 1) * a[7] + b(6, 1) * a[6] +
                   b(5, 1) * a[5] + b(4, 1) * a[4] + b(3, 1) * a[3] +
                   b(2, 1) * a[2] + b(1, 1) * a[1] + b(0, 1) * a[0];
      this->v[2] = b(8, 2) * a[8] + b(7, 2) * a[7] + b(6, 2) * a[6] +
                   b(5, 2) * a[5] + b(4, 2) * a[4] + b(3, 2) * a[3] +
                   b(2, 2) * a[2] + b(1, 2) * a[1] + b(0, 2) * a[0];
      this->v[3] = b(8, 3) * a[8] + b(7, 3) * a[7] + b(6, 3) * a[6] +
                   b(5, 3) * a[5] + b(4, 3) * a[4] + b(3, 3) * a[3] +
                   b(2, 3) * a[2] + b(1, 3) * a[1] + b(0, 3) * a[0];
      this->v[4] = b(8, 4) * a[8] + b(7, 4) * a[7] + b(6, 4) * a[6] +
                   b(5, 4) * a[5] + b(4, 4) * a[4] + b(3, 4) * a[3] +
                   b(2, 4) * a[2] + b(1, 4) * a[1] + b(0, 4) * a[0];
      this->v[5] = b(8, 5) * a[8] + b(7, 5) * a[7] + b(6, 5) * a[6] +
                   b(5, 5) * a[5] + b(4, 5) * a[4] + b(3, 5) * a[3] +
                   b(2, 5) * a[2] + b(1, 5) * a[1] + b(0, 5) * a[0];
      this->v[6] = b(8, 6) * a[8] + b(7, 6) * a[7] + b(6, 6) * a[6] +
                   b(5, 6) * a[5] + b(4, 6) * a[4] + b(3, 6) * a[3] +
                   b(2, 6) * a[2] + b(1, 6) * a[1] + b(0, 6) * a[0];
      this->v[7] = b(8, 7) * a[8] + b(7, 7) * a[7] + b(6, 7) * a[6] +
                   b(5, 7) * a[5] + b(4, 7) * a[4] + b(3, 7) * a[3] +
                   b(2, 7) * a[2] + b(1, 7) * a[1] + b(0, 7) * a[0];
      this->v[8] = b(8, 8) * a[8] + b(7, 8) * a[7] + b(6, 8) * a[6] +
                   b(5, 8) * a[5] + b(4, 8) * a[4] + b(3, 8) * a[3] +
                   b(2, 8) * a[2] + b(1, 8) * a[1] + b(0, 8) * a[0];
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
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<TensorResultType,T2STensorT2toT2ProductExp>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSORT2TOT2PRODUCTEXPR_HXX */
