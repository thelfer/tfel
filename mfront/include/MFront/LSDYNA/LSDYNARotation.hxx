/*!
 * \file   mfront/include/MFront/LSDYNA/LSDYNARotation.hxx
 * \brief  This file declares the LSDYNARotationMatrix class
 * \author É. Castelier
 * \date   17 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LSDYNA_LSDYNAROTATION_HXX
#define LIB_MFRONT_LSDYNA_LSDYNAROTATION_HXX

#include <iterator>
#include "TFEL/Math/tmatrix.hxx"
#include "MFront/LSDYNA/LSDYNAConfig.hxx"

namespace lsdyna {

  template <typename real>
  struct TFEL_VISIBILITY_LOCAL LSDYNARotation2D {
    template <typename InputIterator>
    LSDYNARotation2D(const InputIterator);
    // Compute stresses in the material space
    void rotateStressesForward(const real *const, real *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const real *const, real *const);
    // compute the deformation dradient in the material space
    void rotateDeformationGradientForward(const real *const, real *const);
    // Compute strain in the material space
    void rotateStrainsForward(const real *const, real *const);
    // Compute stresses back in the global space
    void rotateStrainsBackward(const real *const, real *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(real *const) const;

   private:
    const real a[4];

  };  // end of struct LSDYNARotation2D

  template <typename real>
  struct TFEL_VISIBILITY_LOCAL LSDYNARotation3D {
    template <typename InputIterator>
    LSDYNARotation3D(const InputIterator);

    // Compute stresses in the material space
    void rotateStressesForward(const real *const, real *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const real *const, real *const);
    // compute the deformation dradient in the material space
    void rotateDeformationGradientForward(const real *const, real *const);
    // Compute strain in the material space
    void rotateStrainsForward(const real *const, real *const);
    // Compute stresses back in the global space
    void rotateStrainsBackward(const real *const, real *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(real *const) const;

   private:
    const real a[9];

  };  // end of struct LSDYNARotation3D

  template <typename InputIterator>
  tfel::math::
      tmatrix<3u, 3u, typename std::iterator_traits<InputIterator>::value_type>
      getRotationMatrix2D(const InputIterator);

  template <typename InputIterator>
  tfel::math::
      tmatrix<3u, 3u, typename std::iterator_traits<InputIterator>::value_type>
      getRotationMatrix3D(const InputIterator);

}  // end of namespace lsdyna

#include "MFront/LSDYNA/LSDYNARotation.ixx"

#endif /* LIB_MFRONT_LSDYNA_LSDYNAROTATION_HXX */
