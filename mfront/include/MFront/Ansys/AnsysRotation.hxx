/*!
 * \file   mfront/include/MFront/Ansys/AnsysRotation.hxx
 * \brief  This file declares the AnsysRotationMatrix class
 * \author É. Castelier
 * \date   17 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ANSYS_ANSYSROTATION_HXX
#define LIB_MFRONT_ANSYS_ANSYSROTATION_HXX

#include <iterator>
#include "TFEL/Math/tmatrix.hxx"
#include "MFront/Ansys/AnsysConfig.hxx"

namespace ansys {

  template <typename real>
  struct TFEL_VISIBILITY_LOCAL AnsysRotation2D {
    template <typename InputIterator>
    AnsysRotation2D(const InputIterator);
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

  };  // end of struct AnsysRotation2D

  template <typename real>
  struct TFEL_VISIBILITY_LOCAL AnsysRotation3D {
    template <typename InputIterator>
    AnsysRotation3D(const InputIterator);

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

  };  // end of struct AnsysRotation3D

  template <typename InputIterator>
  tfel::math::
      tmatrix<3u, 3u, typename std::iterator_traits<InputIterator>::value_type>
      getRotationMatrix2D(const InputIterator);

  template <typename InputIterator>
  tfel::math::
      tmatrix<3u, 3u, typename std::iterator_traits<InputIterator>::value_type>
      getRotationMatrix3D(const InputIterator);

}  // end of namespace ansys

#include "MFront/Ansys/AnsysRotation.ixx"

#endif /* LIB_MFRONT_ANSYS_ANSYSROTATION_HXX */
