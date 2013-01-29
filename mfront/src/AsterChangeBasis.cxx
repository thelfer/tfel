/*!
 * \file   AsterChangeBasis.cxx
 * \brief    
 * \author É. Castelier
 * \date   17 mar 2008
 */

#include"MFront/UMAT/AsterChangeBasis.hxx"

namespace umat
{

  // Calcul des déformations dans le repère matériau
  // e^m=N:e^g
  // eg : Déformations dans le repère global
  // em : Déformations dans le repère matériau
  void
  AsterChangeBasis2D::rotateStrainsForward(const UMATReal *const eg,
					   UMATReal *const em) {
    // e11
    em[0]=MN[0][0]*eg[0]+MN[0][1]*eg[1]
      +MN[0][2]*eg[3];
    // e22
    em[1]=MN[1][0]*eg[0]+MN[1][1]*eg[1]
      +MN[1][2]*eg[3];
    // g12
    em[3]=2*(MN[2][0]*eg[0]+MN[2][1]*eg[1])
      +MN[2][2]*eg[3];

    // e33
    em[2]=eg[2];
  } // end of AsterChangeBasis2D::rotateStrainsForward


  // Calcul des contraintes dans le repère global
  // s^g=N^T:s^m
  // sm : Contraintes dans le repère matériau
  // sg : Contraintes dans le repère global
  void
  AsterChangeBasis2D::rotateStressesBackward(const UMATReal *const sm,
					     UMATReal *const sg){
    // s11
    sg[0]=MN[0][0]*sm[0]+MN[1][0]*sm[1]
      +2*MN[2][0]*sm[3];
    // s22
    sg[1]=MN[0][1]*sm[0]+MN[1][1]*sm[1]
      +2*MN[2][1]*sm[3];
    // s12
    sg[3]=MN[0][2]*sm[0]+MN[1][2]*sm[1]
      +MN[2][2]*sm[3];

    // s33
    sg[2]=sm[2];
  } // end of AsterChangeBasis2D::rotateStressesBackward

  // Calcul des contraintes dans le repère matériau
  // s^m=M:s^g
  // sg : Contraintes dans le repère global
  // sm : Contraintes dans le repère matériau
  void
  AsterChangeBasis2D::rotateStressesForward(const UMATReal *const sg,
					    UMATReal *const sm){
    // s11
    sm[0]=MN[0][0]*sg[0]+MN[0][1]*sg[1]
      +2*MN[0][2]*sg[3];
    // s22
    sm[1]=MN[1][0]*sg[0]+MN[1][1]*sg[1]
      +2*MN[1][2]*sg[3];
    // s12
    sm[3]=MN[2][0]*sg[0]+MN[2][1]*sg[1]
      +MN[2][2]*sg[3];

    // s33
    sm[2]=sg[2];
  } // end of AsterChangeBasis2D::rotateStressesForward

  // Calcul des déformations dans le repère global
  // e^g=M^T:e^m
  // em : Contraintes dans le repère matériau
  // eg : Contraintes dans le repère global
  void
  AsterChangeBasis2D::rotateStrainsBackward(const UMATReal *const em,
					    UMATReal *const eg)
  {
    // e11
    eg[0]=MN[0][0]*em[0]+MN[1][0]*em[1]
      +MN[2][0]*em[3];
    // e22
    eg[1]=MN[0][1]*em[0]+MN[1][1]*em[1]
      +MN[2][1]*em[3];
    // g12
    eg[3]=2*(MN[0][2]*em[0]+MN[1][2]*em[1])
      +MN[2][2]*em[3];

    // e33
    eg[2]=em[2];
  } // end of AsterChangeBasis2D:::rotateStrainsBackward

  // Calcul des déformations dans le repère matériau
  // e^m=N:e^g
  // eg : Déformations dans le repère global
  // em : Déformations dans le repère matériau
  void
  AsterChangeBasis3D::rotateStrainsForward(const UMATReal *const eg,
					   UMATReal *const em)
  {
    em[0]=MN[0][0]*eg[0]+MN[0][1]*eg[1]+MN[0][2]*eg[2]
      +MN[0][3]*eg[3]+MN[0][4]*eg[4]+MN[0][5]*eg[5];
    em[1]=MN[1][0]*eg[0]+MN[1][1]*eg[1]+MN[1][2]*eg[2]
      +MN[1][3]*eg[3]+MN[1][4]*eg[4]+MN[1][5]*eg[5];
    em[2]=MN[2][0]*eg[0]+MN[2][1]*eg[1]+MN[2][2]*eg[2]
      +MN[2][3]*eg[3]+MN[2][4]*eg[4]+MN[2][5]*eg[5];
    em[3]=2*(MN[3][0]*eg[0]+MN[3][1]*eg[1]+MN[3][2]*eg[2])
      +MN[3][3]*eg[3]+MN[3][4]*eg[4]+MN[3][5]*eg[5];
    em[4]=2*(MN[4][0]*eg[0]+MN[4][1]*eg[1]+MN[4][2]*eg[2])
      +MN[4][3]*eg[3]+MN[4][4]*eg[4]+MN[4][5]*eg[5];
    em[5]=2*(MN[5][0]*eg[0]+MN[5][1]*eg[1]+MN[5][2]*eg[2])
      +MN[5][3]*eg[3]+MN[5][4]*eg[4]+MN[5][5]*eg[5];
  } // end of AsterChangeBasis3D::rotateStrainForward

