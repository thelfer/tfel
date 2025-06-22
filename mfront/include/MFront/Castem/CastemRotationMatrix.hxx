/*!
 * \file   mfront/include/MFront/Castem/CastemRotationMatrix.hxx
 * \brief  This file declares the CastemRotationMatrix class
 * \author É. Castelier
 * \date   17 mar 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMROTATIONMATRIX_H_
#define LIB_MFRONT_CASTEMROTATIONMATRIX_H_

#include "MFront/Castem/CastemConfig.hxx"
#include "MFront/Castem/Castem.hxx"

namespace castem {

  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemRotationMatrix2D {
    CastemRotationMatrix2D(const CastemReal *const, const CastemReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const CastemReal *const, CastemReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const CastemReal *const, CastemReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const CastemReal *const, CastemReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const CastemReal *const, CastemReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const CastemReal *const,
                                          CastemReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(CastemReal *const) const;

   private:
    CastemReal a[4];

  };  // end of struct CastemRotationMatrix2D

  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemRotationMatrix3D {
    CastemRotationMatrix3D(const CastemReal *const, const CastemReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const CastemReal *const, CastemReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const CastemReal *const, CastemReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const CastemReal *const, CastemReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const CastemReal *const, CastemReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const CastemReal *const,
                                          CastemReal *const);
    // Compute stresses back in the global space
    void rotateDeformationGradientBackward(const CastemReal *const,
                                           CastemReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(CastemReal *const) const;

   private:
    CastemReal a[9];

  };  // end of struct CastemRotationMatrix3D

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEMROTATIONMATRIX_H_ */
