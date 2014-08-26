/*!
 * \file   CyranoComputeStiffnessOperator.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include<algorithm>

#include"TFEL/Math/t2tost2.hxx"
#include"MFront/Cyrano/CyranoComputeStiffnessOperator.hxx"

namespace cyrano
{
 
  void
  CyranoComputeStiffnessOperator<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				 ISOTROPIC>::exe(const CyranoReal* const props,
						 tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C)
  {
    using namespace std;
    const CyranoReal E = props[0];
    const CyranoReal n = props[1];
    const CyranoReal l = E*n/((1.-2.*n)*(1+n));
    const CyranoReal G = E/(1+n);
    const CyranoReal C11 = l+G;
    C(0,0)=C11;
    C(0,1)=l;
    C(0,2)=l;
    C(1,0)=l;
    C(1,1)=C11;
    C(1,2)=l;
    C(2,0)=l;
    C(2,1)=l;
    C(2,2)=C11;
  } // end of struct CyranoComputeStiffnessOperator

  void
  CyranoComputeStiffnessOperator<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				 ORTHOTROPIC>::exe(const CyranoReal* const props,
						   tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C)
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
    const CyranoReal S11=1/props[0];
    // S22 = 1/E2
    const CyranoReal S22=1/props[1];
    // S22 = 1/E3
    const CyranoReal S33=1/props[2];
    // S12 = -n12/E1
    const CyranoReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const CyranoReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const CyranoReal S23=-props[4]/props[1];
    const CyranoReal detS=S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12;
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
  } // end of struct CyranoComputeStiffnessOperator

} // end of namespace cyrano
