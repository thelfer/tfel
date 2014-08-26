/*!
 * \file   MTestUmatFiniteStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  18 november 2013
 */

#include<iostream>

#include<cmath>
#include<algorithm>

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestUmatFiniteStrainBehaviour.hxx"
#include"MFront/MechanicalBehaviourSymmetryType.hxx"

namespace mfront
{

  static void
  MTestComputeIsotropicFiniteStrainStiffnessTensor2D(const real* const props,
						    tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
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
    C(3,3)=cste*G;    
    C(3,4)=cste*G;
  }

  static void
  UMATComputeOrthotropicFiniteStrainStiffnessTensor2D(const real* const props,
						      tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
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
    C(3,3)=cste*props[6];
    C(3,4)=cste*props[6];
  } // end of struct UMATComputeStiffnessTensor

  /*!
   * This structure is in charge of computing a stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template<tfel::material::ModellingHypothesis::Hypothesis,
	   MechanicalBehaviourSymmetryType>
  struct MTestUmatComputeFiniteStrainStiffnessTensor;

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						     mfront::ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<1u,real,false>::FiniteStrainStiffnessTensor& C)
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
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  }; 

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRESS,
						     ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      using namespace std;
      static const real cste = sqrt(2);
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
      C(3,3)=cste*C3;
      C(3,4)=cste*C3;    
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  }; 
  
  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICAL,
						     ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      MTestComputeIsotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,
						     ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      MTestComputeIsotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
						     ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      MTestComputeIsotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
						     ISOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<3u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      using namespace std;
      static const real cste = sqrt(2);
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
      C(3,3)=cste*G;
      C(3,4)=cste*G;
      C(4,5)=cste*G;
      C(4,6)=cste*G;
      C(5,7)=cste*G;
      C(5,8)=cste*G;
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };
  
  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						     ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<1u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      using namespace std;
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
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRESS,
						     ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
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
      C(3,3)=2*cste*props[3];
      C(3,4)=2*cste*props[3];    
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICAL,
						     ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      UMATComputeOrthotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::PLANESTRAIN,
						     ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      UMATComputeOrthotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
						     ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<2u,real,false>::FiniteStrainStiffnessTensor& C)
    {
      UMATComputeOrthotropicFiniteStrainStiffnessTensor2D(props,C);
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  template<>
  struct MTestUmatComputeFiniteStrainStiffnessTensor<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
						     ORTHOTROPIC>
  {
    static void
    exe(const real* const props,
	tfel::config::Types<3u,real,false>::FiniteStrainStiffnessTensor& C)
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
      C(3,3)=cste*props[6];
      C(3,4)=cste*props[6];
      C(4,5)=cste*props[7];
      C(4,6)=cste*props[7];
      C(5,7)=cste*props[8];
      C(5,8)=cste*props[8];
    } // end of struct MTestUmatComputeFiniteStrainStiffnessTensor
  };

  // struct MTestUmatfiniteStrainRotationMatrix2D
  // {
  //   MTestUmatfiniteStrainRotationMatrix2D(const real *const);
  //   // compute the stiffness matrix in the global space
  //   void rotateStiffnessMatrixBackward(real *const) const;
  // private:
  //   real MN[3][3];
  // }; // end of struct MTestUmatfiniteStrainRotationMatrix2D
  
  // struct MTestUmatfiniteStrainRotationMatrix3D
  // {
    
  //   MTestUmatfiniteStrainRotationMatrix3D(const real *const);
  //   // compute the stiffness matrix in the global space
  //   void rotateStiffnessMatrixBackward(real *const) const;
  // private:
  //   real MN[6][6];
  // }; // end of struct MTestUmatfiniteStrainRotationMatrix3D

  // // Constructeur
  // // drot : Matrice de passage élement/global
  // MTestUmatfiniteStrainRotationMatrix2D::MTestUmatfiniteStrainRotationMatrix2D(const real *const drot)
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
  // } // end of MTestUmatfiniteStrainRotationMatrix2D::MTestUmatfiniteStrainRotationMatrix2D

  // // Calcul de la déformation dans le repère global
  // // D^g=tN:D^m:N
  // void
  // MTestUmatfiniteStrainRotationMatrix2D::rotateStiffnessMatrixBackward(real *const D) const
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
  // } // end of MTestUmatfiniteStrainRotationMatrix2D::rotateStiffnessMatrixBackward

  // // Constructeur
  // // drot : Matrice de passage élement/global
  // MTestUmatfiniteStrainRotationMatrix3D::MTestUmatfiniteStrainRotationMatrix3D(const real *const drot)
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
  // } // end of MTestUmatfiniteStrainRotationMatrix3D::MTestUmatfiniteStrainRotationMatrix3D

  // // compute the stiffness matrix in the global space
  // void
  // MTestUmatfiniteStrainRotationMatrix3D::rotateStiffnessMatrixBackward(real *const D) const
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
  // } // end of MTestUmatfiniteStrainRotationMatrix3D::rotateStiffnessMatrixBackward

  MTestUmatFiniteStrainBehaviour::MTestUmatFiniteStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : MTestUmatStandardBehaviour(h,l,b)
  {
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    if(h==ModellingHypothesis::PLANESTRESS){
      ELM& elm = ELM::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	//! better name required
	this->ivnames.push_back("AxialStrain");
	this->ivtypes.push_back(0);
      }
    }
  } // end of MTestUmatFiniteStrainBehaviour::MTestUmatFiniteStrainBehaviour

  void
  MTestUmatFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
    v[0] = real(1);
    v[1] = real(1);
    v[2] = real(1);
  } // end of MTestUmatFiniteStrainBehaviour::setDrivingVariablesDefaultInitialValue  

  bool
  MTestUmatFiniteStrainBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
							    const tfel::math::tmatrix<3u,3u,real>& r,
							    const tfel::math::vector<real>&,
							    const tfel::math::vector<real>&,
							    const tfel::math::vector<real>& mp,
							    const tfel::math::vector<real>&,
							    const tfel::math::vector<real>&,
							    const tfel::material::ModellingHypothesis::Hypothesis h,
							    const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace std;
    using namespace tfel::math;
    // rotation matrix
    if(ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      // compute the stiffness operator from material properties
      tmatrix<3u,3u,real> drot(0.);
      tmatrix<3u,3u,real>::size_type i,j;
      for(i=0;i!=r.getNbRows();++i){
	for(j=0;j!=r.getNbCols();++j){
	  drot(i,j) = r(j,i);
	}
      }
      this->computeElasticStiffness(Kt,mp,drot,h);
      return true;
    }
    string msg("MTestUmatFiniteStrainBehaviour::computePredictionOperator : "
	       "computation of the tangent operator "
	       "is not supported");
    throw(runtime_error(msg));
    return false;
  } // end of MTestUmatFiniteStrainBehaviour::computePredictionOperator

  bool
  MTestUmatFiniteStrainBehaviour::integrate(tfel::math::matrix<real>& Kt,
					    tfel::math::vector<real>& s1,
					    tfel::math::vector<real>& iv1,
					    const tfel::math::tmatrix<3u,3u,real>& r,
					    const tfel::math::vector<real>& u0,
					    const tfel::math::vector<real>& u1,
					    const tfel::math::vector<real>& s0,
					    const tfel::math::vector<real>& mp,
					    const tfel::math::vector<real>& iv0,
					    const tfel::math::vector<real>& ev0,
					    const tfel::math::vector<real>& dev,
					    const tfel::material::ModellingHypothesis::Hypothesis h,
					    const real dt,
					    const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace umat;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    UMATInt ntens;
    UMATInt ndi;
    UMATInt nprops = static_cast<UMATInt>(mp.size());
    UMATInt nstatv;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 14;
      ntens = 3;
    } else if (h==MH::AXISYMMETRICAL){
      ndi = 0;
      ntens = 4;
    } else if (h==MH::PLANESTRESS){
      ndi = -2;
      ntens = 4;
    } else if (h==MH::PLANESTRAIN){
      ndi = -1;
      ntens = 4;
    } else if (h==MH::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 4;
    } else if (h==MH::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 6;
    } else {
      string msg("MTestUmatFiniteStrainBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((this->D.getNbRows()!=Kt.getNbRows())||
       (this->D.getNbCols()!=Kt.getNbCols())){
      string msg("MTestUmatFiniteStrainBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    if(((iv0.size()==0)&&(this->iv.size()!=1u))||
       ((iv0.size()!=0)&&(iv0.size()!=this->iv.size()))){
      string msg("MTestUmatFiniteStrainBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    if(iv0.size()!=0){
      copy(iv0.begin(),iv0.end(),
	   this->iv.begin());
    }
    nstatv = static_cast<UMATInt>(iv.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    UMATInt kinc(1);
    tmatrix<3u,3u,real> uu0(real(0));
    tmatrix<3u,3u,real> uu1(real(0));
    uu0(0,0) = u0(0); uu1(0,0) = u1(0);
    uu0(1,1) = u0(1); uu1(1,1) = u1(1);
    uu0(2,2) = u0(2); uu1(2,2) = u1(2);
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      uu0(1,0) = 0.; uu1(1,0) = 0.;
      uu0(0,1) = 0.; uu1(0,1) = 0.;
      uu0(2,0) = 0.; uu1(2,0) = 0.;
      uu0(0,2) = 0.; uu1(0,2) = 0.;
      uu0(2,1) = 0.; uu1(2,1) = 0.;
      uu0(1,2) = 0.; uu1(1,2) = 0.;
    } else if ((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
	       (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
      // uu0 and uu1 must be built using Fortran notations
      uu0(1,0) = u0(3); uu1(1,0) = u1(3);
      uu0(0,1) = u0(4); uu1(0,1) = u1(4);
      uu0(2,0) = 0.; uu1(2,0) = 0.;
      uu0(0,2) = 0.; uu1(0,2) = 0.;
      uu0(2,1) = 0.; uu1(2,1) = 0.;
      uu0(1,2) = 0.; uu1(1,2) = 0.;
    } else if (h==MH::TRIDIMENSIONAL){
      // uu0 and uu1 must be built using Fortran notations
      uu0(1,0) = u0(3); uu1(1,0) = u1(3);
      uu0(0,1) = u0(4); uu1(0,1) = u1(4);
      uu0(2,0) = u0(5); uu1(2,0) = u1(5);
      uu0(0,2) = u0(6); uu1(0,2) = u1(6);
      uu0(2,1) = u0(7); uu1(2,1) = u1(7);
      uu0(1,2) = u0(8); uu1(1,2) = u1(8);
    } else {
      string msg("MTestUmatFiniteStrainBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    copy(s0.begin(),s0.end(),s1.begin());
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
    }
    (this->fct)(&ntens,&dt,&drot(0,0),
		&D(0,0),&uu0(0,0),&uu1(0,0),
		&ev0(0),&dev(0),
		&mp(0),&nprops,
		&ev0(0)+1,&dev(0)+1,
		&iv(0),&nstatv,&s1(0),
		&ndi,&kinc);
    if(kinc!=1){
      return false;
    }
    if(!iv1.empty()){
      copy(iv.begin(),iv.end(),iv1.begin());
    }
    // tangent operator (...)
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,mp,drot,h);
      } else {
	string msg("MTestUmatFiniteStrainBehaviour::integrate : "
		   "computation of the tangent operator "
		   "is not supported");
	throw(runtime_error(msg));
      }
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i) *= sqrt2;
    }
    return true;
  } // end of MTestUmatFiniteStrainBehaviour::integrate

  void
  MTestUmatFiniteStrainBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
							  const tfel::math::vector<real>& mp,
							  const tfel::math::tmatrix<3u,3u,real>&,
							  const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::math;
    typedef tfel::material::ModellingHypothesis MH;
    tmatrix<3u,3u,real>::size_type i,j;
    if(this->stype==0u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	t2tost2<1u,real> De;
	MTestUmatComputeFiniteStrainStiffnessTensor<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						    ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::AXISYMMETRICAL){
	t2tost2<2u,real> De;
	MTestUmatComputeFiniteStrainStiffnessTensor<MH::AXISYMMETRICAL,
						    ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRESS){
	t2tost2<2u,real> De;
	MTestUmatComputeFiniteStrainStiffnessTensor<MH::PLANESTRESS,
						    ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRAIN){
	t2tost2<2u,real> De;
	MTestUmatComputeFiniteStrainStiffnessTensor<MH::PLANESTRAIN,
						    ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::GENERALISEDPLANESTRAIN){
	t2tost2<2u,real> De;
	MTestUmatComputeFiniteStrainStiffnessTensor<MH::GENERALISEDPLANESTRAIN,
						    ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=5u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::TRIDIMENSIONAL){
	t2tost2<3u,real> De;
	MTestUmatComputeFiniteStrainStiffnessTensor<MH::TRIDIMENSIONAL,
						    ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=6u;++i){
	  for(j=0;j!=9u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestUmatFiniteStrainBehaviour::computeElasticMatrix : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      	t2tost2<1u,real> De;
      	MTestUmatComputeFiniteStrainStiffnessTensor<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
						    ORTHOTROPIC>::exe(&mp(0),De);
      	for(i=0;i!=3u;++i){
      	  for(j=0;j!=3u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==MH::AXISYMMETRICAL){
      	t2tost2<2u,real> De;
      	MTestUmatComputeFiniteStrainStiffnessTensor<MH::AXISYMMETRICAL,
						    ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestUmatfiniteStrainRotationMatrix2D m(&drot(0,0));
#warning "to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));"
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==MH::PLANESTRESS){
      	t2tost2<2u,real> De;
      	MTestUmatComputeFiniteStrainStiffnessTensor<MH::PLANESTRESS,
						    ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestUmatfiniteStrainRotationMatrix2D m(&drot(0,0));
#warning "to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));"
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==MH::PLANESTRAIN){
      	t2tost2<2u,real> De;
      	MTestUmatComputeFiniteStrainStiffnessTensor<MH::PLANESTRAIN,
						    ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestUmatfiniteStrainRotationMatrix2D m(&drot(0,0));
#warning "to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));"
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==MH::GENERALISEDPLANESTRAIN){
      	t2tost2<2u,real> De;
      	MTestUmatComputeFiniteStrainStiffnessTensor<MH::GENERALISEDPLANESTRAIN,
						    ORTHOTROPIC>::exe(&mp(0),De);
//      	MTestUmatfiniteStrainRotationMatrix2D m(&drot(0,0));
#warning "to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));"
      	for(i=0;i!=4u;++i){
      	  for(j=0;j!=5u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else if (h==MH::TRIDIMENSIONAL){
      	t2tost2<3u,real> De;
      	MTestUmatComputeFiniteStrainStiffnessTensor<MH::TRIDIMENSIONAL,
						    ORTHOTROPIC>::exe(&mp(0),De);
	//      	MTestUmatfiniteStrainRotationMatrix3D m(&drot(0,0));
#warning "to be implemented : m.rotateStiffnessMatrixBackward(&De(0,0));"
      	for(i=0;i!=6u;++i){
      	  for(j=0;j!=9u;++j){
      	    Kt(i,j) = De(i,j);
      	  }
      	}
      } else {
	string msg("MTestUmatFiniteStrainBehaviour::computeElasticMatrix : ");
	msg += "unsupported modelling hypothesis";
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestUmatFiniteStrainBehaviour::integrate : ");
      msg += "invalid behaviour type (neither "
	"isotropic or orthotropic)";
      throw(runtime_error(msg));
    }

  }

  void
  MTestUmatFiniteStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(MTestEvolutionManager& mp,
									     const MTestEvolutionManager& evm) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    const ModellingHypothesis::Hypothesis h = ModellingHypothesis::fromString(this->hypothesis);
    MTestUmatStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,evm);
    if(this->stype==0){
      MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion",0.);
    } else if(this->stype==1){
      MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion1",0.);
      MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion2",0.);
      MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion3",0.);
    } else {
      string msg("MTestUmatSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      MTestBehaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"PlateWidth",0.);
    }
  } // end of MTestUmatFiniteStrainStrainBehaviour::setOptionalMaterialPropertiesDefaultValues
      
  MTestUmatFiniteStrainBehaviour::~MTestUmatFiniteStrainBehaviour()
  {}
  
} // end of namespace mfront

