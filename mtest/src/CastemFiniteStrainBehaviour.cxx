/*!
 * \file   mfront/mtest/CastemFiniteStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 november 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<limits>
#include<algorithm>

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Castem/Castem.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MTest/Evolution.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/CastemFiniteStrainBehaviour.hxx"
#include"MFront/BehaviourSymmetryType.hxx"

namespace mtest
{

  static void
  MTestComputeIsotropicFiniteStrainStiffnessTensor2D(const real* const props,
						     tfel::math::t2tost2<2u,real>& C)
  {
    using namespace std;
    static const real cste = sqrt(real(2));
    const real E   = props[0];
    const real n   = props[1];
    const real l   = E*n/((1.-2.*n)*(1+n));
    const real G   = E/(2*(1+n));
    const real C11 = l+2*G;
    fill(C.begin(),C.end(),real(0.));
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
    C(3,3)=cste*G/2;    
    C(3,4)=cste*G/2;
  }

  static void
  CastemComputeOrthotropicFiniteStrainStiffnessTensor2D(const real* const props,
						      tfel::math::t2tost2<2u,real>& C)
  {
    using namespace std;
    // props[0] :'YG1'
    // props[1] :'YG2'
    // props[2] :'YG3'
    // props[3] :'NU12'
    // props[4] :'NU23'
    // props[5] :'NU13'
    // props[6] :'G12'
    using namespace std;
    static const real cste = sqrt(real(2));
    // S11 = 1/E1
    const real S11=1/props[0];
    // S22 = 1/E2
    const real S22=1/props[1];
    // S22 = 1/E3
    const real S33=1/props[2];
    // S12 = -n12/E1
    const real S12=-props[3]/props[0];
    // S13 = -n13/E1
    const real S13=-props[5]/props[0];
    // S23 = -n23/E2
    const real S23=-props[4]/props[1];
    const real detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
    // Matrice de raideur
    fill(C.begin(),C.end(),real(0.));
    C(0,0)=(S22*S33-S23*S23)/detS;
    C(0,1)=(S13*S23-S12*S33)/detS;
    C(0,2)=(S12*S23-S13*S22)/detS;
    C(1,0)=(S13*S23-S12*S33)/detS;
    C(1,1)=(S11*S33-S13*S13)/detS;
    C(1,2)=(S12*S13-S11*S23)/detS;
    C(2,0)=(S12*S23-S13*S22)/detS;
    C(2,1)=(S12*S13-S11*S23)/detS;
    C(2,2)=(S11*S22-S12*S12)/detS;
    C(3,3)=cste*props[6]/2;
    C(3,4)=cste*props[6]/2;
  } // end of struct CastemComputeStiffnessTensor

  /*!
   * This structure is in charge of computing a stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template<tfel::material::ModellingHypothesis::Hypothesis,
	   mfront::BehaviourSymmetryType>
  struct MTestCastemComputeFiniteStrainStiffnessTensor;

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						     mfront::ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<1u,real>& C)
    {
      using namespace std;
      const real E = props[0];
      const real n = props[1];
      const real l = E*n/((1.-2.*n)*(1+n));
      const real G = E/(1+n);
      const real C11 = l+G;
      C(0,0)=C11;
      C(0,1)=l;
      C(0,2)=l;
      C(1,0)=l;
      C(1,1)=C11;
      C(1,2)=l;
      C(2,0)=l;
      C(2,1)=l;
      C(2,2)=C11;
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  }; 

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRESS,
						     mfront::ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      using namespace std;
      static const real cste = sqrt(real(2));
      const real E   = props[0];
      const real n   = props[1];
      const real C1  = E/(1+n*n);
      const real C2  = n*C1;
      const real C3  = (1-n)*C1;
      fill(C.begin(),C.end(),real(0.));
      C(0,0)=C1;
      C(0,1)=C2;
      C(1,0)=C2;
      C(1,1)=C1;
      C(3,3)=cste*C3/2;
      C(3,4)=cste*C3/2;    
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  }; 
  
  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICAL,
						     mfront::ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      MTestComputeIsotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,
						     mfront::ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      MTestComputeIsotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
						     mfront::ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      MTestComputeIsotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
						     mfront::ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<3u,real>& C)
    {
      using namespace std;
      static const real cste = sqrt(real(2));
      const real E = props[0];
      const real n = props[1];
      const real l = E*n/((1.-2.*n)*(1+n));
      const real G = E/(2*(1+n));
      const real C11 = l+2*G;
      fill(C.begin(),C.end(),real(0.));
      C(0,0)=C11;
      C(0,1)=l;
      C(0,2)=l;
      C(1,0)=l;
      C(1,1)=C11;
      C(1,2)=l;
      C(2,0)=l;
      C(2,1)=l;
      C(2,2)=C11;
      C(3,3)=cste*G/2;
      C(3,4)=cste*G/2;
      C(4,5)=cste*G/2;
      C(4,6)=cste*G/2;
      C(5,7)=cste*G/2;
      C(5,8)=cste*G/2;
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };
  
  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						     mfront::ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<1u,real>& C)
    {
      // Matrice de souplesse
      // props[0] :'YG1'
      // props[1] :'YG2'
      // props[2] :'YG3'
      // props[3] :'NU12'
      // props[4] :'NU23'
      // props[5] :'NU13'
      using namespace std;
      // S11 = 1/E1
      const real S11=1/props[0];
      // S22 = 1/E2
      const real S22=1/props[1];
      // S22 = 1/E3
      const real S33=1/props[2];
      // S12 = -n12/E1
      const real S12=-props[3]/props[0];
      // S13 = -n13/E1
      const real S13=-props[5]/props[0];
      // S23 = -n23/E2
      const real S23=-props[4]/props[1];
      const real detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
      // Matrice de raideur
      C(0,0)=(S22*S33-S23*S23)/detS;
      C(0,1)=(S13*S23-S12*S33)/detS;
      C(0,2)=(S12*S23-S13*S22)/detS;
      C(1,0)=(S13*S23-S12*S33)/detS;
      C(1,1)=(S11*S33-S13*S13)/detS;
      C(1,2)=(S12*S13-S11*S23)/detS;
      C(2,0)=(S12*S23-S13*S22)/detS;
      C(2,1)=(S12*S13-S11*S23)/detS;
      C(2,2)=(S11*S22-S12*S12)/detS;
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRESS,
						     mfront::ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      using namespace std;
      static const real cste = sqrt(real(2));
      // props[0] :'YG1'
      // props[1] :'YG2'
      // props[2] :'NU12'
      // props[3] :'G12'
      // props[6] :'YG3'
      // props[7] :'NU23'
      // props[8] :'NU13'
      using namespace std;
      // S11 = 1/E1
      const real S11=1/props[0];
      // S22 = 1/E2
      const real S22=1/props[1];
      // S22 = 1/E3
      const real S33=1/props[6];
      // S12 = -n12/E1
      const real S12=-props[2]/props[0];
      // S13 = -n13/E1
      const real S13=-props[8]/props[0];
      // S23 = -n23/E2
      const real S23=-props[7]/props[1];
      const real inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
      // Matrice de raideur
      fill(C.begin(),C.end(),real(0.));
      const real C00=(S22*S33-S23*S23)*inv_detS;
      const real C01=(S13*S23-S12*S33)*inv_detS;
      const real C02=(S12*S23-S13*S22)*inv_detS;
      const real C10=(S13*S23-S12*S33)*inv_detS;
      const real C11=(S11*S33-S13*S13)*inv_detS;
      const real C12=(S12*S13-S11*S23)*inv_detS;
      const real C20=(S12*S23-S13*S22)*inv_detS;
      const real C21=(S12*S13-S11*S23)*inv_detS;
      const real C22=(S11*S22-S12*S12)*inv_detS;
      const real tmp20 = C20/C22;
      const real tmp21 = C21/C22;
      C(0,0)=C00-C02*tmp20;
      C(0,1)=C01-C02*tmp21;
      C(1,0)=C10-C12*tmp20;
      C(1,1)=C11-C12*tmp21;
      C(3,3)=cste*props[3];
      C(3,4)=cste*props[3];    
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICAL,
						     mfront::ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      CastemComputeOrthotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,
						     mfront::ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      CastemComputeOrthotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
						     mfront::ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<2u,real>& C)
    {
      CastemComputeOrthotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
						     mfront::ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::math::t2tost2<3u,real>& C)
    {
      // props[0] :'YG1'
      // props[1] :'YG2'
      // props[2] :'YG3'
      // props[3] :'NU12'
      // props[4] :'NU23'
      // props[5] :'NU13'
      // props[6] :'G12'
      // props[7] :'G23'
      // props[8] :'G13'
      using namespace std;
      static const real cste = sqrt(real(2));
      // S11 = 1/E1
      const real S11=1/props[0];
      // S22 = 1/E2
      const real S22=1/props[1];
      // S22 = 1/E3
      const real S33=1/props[2];
      // S12 = -n12/E1
      const real S12=-props[3]/props[0];
      // S13 = -n13/E1
      const real S13=-props[5]/props[0];
      // S23 = -n23/E2
      const real S23=-props[4]/props[1];
      const real detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
      // Matrice de raideur
      fill(C.begin(),C.end(),real(0.));
      C(0,0)=(S22*S33-S23*S23)/detS;
      C(1,1)=(S11*S33-S13*S13)/detS;
      C(2,2)=(S11*S22-S12*S12)/detS;
      C(0,1)=C(1,0)=(S13*S23-S12*S33)/detS;
      C(0,2)=C(2,0)=(S12*S23-S13*S22)/detS;
      C(1,2)=C(2,1)=(S12*S13-S11*S23)/detS;
      C(3,3)=cste*props[6]/2;
      C(3,4)=cste*props[6]/2;
      C(4,5)=cste*props[7]/2;
      C(4,6)=cste*props[7]/2;
      C(5,7)=cste*props[8]/2;
      C(5,8)=cste*props[8]/2;
    } // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  // struct MTestCastemfiniteStrainRotationMatrix2D
  // {
  //   MTestCastemfiniteStrainRotationMatrix2D(const real *const);
  //   // compute the stiffness matrix in the global space
  //   void rotateStiffnessMatrixBackward(real *const) const;
  // private:
  //   real MN[3][3];
  // }; // end of struct MTestCastemfiniteStrainRotationMatrix2D
  
  // struct MTestCastemfiniteStrainRotationMatrix3D
  // {
    
  //   MTestCastemfiniteStrainRotationMatrix3D(const real *const);
  //   // compute the stiffness matrix in the global space
  //   void rotateStiffnessMatrixBackward(real *const) const;
  // private:
  //   real MN[6][6];
  // }; // end of struct MTestCastemfiniteStrainRotationMatrix3D

  // // Constructeur
  // // drot : Matrice de passage élement/global
  // MTestCastemfiniteStrainRotationMatrix2D::MTestCastemfiniteStrainRotationMatrix2D(const real *const drot)
  // {
  //   // Matrice de passage matériau/global
  //   real a[4];
  //   // // Premier vecteur
  //   // a[1,1]
  //   a[0]=drot[0];
  //   // a[2,1]
  //   a[1]=drot[1];
  //   // Deuxième vecteur :
  //   //   Produit vectoriel
  //   // a[1,2]
  //   a[2]=-a[1];
  //   // a[2,2]
  //   a[3]=a[0];
  //   // Contruction de la matrice de passage N (pour les tenseurs)
  //   // Première ligne
  //   MN[0][0]=a[0]*a[0];
  //   MN[0][1]=a[1]*a[1];
  //   MN[0][2]=a[0]*a[1];
  //   // Deuxième ligne
  //   MN[1][0]=a[2]*a[2];
  //   MN[1][1]=a[3]*a[3];
  //   MN[1][2]=a[2]*a[3];
  //   // Troisième ligne
  //   MN[2][0]=a[0]*a[2];
  //   MN[2][1]=a[1]*a[3];
  //   MN[2][2]=a[0]*a[3]+a[1]*a[2];
  // } // end of MTestCastemfiniteStrainRotationMatrix2D::MTestCastemfiniteStrainRotationMatrix2D

  // // Calcul de la déformation dans le repère global
  // // D^g=tN:D^m:N
  // void
  // MTestCastemfiniteStrainRotationMatrix2D::rotateStiffnessMatrixBackward(real *const D) const
  // {
  //   // matrice N
  //   real N[4][4];
  //   for(unsigned short i=0;i!=2;++i){
  //     for(unsigned short j=0;j!=2;++j){
  // 	N[i][j] = MN[i][j];
  //     }
  //   }
  //   N[2][0]  = 0.;
  //   N[2][1]  = 0.;
  //   N[2][3]  = 0.;
  //   N[0][2]  = 0.;
  //   N[1][2]  = 0.;
  //   N[3][2]  = 0.;
  //   N[2][2]  = 1.;
  //   for(unsigned short i=0;i!=2;++i){
  //     N[3][i] = MN[2][i];
  //     N[i][3] = MN[i][2];
  //   }
  //   N[3][3]  = MN[2][2];
  //   N[3][0] *= real(2.);
  //   N[3][1] *= real(2.);
  //   N[3][2] *= real(2.);
  //   // matrice temporaire
  //   using namespace std;
  //   real t[4][4];
  //   for(unsigned short i=0;i!=4;++i){
  //     for(unsigned short j=0;j!=4;++j){
  // 	t[i][j] = 0.;
  // 	for(unsigned short k=0;k!=4;++k){
  // 	  t[i][j] += D[k*4+i]*(N[k][j]);
  // 	}
  //     }
  //   }
  //   // matrice finale
  //   for(unsigned short i=0;i!=4;++i){
  //     for(unsigned short j=0;j!=4;++j){
  // 	D[j*4+i] = 0.;
  // 	for(unsigned short k=0;k!=4;++k){
  // 	  D[j*4+i] += N[k][i]*t[k][j];
  // 	}
  //     }
  //   }
  // } // end of MTestCastemfiniteStrainRotationMatrix2D::rotateStiffnessMatrixBackward

  // // Constructeur
  // // drot : Matrice de passage élement/global
  // MTestCastemfiniteStrainRotationMatrix3D::MTestCastemfiniteStrainRotationMatrix3D(const real *const drot)
  // {
  //   // Contruction de la matrice de passage N (pour les tenseurs)
  //   // Première ligne
  //   MN[0][0]=drot[0]*drot[0];
  //   MN[0][1]=drot[1]*drot[1];
  //   MN[0][2]=drot[2]*drot[2];
  //   MN[0][5]=drot[1]*drot[2];
  //   MN[0][4]=drot[2]*drot[0];
  //   MN[0][3]=drot[0]*drot[1];

  //   // Deuxième ligne
  //   MN[1][0]=drot[3]*drot[3];
  //   MN[1][1]=drot[4]*drot[4];
  //   MN[1][2]=drot[5]*drot[5];
  //   MN[1][5]=drot[4]*drot[5];
  //   MN[1][4]=drot[5]*drot[3];
  //   MN[1][3]=drot[3]*drot[4];

  //   // Troisième ligne
  //   MN[2][0]=drot[6]*drot[6];
  //   MN[2][1]=drot[7]*drot[7];
  //   MN[2][2]=drot[8]*drot[8];
  //   MN[2][5]=drot[7]*drot[8];
  //   MN[2][4]=drot[8]*drot[6];
  //   MN[2][3]=drot[6]*drot[7];

  //   // Quatrième ligne
  //   MN[5][0]=drot[3]*drot[6];
  //   MN[5][1]=drot[4]*drot[7];
  //   MN[5][2]=drot[5]*drot[8];
  //   MN[5][5]=drot[4]*drot[8]+drot[5]*drot[7];
  //   MN[5][4]=drot[5]*drot[6]+drot[3]*drot[8];
  //   MN[5][3]=drot[3]*drot[7]+drot[4]*drot[6];

  //   // Cinquième ligne
  //   MN[4][0]=drot[6]*drot[0];
  //   MN[4][1]=drot[7]*drot[1];
  //   MN[4][2]=drot[8]*drot[2];
  //   MN[4][5]=drot[7]*drot[2]+drot[8]*drot[1];
  //   MN[4][4]=drot[8]*drot[0]+drot[6]*drot[2];
  //   MN[4][3]=drot[6]*drot[1]+drot[7]*drot[0];

  //   // Sixième ligne
  //   MN[3][0]=drot[0]*drot[3];
  //   MN[3][1]=drot[1]*drot[4];
  //   MN[3][2]=drot[2]*drot[5];
  //   MN[3][5]=drot[1]*drot[5]+drot[2]*drot[4];
  //   MN[3][4]=drot[2]*drot[3]+drot[0]*drot[5];
  //   MN[3][3]=drot[0]*drot[4]+drot[1]*drot[3];
  // } // end of MTestCastemfiniteStrainRotationMatrix3D::MTestCastemfiniteStrainRotationMatrix3D

  // // compute the stiffness matrix in the global space
  // void
  // MTestCastemfiniteStrainRotationMatrix3D::rotateStiffnessMatrixBackward(real *const D) const
  // {
  //   // matrice N
  //   real N[6][6];
  //   for(unsigned short i=0;i!=6;++i){
  //     for(unsigned short j=0;j!=6;++j){
  // 	N[i][j] = MN[i][j];
  //     }
  //   }
  //   N[3][0] *= real(2.);
  //   N[3][1] *= real(2.);
  //   N[3][2] *= real(2.);
  //   N[4][0] *= real(2.);
  //   N[4][1] *= real(2.);
  //   N[4][2] *= real(2.);
  //   N[5][0] *= real(2.);
  //   N[5][1] *= real(2.);
  //   N[5][2] *= real(2.);
  //   // matrice temporaire
  //   real t[6][6];
  //   for(unsigned short i=0;i!=6;++i){
  //     for(unsigned short j=0;j!=6;++j){
  // 	t[i][j] = 0.;
  // 	for(unsigned short k=0;k!=6;++k){
  // 	  t[i][j] += D[k*6+i]*(N[k][j]);
  // 	}
  //     }
  //   }
  //   // matrice finale
  //   for(unsigned short i=0;i!=6;++i){
  //     for(unsigned short j=0;j!=6;++j){
  // 	D[j*6+i] = 0.;
  // 	for(unsigned short k=0;k!=6;++k){
  // 	  D[j*6+i] += N[k][i]*t[k][j];
  // 	}
  //     }
  //   }
  // } // end of MTestCastemfiniteStrainRotationMatrix3D::rotateStiffnessMatrixBackward

  static tfel::material::ModellingHypothesis::Hypothesis
  getEffectiveModellingHypothesis(const tfel::material::ModellingHypothesis::Hypothesis& h,
				  const std::string& l,
				  const std::string& b)
  {
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = ELM::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	return ModellingHypothesis::GENERALISEDPLANESTRAIN;
      }
    }
    return h;
  } // end of getEffectiveModellingHypothesis

  CastemFiniteStrainBehaviour::CastemFiniteStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : CastemStandardBehaviour(getEffectiveModellingHypothesis(h,l,b),l,b)
  {
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	this->usesGenericPlaneStressAlgorithm = true;
	//! better name required
	this->ivnames.push_back("AxialStrain");
	this->ivtypes.push_back(0);
      }
    }
  } // end of CastemFiniteStrainBehaviour::CastemFiniteStrainBehaviour

  void
  CastemFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    std::fill(v.begin(),v.end(),real(0));
    v[0] = real(1);
    v[1] = real(1);
    v[2] = real(1);
  } // end of CastemFiniteStrainBehaviour::setDrivingVariablesDefaultInitialValue  

  std::pair<bool,real>
  CastemFiniteStrainBehaviour::computePredictionOperator(BehaviourWorkSpace& wk,
							 const CurrentState& s,
							 const StiffnessMatrixType ktype) const
  {
    // rotation matrix
    if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      // compute the stiffness operator from material properties
      const auto rt = transpose(s.r);
      this->computeElasticStiffness(wk.kt,s.mprops1,rt);
      return {true,1};
    }
    throw(std::runtime_error("CastemFiniteStrainBehaviour::computePredictionOperator : "
			     "computation of the tangent operator "
			     "is not supported"));
  } // end of CastemFiniteStrainBehaviour::computePredictionOperator

  std::pair<bool,real>
  CastemFiniteStrainBehaviour::integrate(CurrentState& s,
					 BehaviourWorkSpace& wk,
					 const real dt,
					 const StiffnessMatrixType ktype) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace castem;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    const auto h = this->usesGenericPlaneStressAlgorithm ?
                   ModellingHypothesis::PLANESTRESS :this->getHypothesis();
    CastemInt ntens,ndi;
    const auto nprops = static_cast<CastemInt>(s.mprops1.size());
    CastemInt nstatv;
    if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 14;
      ntens = 3;
    } else if (h==ModellingHypothesis::AXISYMMETRICAL){
      ndi = 0;
      ntens = 4;
    } else if (h==ModellingHypothesis::PLANESTRESS){
      ndi = -2;
      ntens = 4;
    } else if (h==ModellingHypothesis::PLANESTRAIN){
      ndi = -1;
      ntens = 4;
    } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 4;
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 6;
    } else {
      throw(runtime_error("CastemFiniteStrainBehaviour::integrate: "
			  "unsupported hypothesis"));
    }
    if((wk.D.getNbRows()!=wk.k.getNbRows())||
       (wk.D.getNbCols()!=wk.k.getNbCols())){
      throw(runtime_error("CastemFiniteStrainBehaviour::integrate: "
			  "the memory has not been allocated correctly"));
    }
    if(((s.iv0.size()==0)&&(wk.ivs.size()!=1u))||
       ((s.iv0.size()!=0)&&(s.iv0.size()!=wk.ivs.size()))){
      throw(runtime_error("CastemFiniteStrainBehaviour::integrate: "
			  "the memory has not been allocated correctly"));
    }
    fill(wk.D.begin(),wk.D.end(),0.);
    if(s.iv0.size()!=0){
      copy(s.iv0.begin(),s.iv0.end(),wk.ivs.begin());
    }
    nstatv = static_cast<CastemInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot = transpose(s.r);
    CastemInt kinc(1);
    tmatrix<3u,3u,real> uu0(real(0));
    tmatrix<3u,3u,real> uu1(real(0));
    uu0(0,0) = s.e0(0); uu1(0,0) = s.e1(0);
    uu0(1,1) = s.e0(1); uu1(1,1) = s.e1(1);
    uu0(2,2) = s.e0(2); uu1(2,2) = s.e1(2);
    if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      uu0(1,0) = 0.; uu1(1,0) = 0.;
      uu0(0,1) = 0.; uu1(0,1) = 0.;
      uu0(2,0) = 0.; uu1(2,0) = 0.;
      uu0(0,2) = 0.; uu1(0,2) = 0.;
      uu0(2,1) = 0.; uu1(2,1) = 0.;
      uu0(1,2) = 0.; uu1(1,2) = 0.;
    } else if ((h==ModellingHypothesis::AXISYMMETRICAL)||
	       (h==ModellingHypothesis::PLANESTRESS)||
	       (h==ModellingHypothesis::PLANESTRAIN)||
	       (h==ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      // uu0 and uu1 must be built using Fortran notations
      uu0(1,0) = s.e0(3); uu1(1,0) = s.e1(3);
      uu0(0,1) = s.e0(4); uu1(0,1) = s.e1(4);
      uu0(2,0) = 0.; uu1(2,0) = 0.;
      uu0(0,2) = 0.; uu1(0,2) = 0.;
      uu0(2,1) = 0.; uu1(2,1) = 0.;
      uu0(1,2) = 0.; uu1(1,2) = 0.;
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      // uu0 and uu1 must be built using Fortran notations
      uu0(1,0) = s.e0(3); uu1(1,0) = s.e1(3);
      uu0(0,1) = s.e0(4); uu1(0,1) = s.e1(4);
      uu0(2,0) = s.e0(5); uu1(2,0) = s.e1(5);
      uu0(0,2) = s.e0(6); uu1(0,2) = s.e1(6);
      uu0(2,1) = s.e0(7); uu1(2,1) = s.e1(7);
      uu0(1,2) = s.e0(8); uu1(1,2) = s.e1(8);
    } else {
      throw(runtime_error("CastemFiniteStrainBehaviour::integrate: "
			  "unsupported hypothesis"));
    }
    copy(s.s0.begin(),s.s0.end(),s.s1.begin());
    tmatrix<3u,3u,real>::size_type i;
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s.s1(i)  /= sqrt2;
    }
    CastemReal ndt = std::numeric_limits<CastemReal>::max();
    (this->fct)(&(s.s1(0)),&(wk.ivs(0)),&(wk.D(0,0)),
		nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,&dt,
		&(s.esv0(0))  ,&(s.desv(0)),
		&(s.esv0(0))+1,&(s.desv(0))+1,
		nullptr,&ndi,nullptr,&ntens,&nstatv,&(s.mprops1(0)),
		&nprops,nullptr,&drot(0,0),&ndt,
		nullptr,&uu0(0,0),&uu1(0,0),nullptr,nullptr,
		nullptr,nullptr,nullptr,&kinc,0);
    if(kinc!=1){
      return {false,ndt};
    }
    // tangent operator (...)
    if(ktype!=StiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(wk.k,s.mprops1,drot);
      } else {
	throw(runtime_error("CastemFiniteStrainBehaviour::integrate: "
			    "computation of the tangent operator "
			    "is not supported"));
      }
    }
    if(!s.iv1.empty()){
      copy_n(wk.ivs.begin(), s.iv1.size(),s.iv1.begin());
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s.s1(i) *= sqrt2;
    }
    return {true,ndt};
  } // end of CastemFiniteStrainBehaviour::integrate

  void
  CastemFiniteStrainBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
						       const tfel::math::vector<real>& mp,
						       const tfel::math::tmatrix<3u,3u,real>&) const
  {
    using namespace std;
    using namespace tfel::math;
    tmatrix<3u,3u,real>::size_type i,j;
    const auto h = this->getHypothesis();    
    if(this->stype==0u){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	t2tost2<1u,real> De;
	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						    mfront::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::AXISYMMETRICAL){
	t2tost2<2u,real> De;
	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::AXISYMMETRICAL,
						    mfront::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::PLANESTRESS){
	t2tost2<2u,real> De;
	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::PLANESTRESS,
						    mfront::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::PLANESTRAIN){
	t2tost2<2u,real> De;
	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::PLANESTRAIN,
						    mfront::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
	t2tost2<2u,real> De;
	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::GENERALISEDPLANESTRAIN,
						    mfront::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	t2tost2<3u,real> De;
	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::TRIDIMENSIONAL,
						    mfront::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=6u;++i){
	  for(j=0;j!=9u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	throw(runtime_error("CastemFiniteStrainBehaviour::computeElasticMatrix: "
			    "unsupported hypothesis"));
      }
    } else if(this->stype==1u){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      	t2tost2<1u,real> De;
      	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						    mfront::ORTHOTROPIC>::exe(&mp(0),De);
      	for(i=0;i!=3u;++i){
      	  for(j=0;j!=3u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==ModellingHypothesis::AXISYMMETRICAL){
      	t2tost2<2u,real> De;
      	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::AXISYMMETRICAL,
						    mfront::ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestCastemfiniteStrainRotationMatrix2D m(&drot(0,0));
#pragma message("to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));")
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==ModellingHypothesis::PLANESTRESS){
      	t2tost2<2u,real> De;
      	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::PLANESTRESS,
						    mfront::ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestCastemfiniteStrainRotationMatrix2D m(&drot(0,0));
#pragma message("to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));")
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==ModellingHypothesis::PLANESTRAIN){
      	t2tost2<2u,real> De;
      	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::PLANESTRAIN,
						    mfront::ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestCastemfiniteStrainRotationMatrix2D m(&drot(0,0));
#pragma message("to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));")
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      	t2tost2<2u,real> De;
      	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::GENERALISEDPLANESTRAIN,
						    mfront::ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestCastemfiniteStrainRotationMatrix2D m(&drot(0,0));
#pragma message("to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));")
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      	t2tost2<3u,real> De;
      	MTestCastemComputeFiniteStrainStiffnessTensor<ModellingHypothesis::TRIDIMENSIONAL,
						      mfront::ORTHOTROPIC>::exe(&mp(0),De);
	//      	MTestCastemfiniteStrainRotationMatrix3D m(&drot(0,0));
#pragma message("to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));")
      	for(i=0;i!=6u;++i){
      	  for(j=0;j!=9u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else {
	throw(runtime_error("CastemFiniteStrainBehaviour::computeElasticMatrix: "
			    "unsupported modelling hypothesis"));
      }
    } else {
      throw(runtime_error("CastemFiniteStrainBehaviour::integrate: "
			  "invalid behaviour type (neither isotropic or orthotropic)"));
    }

  }

  void
  CastemFiniteStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(EvolutionManager& mp,
									     const EvolutionManager& evm) const
  {
    const auto h = this->getHypothesis();
    CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,evm);
    if(this->stype==0){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion",0.);
    } else if(this->stype==1){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion1",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion2",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion3",0.);
    } else {
      throw(std::runtime_error("CastemSmallStrainBehaviour::"
			       "setOptionalMaterialPropertiesDefaultValues : "
			       "unsupported symmetry type"));
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"PlateWidth",0.);
    }
  } // end of MTestCastemFiniteStrainStrainBehaviour::setOptionalMaterialPropertiesDefaultValues
      
  CastemFiniteStrainBehaviour::~CastemFiniteStrainBehaviour()
  {}
  
} // end of namespace mtest

