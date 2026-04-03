/*!
 * \file   CalculiXRotationMatrix.hxx
 * \brief
 * \author Thomas Helfer
 * \date   18 sept. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CALCULIX_CALCULIXROTATIONMATRIX_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXROTATIONMATRIX_HXX

#include "TFEL/Math/tmatrix.hxx"

#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXConfig.hxx"

namespace calculix {

  /*!
   * \brief build a rotation matrix from the coefficients given by
   * `CalculiX`.
   * \param[in] c: coefficients used to build the rotation matrix.
   * \param[in] p: position of current Gauss point
   * \return the rotation matrix
   */
  MFRONT_CALCULIX_VISIBILITY_EXPORT
  tfel::math::tmatrix<3u, 3u, CalculiXReal> getRotationMatrix(
      const CalculiXReal* const, const CalculiXReal* const);

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXROTATIONMATRIX_HXX */
