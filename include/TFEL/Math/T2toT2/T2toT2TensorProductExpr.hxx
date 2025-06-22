/*!
 * \file  include/TFEL/Math/T2toT2/T2toT2TensorProductExpr.hxx
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

#ifndef LIB_TFEL_MATH_T2TOT2TENSORPRODUCTEXPR_HXX
#define LIB_TFEL_MATH_T2TOT2TENSORPRODUCTEXPR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/fsarray.hxx"

namespace tfel::math {

  //! Empty structure used for partial specialisation of the
  //! Expr class
  template <unsigned short N>
  struct T2toT2TensorProductExpr {};  // end of struct T2toT2TensorProductExpr

  /*!
   * Partial specialisation
   */
  template <typename TensorResultType>
  struct Expr<TensorResultType, T2toT2TensorProductExpr<1u>>
      : public TensorConcept<
            Expr<TensorResultType, T2toT2TensorProductExpr<1u>>>,
        public array_holder<
            TensorDimeToSize<getSpaceDimension<TensorResultType>()>::value,
            numeric_type<TensorResultType>> {
    static_assert(getSpaceDimension<TensorResultType>() == 1u);
    //! a simple alias
    typedef EmptyRunTimeProperties RunTimeProperties;
    //! a simple alias
    typedef numeric_type<TensorResultType> value_type;
    /*!
     * \param[in] a : first term of the product
     * \param[in] b : second term of the product
     */
    template <typename T2toT2Type, typename TensorType>
    TFEL_MATH_INLINE Expr(const T2toT2Type& a, const TensorType& b) {
      static_assert(implementsT2toT2Concept<T2toT2Type>());
      static_assert(implementsTensorConcept<TensorType>());
      static_assert(getSpaceDimension<T2toT2Type>() == 1u);
      static_assert(getSpaceDimension<TensorType>() == 1u);
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
     * \return the runtime properties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }
  };  // end of struct Expr<TensorResultType,T2ST2toT2TensorProductExp>

  /*!
   * Partial specialisation
   */
  template <typename TensorResultType>
  struct Expr<TensorResultType, T2toT2TensorProductExpr<2u>>
      : public TensorConcept<
            Expr<TensorResultType, T2toT2TensorProductExpr<2u>>>,
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
    template <typename T2toT2Type, typename TensorType>
    TFEL_MATH_INLINE Expr(const T2toT2Type& a, const TensorType& b) {
      static_assert(implementsT2toT2Concept<T2toT2Type>());
      static_assert(implementsTensorConcept<TensorType>());
      static_assert(getSpaceDimension<T2toT2Type>() == 2u);
      static_assert(getSpaceDimension<TensorType>() == 2u);
      this->v[0] = a(0, 4) * b[4] + a(0, 3) * b[3] + a(0, 2) * b[2] +
                   a(0, 1) * b[1] + a(0, 0) * b[0];
      this->v[1] = a(1, 4) * b[4] + a(1, 3) * b[3] + a(1, 2) * b[2] +
                   a(1, 1) * b[1] + a(1, 0) * b[0];
      this->v[2] = a(2, 4) * b[4] + a(2, 3) * b[3] + a(2, 2) * b[2] +
                   a(2, 1) * b[1] + a(2, 0) * b[0];
      this->v[3] = a(3, 4) * b[4] + a(3, 3) * b[3] + a(3, 2) * b[2] +
                   a(3, 1) * b[1] + a(3, 0) * b[0];
      this->v[4] = a(4, 4) * b[4] + a(4, 3) * b[3] + a(4, 2) * b[2] +
                   a(4, 1) * b[1] + a(4, 0) * b[0];
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
  };  // end of struct Expr<TensorResultType,T2ST2toT2TensorProductExp>

  /*!
   * Partial specialisation
   */
  template <typename TensorResultType>
  struct Expr<TensorResultType, T2toT2TensorProductExpr<3u>>
      : public TensorConcept<
            Expr<TensorResultType, T2toT2TensorProductExpr<3u>>>,
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
    template <typename T2toT2Type, typename TensorType>
    TFEL_MATH_INLINE Expr(const T2toT2Type& a, const TensorType& b) {
      static_assert(implementsT2toT2Concept<T2toT2Type>());
      static_assert(implementsTensorConcept<TensorType>());
      static_assert(getSpaceDimension<T2toT2Type>() == 3u);
      static_assert(getSpaceDimension<TensorType>() == 3u);
      this->v[0] = a(0, 8) * b[8] + a(0, 7) * b[7] + a(0, 6) * b[6] +
                   a(0, 5) * b[5] + a(0, 4) * b[4] + a(0, 3) * b[3] +
                   a(0, 2) * b[2] + a(0, 1) * b[1] + a(0, 0) * b[0];
      this->v[1] = a(1, 8) * b[8] + a(1, 7) * b[7] + a(1, 6) * b[6] +
                   a(1, 5) * b[5] + a(1, 4) * b[4] + a(1, 3) * b[3] +
                   a(1, 2) * b[2] + a(1, 1) * b[1] + a(1, 0) * b[0];
      this->v[2] = a(2, 8) * b[8] + a(2, 7) * b[7] + a(2, 6) * b[6] +
                   a(2, 5) * b[5] + a(2, 4) * b[4] + a(2, 3) * b[3] +
                   a(2, 2) * b[2] + a(2, 1) * b[1] + a(2, 0) * b[0];
      this->v[3] = a(3, 8) * b[8] + a(3, 7) * b[7] + a(3, 6) * b[6] +
                   a(3, 5) * b[5] + a(3, 4) * b[4] + a(3, 3) * b[3] +
                   a(3, 2) * b[2] + a(3, 1) * b[1] + a(3, 0) * b[0];
      this->v[4] = a(4, 8) * b[8] + a(4, 7) * b[7] + a(4, 6) * b[6] +
                   a(4, 5) * b[5] + a(4, 4) * b[4] + a(4, 3) * b[3] +
                   a(4, 2) * b[2] + a(4, 1) * b[1] + a(4, 0) * b[0];
      this->v[5] = a(5, 8) * b[8] + a(5, 7) * b[7] + a(5, 6) * b[6] +
                   a(5, 5) * b[5] + a(5, 4) * b[4] + a(5, 3) * b[3] +
                   a(5, 2) * b[2] + a(5, 1) * b[1] + a(5, 0) * b[0];
      this->v[6] = a(6, 8) * b[8] + a(6, 7) * b[7] + a(6, 6) * b[6] +
                   a(6, 5) * b[5] + a(6, 4) * b[4] + a(6, 3) * b[3] +
                   a(6, 2) * b[2] + a(6, 1) * b[1] + a(6, 0) * b[0];
      this->v[7] = a(7, 8) * b[8] + a(7, 7) * b[7] + a(7, 6) * b[6] +
                   a(7, 5) * b[5] + a(7, 4) * b[4] + a(7, 3) * b[3] +
                   a(7, 2) * b[2] + a(7, 1) * b[1] + a(7, 0) * b[0];
      this->v[8] = a(8, 8) * b[8] + a(8, 7) * b[7] + a(8, 6) * b[6] +
                   a(8, 5) * b[5] + a(8, 4) * b[4] + a(8, 3) * b[3] +
                   a(8, 2) * b[2] + a(8, 1) * b[1] + a(8, 0) * b[0];
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
  };  // end of struct Expr<TensorResultType,T2ST2toT2TensorProductExp>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOT2TENSORPRODUCTEXPR_HXX */
