/*!
 * \file   MatrixUtilities.ixx
 * \brief
 * \author HELFER Thomas 202608
 * \date   07/06/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_MATRIX_MATRIXUTILITIES_IXX
#define LIB_TFEL_MATH_MATRIX_MATRIXUTILITIES_IXX

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, typename NumType, unsigned short I>
    struct DiagonalTermProduct {
      /*!
       * \return the product m(I*I)*m(I+1,I+1)*....*m(N-1,N-1)
       * \tparam MatrixType: matrix type
       * \param[in] m: matrix
       */
      template <typename MatrixType>
      static constexpr typename ComputeUnaryResult<NumType, Power<N>>::Result
      exe(const MatrixType& m) {
        return m(I, I) * DiagonalTermProduct<N, NumType, I + 1>::exe(m);
      }  // end of exe
    };   // end of struct DiagonalTermProduct

    template <unsigned short N, typename NumType>
    struct DiagonalTermProduct<N, NumType, N> {
      /*!
       * \return 1
       * \tparam MatrixType: matrix type
       * \param[in] m: matrix
       */
      template <typename MatrixType>
      static constexpr tfel::typetraits::base_type<NumType> exe(
          const MatrixType&) {
        using real = tfel::typetraits::base_type<NumType>;
        return real{1};
      }  // end of exe
    };   // end of struct DiagonalTermProduct

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_MATRIX_MATRIXUTILITIES_IXX */
