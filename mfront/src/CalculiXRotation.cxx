/*!
 * \file   CalculiXRotation.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   31 août 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/CalculiX/CalculiXRotation.hxx"

namespace calculix
{

  CalculiXRotation3D::CalculiXRotation3D(const CalculiXReal* const i)
    : a{i[0],i[1],i[2],
        i[3],i[4],i[5],
        i[1]*i[5]-i[4]*i[2],
        i[3]*i[2]-i[5]*i[0],
        i[0]*i[4]-i[3]*i[1]}
  {} // end of CalculiXRotation3D::CalculiXRotation3D

  // Calcul des déformations dans le repère matériau
  // e^m=N:e^g
  // eg : Déformations dans le repère global
  // em : Déformations dans le repère matériau
  void CalculiXRotation3D::rotateStrainsForward(const CalculiXReal *const eg,
						      CalculiXReal *const em)
  {
    em[0]=a[1]*a[2]*eg[5]+a[0]*a[2]*eg[4]+a[0]*a[1]*eg[3]+a[2]*a[2]*eg[2]+a[1]*a[1]*eg[1]+a[0]*a[0]*eg[0];
    em[1]=a[4]*a[5]*eg[5]+a[3]*a[5]*eg[4]+a[3]*a[4]*eg[3]+a[5]*a[5]*eg[2]+a[4]*a[4]*eg[1]+a[3]*a[3]*eg[0];
    em[2]=a[7]*a[8]*eg[5]+a[6]*a[8]*eg[4]+a[6]*a[7]*eg[3]+a[8]*a[8]*eg[2]+a[7]*a[7]*eg[1]+a[6]*a[6]*eg[0];
    em[3]=(a[1]*a[5]+a[2]*a[4])*eg[5]+(a[0]*a[5]+a[2]*a[3])*eg[4]+(a[0]*a[4]+a[1]*a[3])*eg[3]+2*a[2]*a[5]*eg[2]+2*a[1]*a[4]*eg[1]+2*a[0]*a[3]*eg[0];
    em[4]=(a[1]*a[8]+a[2]*a[7])*eg[5]+(a[0]*a[8]+a[2]*a[6])*eg[4]+(a[0]*a[7]+a[1]*a[6])*eg[3]+2*a[2]*a[8]*eg[2]+2*a[1]*a[7]*eg[1]+2*a[0]*a[6]*eg[0];
    em[5]=(a[4]*a[8]+a[5]*a[7])*eg[5]+(a[3]*a[8]+a[5]*a[6])*eg[4]+(a[3]*a[7]+a[4]*a[6])*eg[3]+2*a[5]*a[8]*eg[2]+2*a[4]*a[7]*eg[1]+2*a[3]*a[6]*eg[0];
  } // end of CalculiXRotation3D::rotateStrainForward

  // Calcul des déformations dans le repère global
  // e^g=M^T:e^m
  // em : Contraintes dans le repère matériau
  // eg : Contraintes dans le repère global
  void CalculiXRotation3D::rotateStrainsBackward(const CalculiXReal *const em,
						           CalculiXReal *const eg)
  {
    eg[0]=a[3]*a[6]*em[5]+a[0]*a[6]*em[4]+a[0]*a[3]*em[3]+a[6]*a[6]*em[2]+a[3]*a[3]*em[1]+a[0]*a[0]*em[0];
    eg[1]=a[4]*a[7]*em[5]+a[1]*a[7]*em[4]+a[1]*a[4]*em[3]+a[7]*a[7]*em[2]+a[4]*a[4]*em[1]+a[1]*a[1]*em[0];
    eg[2]=a[5]*a[8]*em[5]+a[2]*a[8]*em[4]+a[2]*a[5]*em[3]+a[8]*a[8]*em[2]+a[5]*a[5]*em[1]+a[2]*a[2]*em[0];
    eg[3]=(a[3]*a[7]+a[4]*a[6])*em[5]+(a[0]*a[7]+a[1]*a[6])*em[4]+(a[0]*a[4]+a[1]*a[3])*em[3]+2*a[6]*a[7]*em[2]+2*a[3]*a[4]*em[1]+2*a[0]*a[1]*em[0];
    eg[4]=(a[3]*a[8]+a[5]*a[6])*em[5]+(a[0]*a[8]+a[2]*a[6])*em[4]+(a[0]*a[5]+a[2]*a[3])*em[3]+2*a[6]*a[8]*em[2]+2*a[3]*a[5]*em[1]+2*a[0]*a[2]*em[0];
    eg[5]=(a[4]*a[8]+a[5]*a[7])*em[5]+(a[1]*a[8]+a[2]*a[7])*em[4]+(a[1]*a[5]+a[2]*a[4])*em[3]+2*a[7]*a[8]*em[2]+2*a[4]*a[5]*em[1]+2*a[1]*a[2]*em[0];
  } // end of CalculiXRotation3D::rotateStrainBackward

  // Calcul des contraintes dans le repère matériau
  // s^m=M:s^g
  // sg : Contraintes dans le repère global
  // sm : Contraintes dans le repère matériau
  void CalculiXRotation3D::rotateStressesForward(const CalculiXReal *const sg,
						           CalculiXReal *const sm){
    sm[0]=2*a[1]*a[2]*sg[5]+2*a[0]*a[2]*sg[4]+2*a[0]*a[1]*sg[3]+a[2]*a[2]*sg[2]+a[1]*a[1]*sg[1]+a[0]*a[0]*sg[0];
    sm[1]=2*a[4]*a[5]*sg[5]+2*a[3]*a[5]*sg[4]+2*a[3]*a[4]*sg[3]+a[5]*a[5]*sg[2]+a[4]*a[4]*sg[1]+a[3]*a[3]*sg[0]; 
    sm[2]=2*a[7]*a[8]*sg[5]+2*a[6]*a[8]*sg[4]+2*a[6]*a[7]*sg[3]+a[8]*a[8]*sg[2]+a[7]*a[7]*sg[1]+a[6]*a[6]*sg[0];
    sm[3]=(a[1]*a[5]+a[2]*a[4])*sg[5]+(a[2]*a[3]+a[0]*a[5])*sg[4]+(a[0]*a[4]+a[1]*a[3])*sg[3]+a[2]*a[5]*sg[2]+a[1]*a[4]*sg[1]+a[0]*a[3]*sg[0];
    sm[4]=(a[1]*a[8]+a[2]*a[7])*sg[5]+(a[2]*a[6]+a[0]*a[8])*sg[4]+(a[0]*a[7]+a[1]*a[6])*sg[3]+a[2]*a[8]*sg[2]+a[1]*a[7]*sg[1]+a[0]*a[6]*sg[0];
    sm[5]=(a[4]*a[8]+a[5]*a[7])*sg[5]+(a[5]*a[6]+a[3]*a[8])*sg[4]+(a[3]*a[7]+a[4]*a[6])*sg[3]+a[5]*a[8]*sg[2]+a[4]*a[7]*sg[1]+a[3]*a[6]*sg[0];
  } // end of CalculiXRotation3D::rotateStressesForward

  // Calcul des contraintes dans le repère global
  // s^g=N^T:s^m
  // sm : Contraintes dans le repère matériau
  // sg : Contraintes dans le repère global
  void CalculiXRotation3D::rotateStressesBackward(const CalculiXReal *const sm,
						            CalculiXReal *const sg){
    sg[0]=2*a[3]*a[6]*sm[5]+2*a[0]*a[6]*sm[4]+2*a[0]*a[3]*sm[3]+a[6]*a[6]*sm[2]+a[3]*a[3]*sm[1]+a[0]*a[0]*sm[0];
    sg[1]=2*a[4]*a[7]*sm[5]+2*a[1]*a[7]*sm[4]+2*a[1]*a[4]*sm[3]+a[7]*a[7]*sm[2]+a[4]*a[4]*sm[1]+a[1]*a[1]*sm[0];
    sg[2]=2*a[5]*a[8]*sm[5]+2*a[2]*a[8]*sm[4]+2*a[2]*a[5]*sm[3]+a[8]*a[8]*sm[2]+a[5]*a[5]*sm[1]+a[2]*a[2]*sm[0];
    sg[3]=(a[3]*a[7]+a[4]*a[6])*sm[5]+(a[1]*a[6]+a[0]*a[7])*sm[4]+(a[0]*a[4]+a[1]*a[3])*sm[3]+a[6]*a[7]*sm[2]+a[3]*a[4]*sm[1]+a[0]*a[1]*sm[0];
    sg[4]=(a[3]*a[8]+a[5]*a[6])*sm[5]+(a[2]*a[6]+a[0]*a[8])*sm[4]+(a[0]*a[5]+a[2]*a[3])*sm[3]+a[6]*a[8]*sm[2]+a[3]*a[5]*sm[1]+a[0]*a[2]*sm[0]; 
    sg[5]=(a[4]*a[8]+a[5]*a[7])*sm[5]+(a[2]*a[7]+a[1]*a[8])*sm[4]+(a[1]*a[5]+a[2]*a[4])*sm[3]+a[7]*a[8]*sm[2]+a[4]*a[5]*sm[1]+a[1]*a[2]*sm[0]; 
  } // end of CalculiXRotation3D::rotateStressesBackward
  
  void CalculiXRotation3D::rotateDeformationGradientForward(const CalculiXReal *const F,
								CalculiXReal *const Fm)
  {
    Fm[0]=a[2]*a[2]*F[8]+a[1]*a[2]*F[7]+a[0]*a[2]*F[6]+a[1]*a[2]*F[5]+a[1]*a[1]*F[4]+a[0]*a[1]*F[3]+a[0]*a[2]*F[2]+a[0]*a[1]*F[1]+a[0]*a[0]*F[0];
    Fm[1]=a[2]*a[5]*F[8]+a[2]*a[4]*F[7]+a[2]*a[3]*F[6]+a[1]*a[5]*F[5]+a[1]*a[4]*F[4]+a[1]*a[3]*F[3]+a[0]*a[5]*F[2]+a[0]*a[4]*F[1]+a[0]*a[3]*F[0];
    Fm[2]=a[2]*a[8]*F[8]+a[2]*a[7]*F[7]+a[2]*a[6]*F[6]+a[1]*a[8]*F[5]+a[1]*a[7]*F[4]+a[1]*a[6]*F[3]+a[0]*a[8]*F[2]+a[0]*a[7]*F[1]+a[0]*a[6]*F[0];
    Fm[3]=a[2]*a[5]*F[8]+a[1]*a[5]*F[7]+a[0]*a[5]*F[6]+a[2]*a[4]*F[5]+a[1]*a[4]*F[4]+a[0]*a[4]*F[3]+a[2]*a[3]*F[2]+a[1]*a[3]*F[1]+a[0]*a[3]*F[0];
    Fm[4]=a[5]*a[5]*F[8]+a[4]*a[5]*F[7]+a[3]*a[5]*F[6]+a[4]*a[5]*F[5]+a[4]*a[4]*F[4]+a[3]*a[4]*F[3]+a[3]*a[5]*F[2]+a[3]*a[4]*F[1]+a[3]*a[3]*F[0];
    Fm[5]=a[5]*a[8]*F[8]+a[5]*a[7]*F[7]+a[5]*a[6]*F[6]+a[4]*a[8]*F[5]+a[4]*a[7]*F[4]+a[4]*a[6]*F[3]+a[3]*a[8]*F[2]+a[3]*a[7]*F[1]+a[3]*a[6]*F[0];
    Fm[6]=a[2]*a[8]*F[8]+a[1]*a[8]*F[7]+a[0]*a[8]*F[6]+a[2]*a[7]*F[5]+a[1]*a[7]*F[4]+a[0]*a[7]*F[3]+a[2]*a[6]*F[2]+a[1]*a[6]*F[1]+a[0]*a[6]*F[0];
    Fm[7]=a[5]*a[8]*F[8]+a[4]*a[8]*F[7]+a[3]*a[8]*F[6]+a[5]*a[7]*F[5]+a[4]*a[7]*F[4]+a[3]*a[7]*F[3]+a[5]*a[6]*F[2]+a[4]*a[6]*F[1]+a[3]*a[6]*F[0];
    Fm[8]=a[8]*a[8]*F[8]+a[7]*a[8]*F[7]+a[6]*a[8]*F[6]+a[7]*a[8]*F[5]+a[7]*a[7]*F[4]+a[6]*a[7]*F[3]+a[6]*a[8]*F[2]+a[6]*a[7]*F[1]+a[6]*a[6]*F[0];
  } // end of CalculiXRotation3D::rotateDeformationGradientForward
  
  // compute the stiffness matrix in the global space
  void CalculiXRotation3D::rotateTangentOperatorBackward(CalculiXReal *const D) const
  {
    CalculiXReal MN[6][6];
    // Contruction de la matrice de passage N (pour les tenseurs)
    // Première ligne
    MN[0][0]=a[0]*a[0];
    MN[0][1]=a[1]*a[1];
    MN[0][2]=a[2]*a[2];
    MN[0][5]=a[1]*a[2];
    MN[0][4]=a[2]*a[0];
    MN[0][3]=a[0]*a[1];
    // Deuxième ligne
    MN[1][0]=a[3]*a[3];
    MN[1][1]=a[4]*a[4];
    MN[1][2]=a[5]*a[5];
    MN[1][5]=a[4]*a[5];
    MN[1][4]=a[5]*a[3];
    MN[1][3]=a[3]*a[4];
    // Troisième ligne
    MN[2][0]=a[6]*a[6];
    MN[2][1]=a[7]*a[7];
    MN[2][2]=a[8]*a[8];
    MN[2][5]=a[7]*a[8];
    MN[2][4]=a[8]*a[6];
    MN[2][3]=a[6]*a[7];
    // Quatrième ligne
    MN[5][0]=a[3]*a[6];
    MN[5][1]=a[4]*a[7];
    MN[5][2]=a[5]*a[8];
    MN[5][5]=a[4]*a[8]+a[5]*a[7];
    MN[5][4]=a[5]*a[6]+a[3]*a[8];
    MN[5][3]=a[3]*a[7]+a[4]*a[6];
    // Cinquième ligne
    MN[4][0]=a[6]*a[0];
    MN[4][1]=a[7]*a[1];
    MN[4][2]=a[8]*a[2];
    MN[4][5]=a[7]*a[2]+a[8]*a[1];
    MN[4][4]=a[8]*a[0]+a[6]*a[2];
    MN[4][3]=a[6]*a[1]+a[7]*a[0];
    // Sixième ligne
    MN[3][0]=a[0]*a[3];
    MN[3][1]=a[1]*a[4];
    MN[3][2]=a[2]*a[5];
    MN[3][5]=a[1]*a[5]+a[2]*a[4];
    MN[3][4]=a[2]*a[3]+a[0]*a[5];
    MN[3][3]=a[0]*a[4]+a[1]*a[3];
    // matrice N
    CalculiXReal N[6][6];
    for(unsigned short i=0;i!=6;++i){
      for(unsigned short j=0;j!=6;++j){
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
    CalculiXReal t[6][6];
    for(unsigned short i=0;i!=6;++i){
      for(unsigned short j=0;j!=6;++j){
	t[i][j] = 0.;
	for(unsigned short k=0;k!=6;++k){
	  t[i][j] += D[k*6+i]*(N[k][j]);
	}
      }
    }
    // matrice finale
    for(unsigned short i=0;i!=6;++i){
      for(unsigned short j=0;j!=6;++j){
	D[j*6+i] = 0.;
	for(unsigned short k=0;k!=6;++k){
	  D[j*6+i] += N[k][i]*t[k][j];
	}
      }
    }
  } // end of CalculiXRotation3D::rotateTangentOperatorBackward

  tfel::math::tmatrix<3u,3u,CalculiXReal>
  getRotationMatrix3D(const CalculiXReal* const v){
    const auto v1x = v[0];
    const auto v1y = v[1];
    const auto v1z = v[2];
    const auto v2x = v[3];
    const auto v2y = v[4];
    const auto v2z = v[5];
    const auto v3x = v1y*v2z-v2y*v1z;
    const auto v3y = v2x*v1z-v2z*v1x;
    const auto v3z = v1x*v2y-v2x*v1y;
    tfel::math::tmatrix<3u,3u,CalculiXReal> r;
    r(0,0)=v1x;
    r(0,1)=v1y;
    r(0,2)=v1z;
    r(1,0)=v2x;
    r(1,1)=v2y;
    r(1,2)=v2z;
    r(2,0)=v3x;
    r(2,1)=v3y;
    r(2,2)=v3z;
    return r;
  }
  
} // end of namespace calculix
