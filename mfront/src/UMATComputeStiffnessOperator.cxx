/*!
 * \file   UMATComputeStiffnessOperator.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#include<algorithm>

#include"TFEL/Math/t2tost2.hxx"
#include"MFront/UMAT/UMATComputeStiffnessOperator.hxx"

namespace umat
{
 
  static void
  UMATComputeIsotropicStiffnessOperator2D(const UMATReal* const props,
					tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    const UMATReal E   = props[0];
    const UMATReal n   = props[1];
    const UMATReal l   = E*n/((1.-2.*n)*(1+n));
    const UMATReal G   = E/(1+n);
    const UMATReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(0,3)=0.;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(1,3)=0.;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
    C(2,3)=0.;
    C(3,0)=0.;
    C(3,1)=0.;
    C(3,2)=0.;
    C(3,3)=G;    
  }

  static void
  UMATComputeIsotropicFiniteStrainStiffnessOperator2D(const UMATReal* const props,
						      tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    using namespace std;
    static const UMATReal cste = sqrt(UMATReal(2));
    const UMATReal E   = props[0];
    const UMATReal n   = props[1];
    const UMATReal l   = E*n/((1.-2.*n)*(1+n));
    const UMATReal G   = E/(2*(1+n));
    const UMATReal C11 = l+2*G;
    fill(C.begin(),C.end(),UMATReal(0.));
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
  UMATComputeOrthotropicStiffnessOperator2D(const UMATReal* const props,
					  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
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
    // S11 = 1/E1
    const UMATReal S11=1/props[0];
    // S22 = 1/E2
    const UMATReal S22=1/props[1];
    // S22 = 1/E3
    const UMATReal S33=1/props[2];
    // S12 = -n12/E1
    const UMATReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const UMATReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const UMATReal S23=-props[4]/props[1];
    const UMATReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
    // Matrice de raideur
    fill(C.begin(),C.end(),UMATReal(0.));
    C(0,0)=(S22*S33-S23*S23)/detS;
    C(0,1)=(S13*S23-S12*S33)/detS;
    C(0,2)=(S12*S23-S13*S22)/detS;
    C(1,0)=(S13*S23-S12*S33)/detS;
    C(1,1)=(S11*S33-S13*S13)/detS;
    C(1,2)=(S12*S13-S11*S23)/detS;
    C(2,0)=(S12*S23-S13*S22)/detS;
    C(2,1)=(S12*S13-S11*S23)/detS;
    C(2,2)=(S11*S22-S12*S12)/detS;
    C(3,3)=2.*props[6];
  } // end of struct UMATComputeStiffnessOperator

  static void
  UMATComputeOrthotropicFiniteStrainStiffnessOperator2D(const UMATReal* const props,
							tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
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
    static const UMATReal cste = sqrt(UMATReal(2));
    // S11 = 1/E1
    const UMATReal S11=1/props[0];
    // S22 = 1/E2
    const UMATReal S22=1/props[1];
    // S22 = 1/E3
    const UMATReal S33=1/props[2];
    // S12 = -n12/E1
    const UMATReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const UMATReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const UMATReal S23=-props[4]/props[1];
    const UMATReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
    // Matrice de raideur
    fill(C.begin(),C.end(),UMATReal(0.));
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
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<1u,UMATReal,false>::StiffnessTensor& C)
  {
    using namespace std;
    const UMATReal E = props[0];
    const UMATReal n = props[1];
    const UMATReal l = E*n/((1.-2.*n)*(1+n));
    const UMATReal G = E/(1+n);
    const UMATReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::PLANESTRESS,
			       ISOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    using namespace std;
    const UMATReal E   = props[0];
    const UMATReal n   = props[1];
    const UMATReal C1  = E/(1+n*n);
    const UMATReal C2  = n*C1;
    const UMATReal C3  = (1-n)*C1;
    C(0,0)=C1;
    C(0,1)=C2;
    C(0,2)=0.;
    C(0,3)=0.;
    C(1,0)=C2;
    C(1,1)=C1;
    C(1,2)=0.;
    C(1,3)=0.;
    C(2,0)=0.;
    C(2,1)=0.;
    C(2,2)=0.;
    C(2,3)=0.;
    C(3,0)=0.;
    C(3,1)=0.;
    C(3,2)=0.;
    C(3,3)=C3;    
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICAL,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    UMATComputeIsotropicStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRAIN,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    UMATComputeIsotropicStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    UMATComputeIsotropicStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<3u,UMATReal,false>::StiffnessTensor& C)
  {
    const UMATReal E = props[0];
    const UMATReal n = props[1];
    const UMATReal l = E*n/((1.-2.*n)*(1+n));
    const UMATReal G = E/(1+n);
    const UMATReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(0,3)=0.;
    C(0,4)=0.;
    C(0,5)=0.;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(1,3)=0.;
    C(1,4)=0.;
    C(1,5)=0.;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
    C(2,3)=0.;
    C(2,4)=0.;
    C(2,5)=0.;
    C(3,0)=0.;
    C(3,1)=0.;
    C(3,2)=0.;
    C(3,3)=G;
    C(3,4)=0.;
    C(3,5)=0.;
    C(4,0)=0.;
    C(4,1)=0.;
    C(4,2)=0.;
    C(4,3)=0.;
    C(4,4)=G;
    C(4,5)=0.;
    C(5,0)=0.;
    C(5,1)=0.;
    C(5,2)=0.;
    C(5,3)=0.;
    C(5,4)=0.;
    C(5,5)=G;
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<1u,UMATReal,false>::StiffnessTensor& C)
  {
    using namespace std;
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
    const UMATReal S11=1/props[0];
    // S22 = 1/E2
    const UMATReal S22=1/props[1];
    // S22 = 1/E3
    const UMATReal S33=1/props[2];
    // S12 = -n12/E1
    const UMATReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const UMATReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const UMATReal S23=-props[4]/props[1];
    const UMATReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
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
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRESS,
			     ORTHOTROPIC>::exe(const UMATReal* const,
					       tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&)
  {
    using namespace std;
#warning "something needs to be done HERE"
    string msg("UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,"
	       "tfel::material::ModellingHypothesis::PLANESTRESS"
	       ",ORTHOTROPIC>::exe : ");
    msg += "unimplemented feature";
    throw(runtime_error(msg));
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICAL,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    UMATComputeOrthotropicStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRAIN,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    UMATComputeOrthotropicStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C)
  {
    UMATComputeOrthotropicStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<3u,UMATReal,false>::StiffnessTensor& C)
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
    // S11 = 1/E1
    const UMATReal S11=1/props[0];
    // S22 = 1/E2
    const UMATReal S22=1/props[1];
    // S22 = 1/E3
    const UMATReal S33=1/props[2];
    // S12 = -n12/E1
    const UMATReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const UMATReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const UMATReal S23=-props[4]/props[1];
    const UMATReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
    // Matrice de raideur
    fill(C.begin(),C.end(),UMATReal(0.));
    C(0,0)=(S22*S33-S23*S23)/detS;
    C(1,1)=(S11*S33-S13*S13)/detS;
    C(2,2)=(S11*S22-S12*S12)/detS;
    C(0,1)=C(1,0)=(S13*S23-S12*S33)/detS;
    C(0,2)=C(2,0)=(S12*S23-S13*S22)/detS;
    C(1,2)=C(2,1)=(S12*S13-S11*S23)/detS;
    C(3,3)=2.*props[6];
    C(4,4)=2.*props[8];
    C(5,5)=2.*props[7];
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<1u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    using namespace std;
    const UMATReal E = props[0];
    const UMATReal n = props[1];
    const UMATReal l = E*n/((1.-2.*n)*(1+n));
    const UMATReal G = E/(1+n);
    const UMATReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::PLANESTRESS,
			       ISOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    using namespace std;
    static const UMATReal cste = sqrt(2);
    const UMATReal E   = props[0];
    const UMATReal n   = props[1];
    const UMATReal C1  = E/(1+n*n);
    const UMATReal C2  = n*C1;
    const UMATReal C3  = (1-n)*C1;
    fill(C.begin(),C.end(),UMATReal(0.));
    C(0,0)=C1;
    C(0,1)=C2;
    C(1,0)=C2;
    C(1,1)=C1;
    C(3,3)=cste*C3;
    C(3,4)=cste*C3;    
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICAL,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    UMATComputeIsotropicFiniteStrainStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRAIN,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    UMATComputeIsotropicFiniteStrainStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    UMATComputeIsotropicFiniteStrainStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
			     ISOTROPIC>::exe(const UMATReal* const props,
					     tfel::config::Types<3u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    using namespace std;
    static const UMATReal cste = sqrt(2);
    const UMATReal E = props[0];
    const UMATReal n = props[1];
    const UMATReal l = E*n/((1.-2.*n)*(1+n));
    const UMATReal G = E/(2*(1+n));
    const UMATReal C11 = l+2*G;
    fill(C.begin(),C.end(),UMATReal(0.));
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
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<1u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    using namespace std;
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
    const UMATReal S11=1/props[0];
    // S22 = 1/E2
    const UMATReal S22=1/props[1];
    // S22 = 1/E3
    const UMATReal S33=1/props[2];
    // S12 = -n12/E1
    const UMATReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const UMATReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const UMATReal S23=-props[4]/props[1];
    const UMATReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
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
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRESS,
			     ORTHOTROPIC>::exe(const UMATReal* const,
					       tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor&)
  {
    using namespace std;
#warning "something needs to be done HERE"
    string msg("UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,"
	       "tfel::material::ModellingHypothesis::PLANESTRESS"
	       ",ORTHOTROPIC>::exe : ");
    msg += "unimplemented feature";
    throw(runtime_error(msg));
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICAL,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    UMATComputeOrthotropicFiniteStrainStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRAIN,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    UMATComputeOrthotropicFiniteStrainStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
  {
    UMATComputeOrthotropicFiniteStrainStiffnessOperator2D(props,C);
  } // end of struct UMATComputeStiffnessOperator

  void
  UMATComputeStiffnessOperator<umat::FINITESTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
			     ORTHOTROPIC>::exe(const UMATReal* const props,
					       tfel::config::Types<3u,UMATReal,false>::FiniteStrainStiffnessTensor& C)
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
    static const UMATReal cste = sqrt(UMATReal(2));
    // S11 = 1/E1
    const UMATReal S11=1/props[0];
    // S22 = 1/E2
    const UMATReal S22=1/props[1];
    // S22 = 1/E3
    const UMATReal S33=1/props[2];
    // S12 = -n12/E1
    const UMATReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const UMATReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const UMATReal S23=-props[4]/props[1];
    const UMATReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
    // Matrice de raideur
    fill(C.begin(),C.end(),UMATReal(0.));
    C(0,0)=(S22*S33-S23*S23)/detS;
    C(1,1)=(S11*S33-S13*S13)/detS;
    C(2,2)=(S11*S22-S12*S12)/detS;
    C(0,1)=C(1,0)=(S13*S23-S12*S33)/detS;
    C(0,2)=C(2,0)=(S12*S23-S13*S22)/detS;
    C(1,2)=C(2,1)=(S12*S13-S11*S23)/detS;
    C(3,3)=cste*props[6];
    C(3,4)=cste*props[6];
    C(4,5)=cste*props[8];
    C(4,6)=cste*props[8];
    C(5,7)=cste*props[8];
    C(5,8)=cste*props[8];
  } // end of struct UMATComputeStiffnessOperator

} // end of namespace umat
