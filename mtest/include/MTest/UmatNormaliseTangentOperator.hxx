/*!
 * \file  mtest/include/MTest/UmatNormaliseTangentOperator.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 15 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTUMATNORMALISETANGENTOPERATOR_HXX
#define LIB_MTEST_MTESTUMATNORMALISETANGENTOPERATOR_HXX

#include "TFEL/Math/matrix.hxx"

#include "MTest/Types.hxx"

namespace mtest {
  /*!
   * \brief normalise the tangent operator returned by:
   * - Cast3M behaviours
   * - Abaqus/Standard behaviours
   */
  struct UmatNormaliseTangentOperator {
    /*!
     * \param[out] K : normalised matrix
     * \param[in]  D : tangent operator
     * \param[in]  d : dimension
     */
    static void exe(real* const,
                    const tfel::math::matrix<real>&,
                    const unsigned short);
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTUMATNORMALISETANGENTOPERATOR_HXX */
