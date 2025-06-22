/*!
 * \file  include/TFEL/Math/ST2toST2/ConvertT2toST2ToST2toST2Expr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 juil. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONVERTT2TOST2TOST2TOST2EXPR_H_
#define LIB_TFEL_MATH_CONVERTT2TOST2TOST2TOST2EXPR_H_

#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel {

  namespace math {

    /*!
     * an helper class to define a partial specialisation of the class
     * Expr
     */
    template <unsigned short N>
    struct ConvertT2toST2ToST2toST2Expr;

    /*!
     * Partial specialisation for 1D stensor
     */
    template <typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType, ConvertT2toST2ToST2toST2Expr<1u>>
        : public ST2toST2Concept<
              Expr<ST2toST2ResultType, ConvertT2toST2ToST2toST2Expr<1u>>>,
          public fsarray<9u,
                         typename ST2toST2Traits<ST2toST2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime == 1u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : t2tost2 to be converted
       */
      template <typename T2toST2Type>
      Expr(const T2toST2Type& A) {
        using tfel::fsalgo::copy;
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime ==
                            T2toST2Traits<T2toST2Type>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename T2toST2Traits<T2toST2Type>::NumType,
                typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
        copy<9u>::exe(&A(0, 0), this->v);
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
        // Expr<ST2toST2ResultType,ConvertT2toST2ToST2toST2Expr<1u> >

    /*!
     * Partial specialisation for 2D stensor
     */
    template <typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType, ConvertT2toST2ToST2toST2Expr<2u>>
        : public ST2toST2Concept<
              Expr<ST2toST2ResultType, ConvertT2toST2ToST2toST2Expr<2u>>>,
          public fsarray<16u,
                         typename ST2toST2Traits<ST2toST2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime == 2u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : t2tost2 to be converted
       */
      template <typename T2toST2Type>
      Expr(const T2toST2Type& A) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime ==
                            T2toST2Traits<T2toST2Type>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename T2toST2Traits<T2toST2Type>::NumType,
                typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
        using tfel::typetraits::BaseType;
        typedef typename BaseType<value_type>::type real;
        constexpr const auto icste = Cste<value_type>::sqrt2;
        constexpr const auto icste2 = Cste<value_type>::sqrt2 / (real(2));
        this->v[0] = A(0, 0);
        this->v[1] = A(0, 1);
        this->v[2] = A(0, 2);
        this->v[4] = A(1, 0);
        this->v[5] = A(1, 1);
        this->v[6] = A(1, 2);
        this->v[8] = A(2, 0);
        this->v[9] = A(2, 1);
        this->v[10] = A(2, 2);
        this->v[12] = A(3, 0);
        this->v[13] = A(3, 1);
        this->v[14] = A(3, 2);
        this->v[3] = (A(0, 3) + A(0, 4)) * icste2;
        this->v[7] = (A(1, 3) + A(1, 4)) * icste2;
        this->v[11] = (A(2, 3) + A(2, 4)) * icste2;
        this->v[15] = (A(3, 3) + A(3, 4)) * icste;
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
        // Expr<ST2toST2ResultType,ConvertT2toST2ToST2toST2Expr<2u> >

    /*!
     * Partial specialisation for 3D stensor
     */
    template <typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType, ConvertT2toST2ToST2toST2Expr<3u>>
        : public ST2toST2Concept<
              Expr<ST2toST2ResultType, ConvertT2toST2ToST2toST2Expr<3u>>>,
          public fsarray<36u,
                         typename ST2toST2Traits<ST2toST2ResultType>::NumType> {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime == 3u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : t2tost2 to be converted
       */
      template <typename T2toST2Type>
      Expr(const T2toST2Type& A) {
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::meta::Implements<T2toST2Type, T2toST2Concept>::cond));
        //! a simple check
        TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime ==
                            T2toST2Traits<T2toST2Type>::dime));
        //! a simple check
        TFEL_STATIC_ASSERT(
            (tfel::typetraits::IsAssignableTo<
                typename T2toST2Traits<T2toST2Type>::NumType,
                typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
        using tfel::typetraits::BaseType;
        typedef typename BaseType<value_type>::type real;
        constexpr const auto icste = Cste<value_type>::sqrt2;
        constexpr const auto icste2 = Cste<value_type>::sqrt2 / (real(2));
        this->v[0] = A(0, 0);
        this->v[1] = A(0, 1);
        this->v[2] = A(0, 2);
        this->v[6] = A(1, 0);
        this->v[7] = A(1, 1);
        this->v[8] = A(1, 2);
        this->v[12] = A(2, 0);
        this->v[13] = A(2, 1);
        this->v[14] = A(2, 2);
        this->v[18] = A(3, 0);
        this->v[19] = A(3, 1);
        this->v[20] = A(3, 2);
        this->v[24] = A(4, 0);
        this->v[25] = A(4, 1);
        this->v[26] = A(4, 2);
        this->v[30] = A(5, 0);
        this->v[31] = A(5, 1);
        this->v[32] = A(5, 2);
        this->v[3] = (A(0, 3) + A(0, 4)) * icste2;
        this->v[4] = (A(0, 5) + A(0, 6)) * icste2;
        this->v[5] = (A(0, 7) + A(0, 8)) * icste2;
        this->v[9] = (A(1, 3) + A(1, 4)) * icste2;
        this->v[10] = (A(1, 5) + A(1, 6)) * icste2;
        this->v[11] = (A(1, 7) + A(1, 8)) * icste2;
        this->v[15] = (A(2, 3) + A(2, 4)) * icste2;
        this->v[16] = (A(2, 5) + A(2, 6)) * icste2;
        this->v[17] = (A(2, 7) + A(2, 8)) * icste2;
        this->v[21] = (A(3, 3) + A(3, 4)) * icste;
        this->v[22] = (A(3, 5) + A(3, 6)) * icste;
        this->v[23] = (A(3, 7) + A(3, 8)) * icste;
        this->v[27] = (A(4, 3) + A(4, 4)) * icste;
        this->v[28] = (A(4, 5) + A(4, 6)) * icste;
        this->v[29] = (A(4, 7) + A(4, 8)) * icste;
        this->v[33] = (A(5, 3) + A(5, 4)) * icste;
        this->v[34] = (A(5, 5) + A(5, 6)) * icste;
        this->v[35] = (A(5, 7) + A(5, 8)) * icste;
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
        // Expr<ST2toST2ResultType,ConvertT2toST2ToST2toST2Expr<3u> >

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_CONVERTT2TOST2TOST2TOST2EXPR_H_ */
