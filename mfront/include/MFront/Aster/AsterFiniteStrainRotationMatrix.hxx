/*!
 * \file  mfront/include/MFront/Aster/AsterFiniteStrainRotationMatrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTER_ASTERFINITESTRAINROTATIONMATRIX_H_
#define LIB_MFRONT_ASTER_ASTERFINITESTRAINROTATIONMATRIX_H_

#include "MFront/Aster/AsterConfig.hxx"
#include "MFront/Aster/Aster.hxx"

namespace aster {

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterFiniteStrainRotationMatrix2D {
    AsterFiniteStrainRotationMatrix2D(const AsterReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const AsterReal *const, AsterReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const AsterReal *const, AsterReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const AsterReal *const, AsterReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const AsterReal *const, AsterReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const AsterReal *const,
                                          AsterReal *const);
    // compute the stiffness matrix in the global space
    TFEL_NORETURN void rotateTangentOperatorBackward(AsterReal *const) const;

   private:
    AsterReal a[4];

  };  // end of struct AsterFiniteStrainRotationMatrix2D

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterFiniteStrainRotationMatrix3D {
    AsterFiniteStrainRotationMatrix3D(const AsterReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const AsterReal *const, AsterReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const AsterReal *const, AsterReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const AsterReal *const, AsterReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const AsterReal *const, AsterReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const AsterReal *const,
                                          AsterReal *const);
    // Compute stresses back in the global space
    void rotateDeformationGradientBackward(const AsterReal *const,
                                           AsterReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(AsterReal *const) const;

   private:
    AsterReal a[9];

  };  // end of struct AsterFiniteStrainRotationMatrix3D

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTER_ASTERFINITESTRAINROTATIONMATRIX_H_ */
