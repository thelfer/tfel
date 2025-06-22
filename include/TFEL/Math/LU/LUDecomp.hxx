/*!
 * \file  include/TFEL/Math/LU/LUDecomp.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 15 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LUDECOMP_HXX
#define LIB_TFEL_MATH_LUDECOMP_HXX

#include <cmath>
#include <limits>
#include <algorithm>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/LU/LUException.hxx"

namespace tfel {

  namespace math {

    /*!
     * structure in charge of computing the LU decomposition of a
     * matrix. Line permutation is performed.
     */
    struct LUDecomp {
      /*!
       * Compute the LU decomposition of a matrix. Line permutation is
       * performed.
       *
       * \param[in,out] m   : the matrix to be decomposed
       * \param[in,out] p   : the permutation vector
       * \param[in] eps : numerical parameter used to detect null pivot
       * \return the number of permutation made
       *
       * \note The decomposition is done in-place
       */
      template <typename MatrixType, typename PermutationType>
      static TFEL_MATH_INLINE2 int exe(
          MatrixType&,
          PermutationType&,
          const typename MatrixTraits<MatrixType>::NumType =
              100 * std::numeric_limits<
                        typename MatrixTraits<MatrixType>::NumType>::min());

    };  // end of struct LUDecomp

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/LU/LUDecomp.ixx"

#endif /* LIB_TFEL_MATH_LUDECOMP_HXX */
