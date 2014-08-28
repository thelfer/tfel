/*!
 * \file   mfront/src/CyranoComputeStiffnessTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include<algorithm>

#include"TFEL/Math/st2tost2.hxx"
#include"MFront/Cyrano/CyranoComputeStiffnessTensor.hxx"

namespace cyrano
{
 
  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ISOTROPIC,true>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						    const CyranoReal* const props)
					       
  {
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
  } // end of struct CyranoComputeStiffnessTensor

  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ISOTROPIC,false>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						    const CyranoReal* const props)
  {
    using namespace tfel::material;
    CyranoComputeStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				 ISOTROPIC,true>::exe(C,props);
  } // end of struct CyranoComputeStiffnessTensor

  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ISOTROPIC,true>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						    const CyranoReal* const props)
  {
    using namespace tfel::material;
    CyranoComputeStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				 ISOTROPIC,true>::exe(C,props);
  } // end of struct CyranoComputeStiffnessTensor

  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ISOTROPIC,false>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						     const CyranoReal* const props)
  {
    const CyranoReal E   = props[0];
    const CyranoReal n   = props[1];
    const CyranoReal C1  = E/(1+n*n);
    const CyranoReal C2  = n*C1;
    C(0,0)=C1;
    C(0,1)=C2;
    C(1,0)=C2;
    C(1,1)=C1;
    C(2,2)=0.;
    C(0,2)=0.;
    C(1,2)=0.;
    C(2,0)=0.;
    C(2,1)=0.;
  } // end of struct CyranoComputeStiffnessTensor

  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ORTHOTROPIC,true>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						      const CyranoReal* const props)
						 
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
  } // end of struct CyranoComputeStiffnessTensor

  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			       ORTHOTROPIC,false>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						      const CyranoReal* const props)
						 
  {
    using namespace tfel::material;
    CyranoComputeStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				 ORTHOTROPIC,true>::exe(C,props);
  } // end of struct CyranoComputeStiffnessTensor

  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ORTHOTROPIC,true>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						 const CyranoReal* const props)
						 
  {
    using namespace tfel::material;
    CyranoComputeStiffnessTensor<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				 ORTHOTROPIC,true>::exe(C,props);
  } // end of struct CyranoComputeStiffnessTensor

  void
  CyranoComputeStiffnessTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS,
			       ORTHOTROPIC,false>::exe(tfel::config::Types<1u,CyranoReal,false>::StiffnessTensor& C,
						       const CyranoReal* const props)
						 
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
    const CyranoReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),CyranoReal(0.));
    const CyranoReal C00=(S22*S33-S23*S23)*inv_detS;
    const CyranoReal C01=(S13*S23-S12*S33)*inv_detS;
    const CyranoReal C02=(S12*S23-S13*S22)*inv_detS;
    const CyranoReal C10=(S13*S23-S12*S33)*inv_detS;
    const CyranoReal C11=(S11*S33-S13*S13)*inv_detS;
    const CyranoReal C12=(S12*S13-S11*S23)*inv_detS;
    const CyranoReal C20=(S12*S23-S13*S22)*inv_detS;
    const CyranoReal C21=(S12*S13-S11*S23)*inv_detS;
    const CyranoReal C22=(S11*S22-S12*S12)*inv_detS;
    const CyranoReal tmp20 = C20/C22;
    const CyranoReal tmp21 = C21/C22;
    C(0,0)=C00-C02*tmp20;
    C(0,1)=C01-C02*tmp21;
    C(1,0)=C10-C12*tmp20;
    C(1,1)=C11-C12*tmp21;
  } // end of struct CyranoComputeStiffnessTensor


} // end of namespace cyrano
