/*!
 * \file  mfront/src/AsterFiniteStrainRotationMatrix.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 26 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Math/General/MathConstants.hxx"
#include "MFront/Aster/AsterFiniteStrainRotationMatrix.hxx"

namespace aster {

  AsterFiniteStrainRotationMatrix2D::AsterFiniteStrainRotationMatrix2D(
      const AsterReal *const drot) {
    // Premier vecteur
    // a[1,1]
    a[0] = drot[0];
    // a[2,1]
    a[1] = drot[1];
    // Deuxième vecteur :
    //   Produit vectoriel
    // a[1,2]
    a[2] = -a[1];
    // a[2,2]
    a[3] = a[0];
  }  // end of
     // AsterFiniteStrainRotationMatrix2D::AsterFiniteStrainRotationMatrix2D

  // Compute strains in the material space
  void AsterFiniteStrainRotationMatrix2D::rotateStrainsForward(
      const AsterReal *const e, AsterReal *const em) {
    em[0] = a[0] * a[1] * e[3] + a[1] * a[1] * e[1] + a[0] * a[0] * e[0];
    em[1] = a[2] * a[3] * e[3] + a[3] * a[3] * e[1] + a[2] * a[2] * e[0];
    em[2] = e[2];
    em[3] = (a[0] * a[3] + a[1] * a[2]) * e[3] + 2 * a[1] * a[3] * e[1] +
            2 * a[0] * a[2] * e[0];
  }  // end of AsterFiniteStrainRotationMatrix2D::rotateStrainsForward

  // Compute strains back in the global space
  void AsterFiniteStrainRotationMatrix2D::rotateStrainsBackward(
      const AsterReal *const e, AsterReal *const eg) {
    eg[0] = a[0] * a[2] * e[3] + a[2] * a[2] * e[1] + a[0] * a[0] * e[0];
    eg[1] = a[1] * a[3] * e[3] + a[3] * a[3] * e[1] + a[1] * a[1] * e[0];
    eg[2] = e[2];
    eg[3] = (a[0] * a[3] + a[1] * a[2]) * e[3] + 2 * a[2] * a[3] * e[1] +
            2 * a[0] * a[1] * e[0];
  }  // end of AsterFiniteStrainRotationMatrix2D::rotateStrainsBackward

  // Compute stresses in the material space
  void AsterFiniteStrainRotationMatrix2D::rotateStressesForward(
      const AsterReal *const s, AsterReal *const sm) {
    sm[0] = 2 * a[0] * a[1] * s[3] + a[1] * a[1] * s[1] + a[0] * a[0] * s[0];
    sm[1] = 2 * a[2] * a[3] * s[3] + a[3] * a[3] * s[1] + a[2] * a[2] * s[0];
    sm[2] = s[2];
    sm[3] = (a[0] * a[3] + a[1] * a[2]) * s[3] + a[1] * a[3] * s[1] +
            a[0] * a[2] * s[0];
  }  // end of AsterFiniteStrainRotationMatrix2D::rotateStressesForward

  // Compute stresses back in the global space
  void AsterFiniteStrainRotationMatrix2D::rotateStressesBackward(
      const AsterReal *const s, AsterReal *const sg) {
    sg[0] = 2 * a[0] * a[2] * s[3] + a[2] * a[2] * s[1] + a[0] * a[0] * s[0];
    sg[1] = 2 * a[1] * a[3] * s[3] + a[3] * a[3] * s[1] + a[1] * a[1] * s[0];
    sg[2] = s[2];
    sg[3] = (a[0] * a[3] + a[1] * a[2]) * s[3] + a[2] * a[3] * s[1] +
            a[0] * a[1] * s[0];
  }  // end of AsterFiniteStrainRotationMatrix2D::rotateStressesBackward

  void AsterFiniteStrainRotationMatrix2D::rotateDeformationGradientForward(
      const AsterReal *const F, AsterReal *const Fg) {
    Fg[0] = a[1] * a[1] * F[4] + a[0] * a[1] * F[3] + a[0] * a[1] * F[1] +
            a[0] * a[0] * F[0];
    Fg[1] = a[1] * a[3] * F[4] + a[1] * a[2] * F[3] + a[0] * a[3] * F[1] +
            a[0] * a[2] * F[0];
    Fg[3] = 0;
    Fg[4] = a[1] * a[3] * F[4] + a[0] * a[3] * F[3] + a[1] * a[2] * F[1] +
            a[0] * a[2] * F[0];
    Fg[5] = a[3] * a[3] * F[4] + a[2] * a[3] * F[3] + a[2] * a[3] * F[1] +
            a[2] * a[2] * F[0];
    Fg[6] = 0;
    Fg[7] = 0;
    Fg[8] = F[8];
  }  // end of
     // AsterFiniteStrainRotationMatrix2D::rotateDeformationGradientForward

  void AsterFiniteStrainRotationMatrix2D::rotateTangentOperatorBackward(
      AsterReal *const) const {
    throw(std::runtime_error(
        "AsterFiniteStrainRotationMatrix2D::rotateTangentOperatorBackward : "
        "unimplemented feature"));
  }  // end of AsterFiniteStrainRotationMatrix2D::rotateTangentOperatorBackward

  AsterFiniteStrainRotationMatrix3D::AsterFiniteStrainRotationMatrix3D(
      const AsterReal *const drot) {
    // Premier vecteur
    a[0] = drot[0];
    a[1] = drot[1];
    a[2] = drot[2];
    // Deuxième vecteur
    a[3] = drot[3];
    a[4] = drot[4];
    a[5] = drot[5];
    // Troisième vecteur :
    //   produit vectoriel des deux premiers
    a[6] = a[1] * a[5] - a[4] * a[2];
    a[7] = a[2] * a[3] - a[5] * a[0];
    a[8] = a[0] * a[4] - a[3] * a[1];
  }  // end of
     // AsterFiniteStrainRotationMatrix3D::AsterFiniteStrainRotationMatrix3D

  // Compute strains in the material space
  void AsterFiniteStrainRotationMatrix3D::rotateStrainsForward(
      const AsterReal *const e, AsterReal *const em) {
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
  }  // end of AsterFiniteStrainRotationMatrix3D::rotateStrainsForward

  // Compute strains back in the global space
  void AsterFiniteStrainRotationMatrix3D::rotateStrainsBackward(
      const AsterReal *const e, AsterReal *const eg) {
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
  }  // end of AsterFiniteStrainRotationMatrix3D::rotateStrainsBackward

  // Compute stresses in the material space
  void AsterFiniteStrainRotationMatrix3D::rotateStressesForward(
      const AsterReal *const s, AsterReal *const sm) {
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
  }  // end of AsterFiniteStrainRotationMatrix3D::rotateStressesForward

  // Compute stresses back in the global space
  void AsterFiniteStrainRotationMatrix3D::rotateStressesBackward(
      const AsterReal *const s, AsterReal *const sg) {
    // avec les racines de 2
    // tg[0]=cste*a[3]*a[6]*s[5]+cste*a[0]*a[6]*s[4]+cste*a[0]*a[3]*s[3]+a[6]*a[6]*s[2]+a[3]*a[3]*s[1]+a[0]*a[0]*s[0];
    // tg[1]=cste*a[4]*a[7]*s[5]+cste*a[1]*a[7]*s[4]+cste*a[1]*a[4]*s[3]+a[7]*a[7]*s[2]+a[4]*a[4]*s[1]+a[1]*a[1]*s[0];
    // tg[2]=cste*a[5]*a[8]*s[5]+cste*a[2]*a[8]*s[4]+cste*a[2]*a[5]*s[3]+a[8]*a[8]*s[2]+a[5]*a[5]*s[1]+a[2]*a[2]*s[0];
    // tg[3]=(a[3]*a[7]+a[4]*a[6)]*s[5]+(a[0]*a[7]+a[1]*a[6)]*s[4]+(a[0]*a[4]+a[1]*a[3)]*s[3]+cste*a[6]*a[7]*s[2]+cste*a[3]*a[4]*s[1]+cste*a[0]*a[1]*s[0];
    // tg[4]=(a[3]*a[8]+a[5]*a[6)]*s[5]+(a[0]*a[8]+a[2]*a[6)]*s[4]+(a[0]*a[5]+a[2]*a[3)]*s[3]+cste*a[6]*a[8]*s[2]+cste*a[3]*a[5]*s[1]+cste*a[0]*a[2]*s[0];
    // tg[5]=(a[4]*a[8]+a[5]*a[7)]*s[5]+(a[1]*a[8]+a[2]*a[7)]*s[4]+(a[1]*a[5]+a[2]*a[4)]*s[3]+cste*a[7]*a[8]*s[2]+cste*a[4]*a[5]*s[1]+cste*a[1]*a[2]*s[0];
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
  }  // end of AsterFiniteStrainRotationMatrix3D::rotateStressesBackward

  void AsterFiniteStrainRotationMatrix3D::rotateDeformationGradientForward(
      const AsterReal *const F, AsterReal *const Fm) {
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
  }  // end of
     // AsterFiniteStrainRotationMatrix3D::rotateDeformationGradientForward

  void AsterFiniteStrainRotationMatrix3D::rotateDeformationGradientBackward(
      const AsterReal *const F, AsterReal *const Fg) {
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

  static inline unsigned short getRowIndex(const unsigned short i) {
    // Attention, les indices sont ceux du fortran :
    //     (F0 F3 F6)
    // F = (F1 F4 F7)
    //     (F2 F5 F8)
    switch (i) {
      case 0:
        return 0;
      case 1:
        return 1;
      case 2:
        return 2;
      case 3:
        return 0;
      case 4:
        return 1;
      case 5:
        return 2;
      case 6:
        return 0;
      case 7:
        return 1;
      case 8:
        return 2;
    }
    return 0;
  }

  static inline unsigned short getColumnIndex(const unsigned short j) {
    // Attention, les indices sont ceux du fortran :
    //     (F0 F3 F6)
    // F = (F1 F4 F7)
    //     (F2 F5 F8)
    switch (j) {
      case 0:
        return 0;
      case 1:
        return 0;
      case 2:
        return 0;
      case 3:
        return 1;
      case 4:
        return 1;
      case 5:
        return 1;
      case 6:
        return 2;
      case 7:
        return 2;
      case 8:
        return 2;
    }
    return 0;
  }

  void AsterFiniteStrainRotationMatrix3D::rotateTangentOperatorBackward(
      AsterReal *const Dg) const {
    // note sur le rangement fortran :
    // on récupère Dm(k,i,j)=dtau[k]_dF[ij] à l'adresse Dm[k+n1*(i+3*j)]
    using namespace std;
    constexpr const auto cste = tfel::math::Cste<AsterReal>::sqrt2;
    const unsigned short n1 = 6u;  // stensor size
    AsterReal Dm[54];
    copy(Dg, Dg + 54, Dm);
    AsterReal dFm_dFg[9][9];
    AsterReal dtg_dtm[6][6];
    // computing dFm_dFg
    // Attention, les indices sont ceux du fortran :
    //     (F0 F3 F6)
    // F = (F1 F4 F7)
    //     (F2 F5 F8)
    // sinon ce serait trop facile :)
    dFm_dFg[0][8] = a[2] * a[2];
    dFm_dFg[0][7] = a[1] * a[2];
    dFm_dFg[0][6] = a[0] * a[2];
    dFm_dFg[0][5] = a[1] * a[2];
    dFm_dFg[0][4] = a[1] * a[1];
    dFm_dFg[0][3] = a[0] * a[1];
    dFm_dFg[0][2] = a[0] * a[2];
    dFm_dFg[0][1] = a[0] * a[1];
    dFm_dFg[0][0] = a[0] * a[0];
    dFm_dFg[1][8] = a[2] * a[5];
    dFm_dFg[1][7] = a[2] * a[4];
    dFm_dFg[1][6] = a[2] * a[3];
    dFm_dFg[1][5] = a[1] * a[5];
    dFm_dFg[1][4] = a[1] * a[4];
    dFm_dFg[1][3] = a[1] * a[3];
    dFm_dFg[1][2] = a[0] * a[5];
    dFm_dFg[1][1] = a[0] * a[4];
    dFm_dFg[1][0] = a[0] * a[3];
    dFm_dFg[2][8] = a[2] * a[8];
    dFm_dFg[2][7] = a[2] * a[7];
    dFm_dFg[2][6] = a[2] * a[6];
    dFm_dFg[2][5] = a[1] * a[8];
    dFm_dFg[2][4] = a[1] * a[7];
    dFm_dFg[2][3] = a[1] * a[6];
    dFm_dFg[2][2] = a[0] * a[8];
    dFm_dFg[2][1] = a[0] * a[7];
    dFm_dFg[2][0] = a[0] * a[6];
    dFm_dFg[3][8] = a[2] * a[5];
    dFm_dFg[3][7] = a[1] * a[5];
    dFm_dFg[3][6] = a[0] * a[5];
    dFm_dFg[3][5] = a[2] * a[4];
    dFm_dFg[3][4] = a[1] * a[4];
    dFm_dFg[3][3] = a[0] * a[4];
    dFm_dFg[3][2] = a[2] * a[3];
    dFm_dFg[3][1] = a[1] * a[3];
    dFm_dFg[3][0] = a[0] * a[3];
    dFm_dFg[4][8] = a[5] * a[5];
    dFm_dFg[4][7] = a[4] * a[5];
    dFm_dFg[4][6] = a[3] * a[5];
    dFm_dFg[4][5] = a[4] * a[5];
    dFm_dFg[4][4] = a[4] * a[4];
    dFm_dFg[4][3] = a[3] * a[4];
    dFm_dFg[4][2] = a[3] * a[5];
    dFm_dFg[4][1] = a[3] * a[4];
    dFm_dFg[4][0] = a[3] * a[3];
    dFm_dFg[5][8] = a[5] * a[8];
    dFm_dFg[5][7] = a[5] * a[7];
    dFm_dFg[5][6] = a[5] * a[6];
    dFm_dFg[5][5] = a[4] * a[8];
    dFm_dFg[5][4] = a[4] * a[7];
    dFm_dFg[5][3] = a[4] * a[6];
    dFm_dFg[5][2] = a[3] * a[8];
    dFm_dFg[5][1] = a[3] * a[7];
    dFm_dFg[5][0] = a[3] * a[6];
    dFm_dFg[6][8] = a[2] * a[8];
    dFm_dFg[6][7] = a[1] * a[8];
    dFm_dFg[6][6] = a[0] * a[8];
    dFm_dFg[6][5] = a[2] * a[7];
    dFm_dFg[6][4] = a[1] * a[7];
    dFm_dFg[6][3] = a[0] * a[7];
    dFm_dFg[6][2] = a[2] * a[6];
    dFm_dFg[6][1] = a[1] * a[6];
    dFm_dFg[6][0] = a[0] * a[6];
    dFm_dFg[7][8] = a[5] * a[8];
    dFm_dFg[7][7] = a[4] * a[8];
    dFm_dFg[7][6] = a[3] * a[8];
    dFm_dFg[7][5] = a[5] * a[7];
    dFm_dFg[7][4] = a[4] * a[7];
    dFm_dFg[7][3] = a[3] * a[7];
    dFm_dFg[7][2] = a[5] * a[6];
    dFm_dFg[7][1] = a[4] * a[6];
    dFm_dFg[7][0] = a[3] * a[6];
    dFm_dFg[8][8] = a[8] * a[8];
    dFm_dFg[8][7] = a[7] * a[8];
    dFm_dFg[8][6] = a[6] * a[8];
    dFm_dFg[8][5] = a[7] * a[8];
    dFm_dFg[8][4] = a[7] * a[7];
    dFm_dFg[8][3] = a[6] * a[7];
    dFm_dFg[8][2] = a[6] * a[8];
    dFm_dFg[8][1] = a[6] * a[7];
    dFm_dFg[8][0] = a[6] * a[6];
    // computing dtg_dtm
    dtg_dtm[0][5] = cste * a[3] * a[6];
    dtg_dtm[0][4] = cste * a[0] * a[6];
    dtg_dtm[0][3] = cste * a[0] * a[3];
    dtg_dtm[0][2] = a[6] * a[6];
    dtg_dtm[0][1] = a[3] * a[3];
    dtg_dtm[0][0] = a[0] * a[0];
    dtg_dtm[1][5] = cste * a[4] * a[7];
    dtg_dtm[1][4] = cste * a[1] * a[7];
    dtg_dtm[1][3] = cste * a[1] * a[4];
    dtg_dtm[1][2] = a[7] * a[7];
    dtg_dtm[1][1] = a[4] * a[4];
    dtg_dtm[1][0] = a[1] * a[1];
    dtg_dtm[2][5] = cste * a[5] * a[8];
    dtg_dtm[2][4] = cste * a[2] * a[8];
    dtg_dtm[2][3] = cste * a[2] * a[5];
    dtg_dtm[2][2] = a[8] * a[8];
    dtg_dtm[2][1] = a[5] * a[5];
    dtg_dtm[2][0] = a[2] * a[2];
    dtg_dtm[3][5] = a[3] * a[7] + a[4] * a[6];
    dtg_dtm[3][4] = a[0] * a[7] + a[1] * a[6];
    dtg_dtm[3][3] = a[0] * a[4] + a[1] * a[3];
    dtg_dtm[3][2] = cste * a[6] * a[7];
    dtg_dtm[3][1] = cste * a[3] * a[4];
    dtg_dtm[3][0] = cste * a[0] * a[1];
    dtg_dtm[4][5] = a[3] * a[8] + a[5] * a[6];
    dtg_dtm[4][4] = a[0] * a[8] + a[2] * a[6];
    dtg_dtm[4][3] = a[0] * a[5] + a[2] * a[3];
    dtg_dtm[4][2] = cste * a[6] * a[8];
    dtg_dtm[4][1] = cste * a[3] * a[5];
    dtg_dtm[4][0] = cste * a[0] * a[2];
    dtg_dtm[5][5] = a[4] * a[8] + a[5] * a[7];
    dtg_dtm[5][4] = a[1] * a[8] + a[2] * a[7];
    dtg_dtm[5][3] = a[1] * a[5] + a[2] * a[4];
    dtg_dtm[5][2] = cste * a[7] * a[8];
    dtg_dtm[5][1] = cste * a[4] * a[5];
    dtg_dtm[5][0] = cste * a[1] * a[2];
    for (unsigned short i = 0; i != 6; ++i) {
      for (unsigned short j = 0; j != 9; ++j) {
        AsterReal v = AsterReal(0);
        for (unsigned short k = 0; k != 6; ++k) {
          for (unsigned short l = 0; l != 9; ++l) {
            const unsigned short ml_r = getRowIndex(l);
            const unsigned short ml_c = getColumnIndex(l);
            const AsterReal vm = Dm[k + n1 * (ml_r + 3 * ml_c)];
            v += dtg_dtm[i][k] * vm * dFm_dFg[l][j];
          }
        }
        const unsigned short mj_r = getRowIndex(j);
        const unsigned short mj_c = getColumnIndex(j);
        Dg[i + n1 * (mj_r + 3 * mj_c)] = v;
      }
    }
    // string
    // msg("AsterFiniteStrainRotationMatrix3D::rotateTangentOperatorBackward : "
    // 	       "unimplemented feature");
    // throw(runtime_error(msg));
  }  // end of AsterFiniteStrainRotationMatrix3D::rotateTangentOperatorBackward

}  // end of namespace aster
