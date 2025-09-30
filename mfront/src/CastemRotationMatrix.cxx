/*!
 * \file   mfront/src/CastemRotationMatrix.cxx
 * \brief
 * \author E. Castelier
 * \date   17/03/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/Castem/CastemRotationMatrix.hxx"

namespace castem {
  CastemRotationMatrix2D::CastemRotationMatrix2D(const CastemReal *const V,
                                                 const CastemReal *const drot) {
    // Premier vecteur
    // a[1,1]
    a[0] = drot[0] * V[0] + drot[3] * V[1];
    // a[2,1]
    a[1] = drot[1] * V[0] + drot[4] * V[1];
    // Deuxième vecteur :
    //   Produit vectoriel
    // a[1,2]
    a[2] = -a[1];
    // a[2,2]
    a[3] = a[0];
  }  // end of CastemRotationMatrix2D::CastemRotationMatrix2D

  // Compute strains in the material space
  void CastemRotationMatrix2D::rotateStrainsForward(const CastemReal *const e,
                                                    CastemReal *const em) {
    em[0] = a[0] * a[1] * e[3] + a[1] * a[1] * e[1] + a[0] * a[0] * e[0];
    em[1] = a[2] * a[3] * e[3] + a[3] * a[3] * e[1] + a[2] * a[2] * e[0];
    em[2] = e[2];
    em[3] = (a[0] * a[3] + a[1] * a[2]) * e[3] + 2 * a[1] * a[3] * e[1] +
            2 * a[0] * a[2] * e[0];
  }  // end of CastemRotationMatrix2D::rotateStrainsForward

  // Compute strains back in the global space
  void CastemRotationMatrix2D::rotateStrainsBackward(const CastemReal *const e,
                                                     CastemReal *const eg) {
    eg[0] = a[0] * a[2] * e[3] + a[2] * a[2] * e[1] + a[0] * a[0] * e[0];
    eg[1] = a[1] * a[3] * e[3] + a[3] * a[3] * e[1] + a[1] * a[1] * e[0];
    eg[2] = e[2];
    eg[3] = (a[0] * a[3] + a[1] * a[2]) * e[3] + 2 * a[2] * a[3] * e[1] +
            2 * a[0] * a[1] * e[0];
  }  // end of CastemRotationMatrix2D::rotateStrainsBackward

  // Compute stresses in the material space
  void CastemRotationMatrix2D::rotateStressesForward(const CastemReal *const s,
                                                     CastemReal *const sm) {
    sm[0] = 2 * a[0] * a[1] * s[3] + a[1] * a[1] * s[1] + a[0] * a[0] * s[0];
    sm[1] = 2 * a[2] * a[3] * s[3] + a[3] * a[3] * s[1] + a[2] * a[2] * s[0];
    sm[2] = s[2];
    sm[3] = (a[0] * a[3] + a[1] * a[2]) * s[3] + a[1] * a[3] * s[1] +
            a[0] * a[2] * s[0];
  }  // end of CastemRotationMatrix2D::rotateStressesForward

  // Compute stresses back in the global space
  void CastemRotationMatrix2D::rotateStressesBackward(const CastemReal *const s,
                                                      CastemReal *const sg) {
    sg[0] = 2 * a[0] * a[2] * s[3] + a[2] * a[2] * s[1] + a[0] * a[0] * s[0];
    sg[1] = 2 * a[1] * a[3] * s[3] + a[3] * a[3] * s[1] + a[1] * a[1] * s[0];
    sg[2] = s[2];
    sg[3] = (a[0] * a[3] + a[1] * a[2]) * s[3] + a[2] * a[3] * s[1] +
            a[0] * a[1] * s[0];
  }  // end of CastemRotationMatrix2D::rotateStressesBackward

  void CastemRotationMatrix2D::rotateDeformationGradientForward(
      const CastemReal *const F, CastemReal *const Fm) {
    Fm[0] = a[1] * a[1] * F[4] + a[0] * a[1] * F[3] + a[0] * a[1] * F[1] +
            a[0] * a[0] * F[0];
    Fm[1] = a[1] * a[3] * F[4] + a[1] * a[2] * F[3] + a[0] * a[3] * F[1] +
            a[0] * a[2] * F[0];
    Fm[2] = 0;
    Fm[3] = a[1] * a[3] * F[4] + a[0] * a[3] * F[3] + a[1] * a[2] * F[1] +
            a[0] * a[2] * F[0];
    Fm[4] = a[3] * a[3] * F[4] + a[2] * a[3] * F[3] + a[2] * a[3] * F[1] +
            a[2] * a[2] * F[0];
    Fm[5] = 0;
    Fm[6] = 0;
    Fm[7] = 0;
    Fm[8] = F[8];
  }  // end of CastemRotationMatrix2D::rotateDeformationGradientForward

  void CastemRotationMatrix2D::rotateTangentOperatorBackward(
      CastemReal *const D) const {
    CastemReal MN[4][4];
    // Première ligne
    MN[0][0] = a[0] * a[0];
    MN[0][1] = a[1] * a[1];
    MN[0][2] = a[0] * a[1];
    // Deuxième ligne
    MN[1][0] = a[2] * a[2];
    MN[1][1] = a[3] * a[3];
    MN[1][2] = a[2] * a[3];
    // Troisième ligne
    MN[2][0] = a[0] * a[2];
    MN[2][1] = a[1] * a[3];
    MN[2][2] = a[0] * a[3] + a[1] * a[2];
    // matrice N
    CastemReal N[4][4];
    for (unsigned short i = 0; i != 2; ++i) {
      for (unsigned short j = 0; j != 2; ++j) {
        N[i][j] = MN[i][j];
      }
    }
    N[2][0] = 0.;
    N[2][1] = 0.;
    N[2][3] = 0.;
    N[0][2] = 0.;
    N[1][2] = 0.;
    N[3][2] = 0.;
    N[2][2] = 1.;
    for (unsigned short i = 0; i != 2; ++i) {
      N[3][i] = MN[2][i];
      N[i][3] = MN[i][2];
    }
    N[3][3] = MN[2][2];
    N[3][0] *= 2;
    N[3][1] *= 2;
    N[3][2] *= 2;
    // matrice temporaire
    CastemReal t[4][4];
    for (unsigned short i = 0; i != 4; ++i) {
      for (unsigned short j = 0; j != 4; ++j) {
        t[i][j] = 0.;
        for (unsigned short k = 0; k != 4; ++k) {
          t[i][j] += D[k * 4 + i] * (N[k][j]);
        }
      }
    }
    // matrice finale
    for (unsigned short i = 0; i != 4; ++i) {
      for (unsigned short j = 0; j != 4; ++j) {
        D[j * 4 + i] = 0.;
        for (unsigned short k = 0; k != 4; ++k) {
          D[j * 4 + i] += N[k][i] * t[k][j];
        }
      }
    }
  }  // end of CastemRotationMatrix2D::rotateTangentOperatorBackward

  CastemRotationMatrix3D::CastemRotationMatrix3D(const CastemReal *const V,
                                                 const CastemReal *const drot) {
    // Premier vecteur
    a[0] = drot[0] * V[0] + drot[3] * V[1] + drot[6] * V[2];
    a[1] = drot[1] * V[0] + drot[4] * V[1] + drot[7] * V[2];
    a[2] = drot[2] * V[0] + drot[5] * V[1] + drot[8] * V[2];
    // Deuxième vecteur
    a[3] = drot[0] * V[3] + drot[3] * V[4] + drot[6] * V[5];
    a[4] = drot[1] * V[3] + drot[4] * V[4] + drot[7] * V[5];
    a[5] = drot[2] * V[3] + drot[5] * V[4] + drot[8] * V[5];
    // Troisième vecteur :
    //   produit vectoriel des deux premiers
    a[6] = a[1] * a[5] - a[4] * a[2];
    a[7] = a[2] * a[3] - a[5] * a[0];
    a[8] = a[0] * a[4] - a[3] * a[1];
  }  // end of CastemRotationMatrix3D::CastemRotationMatrix3D

  // Compute strains in the material space
  void CastemRotationMatrix3D::rotateStrainsForward(const CastemReal *const e,
                                                    CastemReal *const em) {
    em[0] = a[1] * a[2] * e[5] + a[0] * a[2] * e[4] + a[0] * a[1] * e[3] +
            a[2] * a[2] * e[2] + a[1] * a[1] * e[1] + a[0] * a[0] * e[0];
    em[1] = a[4] * a[5] * e[5] + a[3] * a[5] * e[4] + a[3] * a[4] * e[3] +
            a[5] * a[5] * e[2] + a[4] * a[4] * e[1] + a[3] * a[3] * e[0];
    em[2] = a[7] * a[8] * e[5] + a[6] * a[8] * e[4] + a[6] * a[7] * e[3] +
            a[8] * a[8] * e[2] + a[7] * a[7] * e[1] + a[6] * a[6] * e[0];
    em[3] = (a[1] * a[5] + a[2] * a[4]) * e[5] +
            (a[0] * a[5] + a[2] * a[3]) * e[4] +
            (a[0] * a[4] + a[1] * a[3]) * e[3] + 2 * a[2] * a[5] * e[2] +
            2 * a[1] * a[4] * e[1] + 2 * a[0] * a[3] * e[0];
    em[4] = (a[1] * a[8] + a[2] * a[7]) * e[5] +
            (a[0] * a[8] + a[2] * a[6]) * e[4] +
            (a[0] * a[7] + a[1] * a[6]) * e[3] + 2 * a[2] * a[8] * e[2] +
            2 * a[1] * a[7] * e[1] + 2 * a[0] * a[6] * e[0];
    em[5] = (a[4] * a[8] + a[5] * a[7]) * e[5] +
            (a[3] * a[8] + a[5] * a[6]) * e[4] +
            (a[3] * a[7] + a[4] * a[6]) * e[3] + 2 * a[5] * a[8] * e[2] +
            2 * a[4] * a[7] * e[1] + 2 * a[3] * a[6] * e[0];
  }  // end of CastemRotationMatrix3D::rotateStrainsForward

  // Compute strains back in the global space
  void CastemRotationMatrix3D::rotateStrainsBackward(const CastemReal *const e,
                                                     CastemReal *const eg) {
    eg[0] = a[3] * a[6] * e[5] + a[0] * a[6] * e[4] + a[0] * a[3] * e[3] +
            a[6] * a[6] * e[2] + a[3] * a[3] * e[1] + a[0] * a[0] * e[0];
    eg[1] = a[4] * a[7] * e[5] + a[1] * a[7] * e[4] + a[1] * a[4] * e[3] +
            a[7] * a[7] * e[2] + a[4] * a[4] * e[1] + a[1] * a[1] * e[0];
    eg[2] = a[5] * a[8] * e[5] + a[2] * a[8] * e[4] + a[2] * a[5] * e[3] +
            a[8] * a[8] * e[2] + a[5] * a[5] * e[1] + a[2] * a[2] * e[0];
    eg[3] = (a[3] * a[7] + a[4] * a[6]) * e[5] +
            (a[0] * a[7] + a[1] * a[6]) * e[4] +
            (a[0] * a[4] + a[1] * a[3]) * e[3] + 2 * a[6] * a[7] * e[2] +
            2 * a[3] * a[4] * e[1] + 2 * a[0] * a[1] * e[0];
    eg[4] = (a[3] * a[8] + a[5] * a[6]) * e[5] +
            (a[0] * a[8] + a[2] * a[6]) * e[4] +
            (a[0] * a[5] + a[2] * a[3]) * e[3] + 2 * a[6] * a[8] * e[2] +
            2 * a[3] * a[5] * e[1] + 2 * a[0] * a[2] * e[0];
    eg[5] = (a[4] * a[8] + a[5] * a[7]) * e[5] +
            (a[1] * a[8] + a[2] * a[7]) * e[4] +
            (a[1] * a[5] + a[2] * a[4]) * e[3] + 2 * a[7] * a[8] * e[2] +
            2 * a[4] * a[5] * e[1] + 2 * a[1] * a[2] * e[0];
  }  // end of CastemRotationMatrix3D::rotateStrainsBackward

  // Compute stresses in the material space
  void CastemRotationMatrix3D::rotateStressesForward(const CastemReal *const s,
                                                     CastemReal *const sm) {
    sm[0] = 2 * a[1] * a[2] * s[5] + 2 * a[0] * a[2] * s[4] +
            2 * a[0] * a[1] * s[3] + a[2] * a[2] * s[2] + a[1] * a[1] * s[1] +
            a[0] * a[0] * s[0];
    sm[1] = 2 * a[4] * a[5] * s[5] + 2 * a[3] * a[5] * s[4] +
            2 * a[3] * a[4] * s[3] + a[5] * a[5] * s[2] + a[4] * a[4] * s[1] +
            a[3] * a[3] * s[0];
    sm[2] = 2 * a[7] * a[8] * s[5] + 2 * a[6] * a[8] * s[4] +
            2 * a[6] * a[7] * s[3] + a[8] * a[8] * s[2] + a[7] * a[7] * s[1] +
            a[6] * a[6] * s[0];
    sm[3] = (a[1] * a[5] + a[2] * a[4]) * s[5] +
            (a[2] * a[3] + a[0] * a[5]) * s[4] +
            (a[0] * a[4] + a[1] * a[3]) * s[3] + a[2] * a[5] * s[2] +
            a[1] * a[4] * s[1] + a[0] * a[3] * s[0];
    sm[4] = (a[1] * a[8] + a[2] * a[7]) * s[5] +
            (a[2] * a[6] + a[0] * a[8]) * s[4] +
            (a[0] * a[7] + a[1] * a[6]) * s[3] + a[2] * a[8] * s[2] +
            a[1] * a[7] * s[1] + a[0] * a[6] * s[0];
    sm[5] = (a[4] * a[8] + a[5] * a[7]) * s[5] +
            (a[5] * a[6] + a[3] * a[8]) * s[4] +
            (a[3] * a[7] + a[4] * a[6]) * s[3] + a[5] * a[8] * s[2] +
            a[4] * a[7] * s[1] + a[3] * a[6] * s[0];
  }  // end of CastemRotationMatrix3D::rotateStressesForward

  // Compute stresses back in the global space
  void CastemRotationMatrix3D::rotateStressesBackward(const CastemReal *const s,
                                                      CastemReal *const sg) {
    sg[0] = 2 * a[3] * a[6] * s[5] + 2 * a[0] * a[6] * s[4] +
            2 * a[0] * a[3] * s[3] + a[6] * a[6] * s[2] + a[3] * a[3] * s[1] +
            a[0] * a[0] * s[0];
    sg[1] = 2 * a[4] * a[7] * s[5] + 2 * a[1] * a[7] * s[4] +
            2 * a[1] * a[4] * s[3] + a[7] * a[7] * s[2] + a[4] * a[4] * s[1] +
            a[1] * a[1] * s[0];
    sg[2] = 2 * a[5] * a[8] * s[5] + 2 * a[2] * a[8] * s[4] +
            2 * a[2] * a[5] * s[3] + a[8] * a[8] * s[2] + a[5] * a[5] * s[1] +
            a[2] * a[2] * s[0];
    sg[3] = (a[3] * a[7] + a[4] * a[6]) * s[5] +
            (a[1] * a[6] + a[0] * a[7]) * s[4] +
            (a[0] * a[4] + a[1] * a[3]) * s[3] + a[6] * a[7] * s[2] +
            a[3] * a[4] * s[1] + a[0] * a[1] * s[0];
    sg[4] = (a[3] * a[8] + a[5] * a[6]) * s[5] +
            (a[2] * a[6] + a[0] * a[8]) * s[4] +
            (a[0] * a[5] + a[2] * a[3]) * s[3] + a[6] * a[8] * s[2] +
            a[3] * a[5] * s[1] + a[0] * a[2] * s[0];
    sg[5] = (a[4] * a[8] + a[5] * a[7]) * s[5] +
            (a[2] * a[7] + a[1] * a[8]) * s[4] +
            (a[1] * a[5] + a[2] * a[4]) * s[3] + a[7] * a[8] * s[2] +
            a[4] * a[5] * s[1] + a[1] * a[2] * s[0];
  }  // end of CastemRotationMatrix3D::rotateStressesBackward

  void CastemRotationMatrix3D::rotateDeformationGradientForward(
      const CastemReal *const F, CastemReal *const Fm) {
    Fm[0] = a[2] * a[2] * F[8] + a[1] * a[2] * F[7] + a[0] * a[2] * F[6] +
            a[1] * a[2] * F[5] + a[1] * a[1] * F[4] + a[0] * a[1] * F[3] +
            a[0] * a[2] * F[2] + a[0] * a[1] * F[1] + a[0] * a[0] * F[0];
    Fm[1] = a[2] * a[5] * F[8] + a[2] * a[4] * F[7] + a[2] * a[3] * F[6] +
            a[1] * a[5] * F[5] + a[1] * a[4] * F[4] + a[1] * a[3] * F[3] +
            a[0] * a[5] * F[2] + a[0] * a[4] * F[1] + a[0] * a[3] * F[0];
    Fm[2] = a[2] * a[8] * F[8] + a[2] * a[7] * F[7] + a[2] * a[6] * F[6] +
            a[1] * a[8] * F[5] + a[1] * a[7] * F[4] + a[1] * a[6] * F[3] +
            a[0] * a[8] * F[2] + a[0] * a[7] * F[1] + a[0] * a[6] * F[0];
    Fm[3] = a[2] * a[5] * F[8] + a[1] * a[5] * F[7] + a[0] * a[5] * F[6] +
            a[2] * a[4] * F[5] + a[1] * a[4] * F[4] + a[0] * a[4] * F[3] +
            a[2] * a[3] * F[2] + a[1] * a[3] * F[1] + a[0] * a[3] * F[0];
    Fm[4] = a[5] * a[5] * F[8] + a[4] * a[5] * F[7] + a[3] * a[5] * F[6] +
            a[4] * a[5] * F[5] + a[4] * a[4] * F[4] + a[3] * a[4] * F[3] +
            a[3] * a[5] * F[2] + a[3] * a[4] * F[1] + a[3] * a[3] * F[0];
    Fm[5] = a[5] * a[8] * F[8] + a[5] * a[7] * F[7] + a[5] * a[6] * F[6] +
            a[4] * a[8] * F[5] + a[4] * a[7] * F[4] + a[4] * a[6] * F[3] +
            a[3] * a[8] * F[2] + a[3] * a[7] * F[1] + a[3] * a[6] * F[0];
    Fm[6] = a[2] * a[8] * F[8] + a[1] * a[8] * F[7] + a[0] * a[8] * F[6] +
            a[2] * a[7] * F[5] + a[1] * a[7] * F[4] + a[0] * a[7] * F[3] +
            a[2] * a[6] * F[2] + a[1] * a[6] * F[1] + a[0] * a[6] * F[0];
    Fm[7] = a[5] * a[8] * F[8] + a[4] * a[8] * F[7] + a[3] * a[8] * F[6] +
            a[5] * a[7] * F[5] + a[4] * a[7] * F[4] + a[3] * a[7] * F[3] +
            a[5] * a[6] * F[2] + a[4] * a[6] * F[1] + a[3] * a[6] * F[0];
    Fm[8] = a[8] * a[8] * F[8] + a[7] * a[8] * F[7] + a[6] * a[8] * F[6] +
            a[7] * a[8] * F[5] + a[7] * a[7] * F[4] + a[6] * a[7] * F[3] +
            a[6] * a[8] * F[2] + a[6] * a[7] * F[1] + a[6] * a[6] * F[0];
  }  // end of CastemRotationMatrix3D::rotateDeformationGradientForward

  void CastemRotationMatrix3D::rotateDeformationGradientBackward(
      const CastemReal *const F, CastemReal *const Fg) {
    Fg[0] = a[6] * a[6] * F[8] + a[3] * a[6] * F[7] + a[0] * a[6] * F[6] +
            a[3] * a[6] * F[5] + a[3] * a[3] * F[4] + a[0] * a[3] * F[3] +
            a[0] * a[6] * F[2] + a[0] * a[3] * F[1] + a[0] * a[0] * F[0];
    Fg[1] = a[6] * a[7] * F[8] + a[4] * a[6] * F[7] + a[1] * a[6] * F[6] +
            a[3] * a[7] * F[5] + a[3] * a[4] * F[4] + a[1] * a[3] * F[3] +
            a[0] * a[7] * F[2] + a[0] * a[4] * F[1] + a[0] * a[1] * F[0];
    Fg[2] = a[6] * a[8] * F[8] + a[5] * a[6] * F[7] + a[2] * a[6] * F[6] +
            a[3] * a[8] * F[5] + a[3] * a[5] * F[4] + a[2] * a[3] * F[3] +
            a[0] * a[8] * F[2] + a[0] * a[5] * F[1] + a[0] * a[2] * F[0];
    Fg[3] = a[6] * a[7] * F[8] + a[3] * a[7] * F[7] + a[0] * a[7] * F[6] +
            a[4] * a[6] * F[5] + a[3] * a[4] * F[4] + a[0] * a[4] * F[3] +
            a[1] * a[6] * F[2] + a[1] * a[3] * F[1] + a[0] * a[1] * F[0];
    Fg[4] = a[7] * a[7] * F[8] + a[4] * a[7] * F[7] + a[1] * a[7] * F[6] +
            a[4] * a[7] * F[5] + a[4] * a[4] * F[4] + a[1] * a[4] * F[3] +
            a[1] * a[7] * F[2] + a[1] * a[4] * F[1] + a[1] * a[1] * F[0];
    Fg[5] = a[7] * a[8] * F[8] + a[5] * a[7] * F[7] + a[2] * a[7] * F[6] +
            a[4] * a[8] * F[5] + a[4] * a[5] * F[4] + a[2] * a[4] * F[3] +
            a[1] * a[8] * F[2] + a[1] * a[5] * F[1] + a[1] * a[2] * F[0];
    Fg[6] = a[6] * a[8] * F[8] + a[3] * a[8] * F[7] + a[0] * a[8] * F[6] +
            a[5] * a[6] * F[5] + a[3] * a[5] * F[4] + a[0] * a[5] * F[3] +
            a[2] * a[6] * F[2] + a[2] * a[3] * F[1] + a[0] * a[2] * F[0];
    Fg[7] = a[7] * a[8] * F[8] + a[4] * a[8] * F[7] + a[1] * a[8] * F[6] +
            a[5] * a[7] * F[5] + a[4] * a[5] * F[4] + a[1] * a[5] * F[3] +
            a[2] * a[7] * F[2] + a[2] * a[4] * F[1] + a[1] * a[2] * F[0];
    Fg[8] = a[8] * a[8] * F[8] + a[5] * a[8] * F[7] + a[2] * a[8] * F[6] +
            a[5] * a[8] * F[5] + a[5] * a[5] * F[4] + a[2] * a[5] * F[3] +
            a[2] * a[8] * F[2] + a[2] * a[5] * F[1] + a[2] * a[2] * F[0];
  }

  void CastemRotationMatrix3D::rotateTangentOperatorBackward(
      CastemReal *const D) const {
    CastemReal MN[6][6];
    // Contruction de la matrice de passage N (pour les tenseurs)
    // Première ligne
    MN[0][0] = a[0] * a[0];
    MN[0][1] = a[1] * a[1];
    MN[0][2] = a[2] * a[2];
    MN[0][5] = a[1] * a[2];
    MN[0][4] = a[2] * a[0];
    MN[0][3] = a[0] * a[1];
    // Deuxième ligne
    MN[1][0] = a[3] * a[3];
    MN[1][1] = a[4] * a[4];
    MN[1][2] = a[5] * a[5];
    MN[1][5] = a[4] * a[5];
    MN[1][4] = a[5] * a[3];
    MN[1][3] = a[3] * a[4];
    // Troisième ligne
    MN[2][0] = a[6] * a[6];
    MN[2][1] = a[7] * a[7];
    MN[2][2] = a[8] * a[8];
    MN[2][5] = a[7] * a[8];
    MN[2][4] = a[8] * a[6];
    MN[2][3] = a[6] * a[7];
    // Quatrième ligne
    MN[5][0] = a[3] * a[6];
    MN[5][1] = a[4] * a[7];
    MN[5][2] = a[5] * a[8];
    MN[5][5] = a[4] * a[8] + a[5] * a[7];
    MN[5][4] = a[5] * a[6] + a[3] * a[8];
    MN[5][3] = a[3] * a[7] + a[4] * a[6];
    // Cinquième ligne
    MN[4][0] = a[6] * a[0];
    MN[4][1] = a[7] * a[1];
    MN[4][2] = a[8] * a[2];
    MN[4][5] = a[7] * a[2] + a[8] * a[1];
    MN[4][4] = a[8] * a[0] + a[6] * a[2];
    MN[4][3] = a[6] * a[1] + a[7] * a[0];
    // Sixième ligne
    MN[3][0] = a[0] * a[3];
    MN[3][1] = a[1] * a[4];
    MN[3][2] = a[2] * a[5];
    MN[3][5] = a[1] * a[5] + a[2] * a[4];
    MN[3][4] = a[2] * a[3] + a[0] * a[5];
    MN[3][3] = a[0] * a[4] + a[1] * a[3];
    // matrice N
    CastemReal N[6][6];
    for (unsigned short i = 0; i != 6; ++i) {
      for (unsigned short j = 0; j != 6; ++j) {
        N[i][j] = MN[i][j];
      }
    }
    N[3][0] *= 2;
    N[3][1] *= 2;
    N[3][2] *= 2;
    N[4][0] *= 2;
    N[4][1] *= 2;
    N[4][2] *= 2;
    N[5][0] *= 2;
    N[5][1] *= 2;
    N[5][2] *= 2;
    // matrice temporaire
    CastemReal t[6][6];
    for (unsigned short i = 0; i != 6; ++i) {
      for (unsigned short j = 0; j != 6; ++j) {
        t[i][j] = 0.;
        for (unsigned short k = 0; k != 6; ++k) {
          t[i][j] += D[k * 6 + i] * (N[k][j]);
        }
      }
    }
    // matrice finale
    for (unsigned short i = 0; i != 6; ++i) {
      for (unsigned short j = 0; j != 6; ++j) {
        D[j * 6 + i] = 0.;
        for (unsigned short k = 0; k != 6; ++k) {
          D[j * 6 + i] += N[k][i] * t[k][j];
        }
      }
    }
  }  // end of CastemRotationMatrix3D::rotateTangentOperatorBackward

}  // end of namespace castem
