/*!
 * \file   mfront/src/UMATRotationMatrix.cxx
 * \brief    
 * \author É. Castelier
 * \date   17 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/UMAT/UMATRotationMatrix.hxx"

namespace umat
{

  // Constructeur
  // V : Matrice de passage matériau/élement
  //     (deux premiers vecteurs)
  // drot : Matrice de passage élement/global
  UMATRotationMatrix2D::UMATRotationMatrix2D(const UMATReal *const V,
					     const UMATReal *const drot)
  {
    // Matrice de passage matériau/global
    UMATReal a[4];

    // Premier vecteur
    // a[1,1]
    a[0]=drot[0]*V[0]+drot[3]*V[1];
    // a[2,1]
    a[1]=drot[1]*V[0]+drot[4]*V[1];
    // Deuxième vecteur :
    //   Produit vectoriel
    // a[1,2]
    a[2]=-a[1];
    // a[2,2]
    a[3]=a[0];

    // Contruction de la matrice de passage N (pour les tenseurs)
    // Première ligne
    MN[0][0]=a[0]*a[0];
    MN[0][1]=a[1]*a[1];
    MN[0][2]=a[0]*a[1];

    // Deuxième ligne
    MN[1][0]=a[2]*a[2];
    MN[1][1]=a[3]*a[3];
    MN[1][2]=a[2]*a[3];

    // Troisième ligne
    MN[2][0]=a[0]*a[2];
    MN[2][1]=a[1]*a[3];
    MN[2][2]=a[0]*a[3]+a[1]*a[2];
  } // end of UMATRotationMatrix2D::UMATRotationMatrix2D

  // Calcul des déformations dans le repère matériau
  // e^m=N:e^g
  // eg : Déformations dans le repère global
  // em : Déformations dans le repère matériau
  void
  UMATRotationMatrix2D::rotateStrainsForward(const UMATReal *const eg,
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
  } // end of UMATRotationMatrix2D::rotateStrainsForward


  // Calcul des contraintes dans le repère global
  // s^g=N^T:s^m
  // sm : Contraintes dans le repère matériau
  // sg : Contraintes dans le repère global
  void
  UMATRotationMatrix2D::rotateStressesBackward(const UMATReal *const sm,
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
  } // end of UMATRotationMatrix2D::rotateStressesBackward

  // Calcul des contraintes dans le repère matériau
  // s^m=M:s^g
  // sg : Contraintes dans le repère global
  // sm : Contraintes dans le repère matériau
  void
  UMATRotationMatrix2D::rotateStressesForward(const UMATReal *const sg,
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
  } // end of UMATRotationMatrix2D::rotateStressesForward

  // Calcul des déformations dans le repère global
  // e^g=M^T:e^m
  // em : Contraintes dans le repère matériau
  // eg : Contraintes dans le repère global
  void
  UMATRotationMatrix2D::rotateStrainsBackward(const UMATReal *const em,
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
  } // end of UMATRotationMatrix2D:::rotateStrainsBackward

  // Calcul de la déformation dans le repère global
  // D^g=tN:D^m:N
  void
  UMATRotationMatrix2D::rotateTangentOperatorBackward(UMATReal *const D) const
  {
    // matrice N
    UMATReal N[4][4];
    for(unsigned short i=0;i!=2;++i){
      for(unsigned short j=0;j!=2;++j){
	N[i][j] = MN[i][j];
      }
    }
    N[2][0]  = 0.;
    N[2][1]  = 0.;
    N[2][3]  = 0.;
    N[0][2]  = 0.;
    N[1][2]  = 0.;
    N[3][2]  = 0.;
    N[2][2]  = 1.;
    for(unsigned short i=0;i!=2;++i){
      N[3][i] = MN[2][i];
      N[i][3] = MN[i][2];
    }
    N[3][3]  = MN[2][2];
    N[3][0] *= UMATReal(2.);
    N[3][1] *= UMATReal(2.);
    N[3][2] *= UMATReal(2.);
    // matrice temporaire
    using namespace std;
    UMATReal t[4][4];
    for(unsigned short i=0;i!=4;++i){
      for(unsigned short j=0;j!=4;++j){
	t[i][j] = 0.;
	for(unsigned short k=0;k!=4;++k){
	  t[i][j] += D[k*4+i]*(N[k][j]);
	}
      }
    }
    // matrice finale
    for(unsigned short i=0;i!=4;++i){
      for(unsigned short j=0;j!=4;++j){
	D[j*4+i] = 0.;
	for(unsigned short k=0;k!=4;++k){
	  D[j*4+i] += N[k][i]*t[k][j];
	}
      }
    }
  } // end of UMATRotationMatrix2D::rotateTangentOperatorBackward

  // Constructeur
  // V : Matrice de passage matériau/élement
  //     (deux premiers vecteurs)
  // drot : Matrice de passage élement/global
  UMATRotationMatrix3D::UMATRotationMatrix3D(const UMATReal *const V,
					     const UMATReal *const drot)
  {
    // Matrice de passage matériau/global
    UMATReal a[9];
    
    // Premier vecteur
    a[0]=drot[0]*V[0]+drot[3]*V[1]+drot[6]*V[2];
    a[1]=drot[1]*V[0]+drot[4]*V[1]+drot[7]*V[2];
    a[2]=drot[2]*V[0]+drot[5]*V[1]+drot[8]*V[2];
    // Deuxième vecteur
    a[3]=drot[0]*V[3]+drot[3]*V[4]+drot[6]*V[5];
    a[4]=drot[1]*V[3]+drot[4]*V[4]+drot[7]*V[5];
    a[5]=drot[2]*V[3]+drot[5]*V[4]+drot[8]*V[5];
    // Troisième vecteur :
    //   produit vectoriel des deux premiers
    a[6]=a[1]*a[5]-a[4]*a[2];
    a[7]=a[2]*a[3]-a[5]*a[0];
    a[8]=a[0]*a[4]-a[3]*a[1];

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
  } // end of UMATRotationMatrix3D::UMATRotationMatrix3D

  // Calcul des déformations dans le repère matériau
  // e^m=N:e^g
  // eg : Déformations dans le repère global
  // em : Déformations dans le repère matériau
  void
  UMATRotationMatrix3D::rotateStrainsForward(const UMATReal *const eg,
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
  } // end of UMATRotationMatrix3D::rotateStrainForward

  // Calcul des contraintes dans le repère global
  // s^g=N^T:s^m
  // sm : Contraintes dans le repère matériau
  // sg : Contraintes dans le repère global
  void
  UMATRotationMatrix3D::rotateStressesBackward(const UMATReal *const sm,
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
  } // end of UMATRotationMatrix3D::rotateStressesBackward

  // Calcul des contraintes dans le repère matériau
  // s^m=M:s^g
  // sg : Contraintes dans le repère global
  // sm : Contraintes dans le repère matériau
  void
  UMATRotationMatrix3D::rotateStressesForward(const UMATReal *const sg,
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
  } // end of UMATRotationMatrix3D::rotateStressesForward

  // Calcul des déformations dans le repère global
  // e^g=M^T:e^m
  // em : Contraintes dans le repère matériau
  // eg : Contraintes dans le repère global
  void
  UMATRotationMatrix3D::rotateStrainsBackward(const UMATReal *const em,
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
  } // end of UMATRotationMatrix3D::rotateStrainBackward

  // compute the stiffness matrix in the global space
  void
  UMATRotationMatrix3D::rotateTangentOperatorBackward(UMATReal *const D) const
  {
    // matrice N
    UMATReal N[6][6];
    for(unsigned short i=0;i!=6;++i){
      for(unsigned short j=0;j!=6;++j){
	N[i][j] = MN[i][j];
      }
    }
    N[3][0] *= UMATReal(2.);
    N[3][1] *= UMATReal(2.);
    N[3][2] *= UMATReal(2.);
    N[4][0] *= UMATReal(2.);
    N[4][1] *= UMATReal(2.);
    N[4][2] *= UMATReal(2.);
    N[5][0] *= UMATReal(2.);
    N[5][1] *= UMATReal(2.);
    N[5][2] *= UMATReal(2.);
    // matrice temporaire
    UMATReal t[6][6];
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
  } // end of UMATRotationMatrix3D::rotateTangentOperatorBackward

} // end of namespace umat