  // Calcul des contraintes dans le repère global
  // s^g=N^T:s^m
  // sm : Contraintes dans le repère matériau
  // sg : Contraintes dans le repère global
  void
  AsterChangeBasis3D::rotateStressesBackward(const UMATReal *const sm,
					     UMATReal *const sg){
    sg[0]=MN[0][0]*sm[0]+MN[1][0]*sm[1]+MN[2][0]*sm[2]
      +2*(MN[3][0]*sm[3]+MN[4][0]*sm[4]+MN[5][0]*sm[5]);
    sg[1]=MN[0][1]*sm[0]+MN[1][1]*sm[1]+MN[2][1]*sm[2]
      +2*(MN[3][1]*sm[3]+MN[4][1]*sm[4]+MN[5][1]*sm[5]);
    sg[2]=MN[0][2]*sm[0]+MN[1][2]*sm[1]+MN[2][2]*sm[2]
      +2*(MN[3][2]*sm[3]+MN[4][2]*sm[4]+MN[5][2]*sm[5]);
    sg[3]=MN[0][3]*sm[0]+MN[1][3]*sm[1]+MN[2][3]*sm[2]
      +MN[3][3]*sm[3]+MN[4][3]*sm[4]+MN[5][3]*sm[5];
    sg[4]=MN[0][4]*sm[0]+MN[1][4]*sm[1]+MN[2][4]*sm[2]
      +MN[3][4]*sm[3]+MN[4][4]*sm[4]+MN[5][4]*sm[5];
    sg[5]=MN[0][5]*sm[0]+MN[1][5]*sm[1]+MN[2][5]*sm[2]
      +MN[3][5]*sm[3]+MN[4][5]*sm[4]+MN[5][5]*sm[5];
  } // end of AsterChangeBasis3D::rotateStressesBackward

  // Calcul des contraintes dans le repère matériau
  // s^m=M:s^g
  // sg : Contraintes dans le repère global
  // sm : Contraintes dans le repère matériau
  void
  AsterChangeBasis3D::rotateStressesForward(const UMATReal *const sg,
					    UMATReal *const sm){
    sm[0]=MN[0][0]*sg[0]+MN[0][1]*sg[1]+MN[0][2]*sg[2]
      +2*(MN[0][3]*sg[3]+MN[0][4]*sg[4]+MN[0][5]*sg[5]);
    sm[1]=MN[1][0]*sg[0]+MN[1][1]*sg[1]+MN[1][2]*sg[2]
      +2*(MN[1][3]*sg[3]+MN[1][4]*sg[4]+MN[1][5]*sg[5]);
    sm[2]=MN[2][0]*sg[0]+MN[2][1]*sg[1]+MN[2][2]*sg[2]
      +2*(MN[2][3]*sg[3]+MN[2][4]*sg[4]+MN[2][5]*sg[5]);
    sm[3]=MN[3][0]*sg[0]+MN[3][1]*sg[1]+MN[3][2]*sg[2]
      +MN[3][3]*sg[3]+MN[3][4]*sg[4]+MN[3][5]*sg[5];
    sm[4]=MN[4][0]*sg[0]+MN[4][1]*sg[1]+MN[4][2]*sg[2]
      +MN[4][3]*sg[3]+MN[4][4]*sg[4]+MN[4][5]*sg[5];
    sm[5]=MN[5][0]*sg[0]+MN[5][1]*sg[1]+MN[5][2]*sg[2]
      +MN[5][3]*sg[3]+MN[5][4]*sg[4]+MN[5][5]*sg[5];
  } // end of AsterChangeBasis3D::rotateStressesForward

  // Calcul des déformations dans le repère global
  // e^g=M^T:e^m
  // em : Contraintes dans le repère matériau
  // eg : Contraintes dans le repère global
  void
  AsterChangeBasis3D::rotateStrainsBackward(const UMATReal *const em,
					    UMATReal *const eg)
  {
    eg[0]=MN[0][0]*em[0]+MN[1][0]*em[1]+MN[2][0]*em[2]
      +MN[3][0]*em[3]+MN[4][0]*em[4]+MN[5][0]*em[5];
    eg[1]=MN[0][1]*em[0]+MN[1][1]*em[1]+MN[2][1]*em[2]
      +MN[3][1]*em[3]+MN[4][1]*em[4]+MN[5][1]*em[5];
    eg[2]=MN[0][2]*em[0]+MN[1][2]*em[1]+MN[2][2]*em[2]
      +MN[3][2]*em[3]+MN[4][2]*em[4]+MN[5][2]*em[5];
    eg[3]=2*(MN[0][3]*em[0]+MN[1][3]*em[1]+MN[2][3]*em[2])
      +MN[3][3]*em[3]+MN[4][3]*em[4]+MN[5][3]*em[5];
    eg[4]=2*(MN[0][4]*em[0]+MN[1][4]*em[1]+MN[2][4]*em[2])
      +MN[3][4]*em[3]+MN[4][4]*em[4]+MN[5][4]*em[5];
    eg[5]=2*(MN[0][5]*em[0]+MN[1][5]*em[1]+MN[2][5]*em[2])
      +MN[3][5]*em[3]+MN[4][5]*em[4]+MN[5][5]*em[5];
  } // end of AsterChangeBasis3D::rotateStrainBackward


} // end of namespace umat
