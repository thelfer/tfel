/*!
 * \file   mfront/src/EuroplexusComputeStiffnessTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>

#include"TFEL/Math/st2tost2.hxx"
#include"MFront/Europlexus/EuroplexusComputeStiffnessTensor.hxx"

namespace epx
{
 
  void
  EuroplexusComputeIsotropicStiffnessTensor2D(tfel::config::Types<2u,EuroplexusReal,false>::StiffnessTensor& C,
					      const EuroplexusReal* const props)
  {
    const EuroplexusReal E   = props[0];
    const EuroplexusReal n   = props[1];
    const EuroplexusReal l   = E*n/((1.-2.*n)*(1+n));
    const EuroplexusReal G   = E/(1+n);
    const EuroplexusReal C11 = l+G;
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

  void
  EuroplexusComputeIsotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,EuroplexusReal,false>::StiffnessTensor& C,
							      const EuroplexusReal* const props)
					       
  {
    const EuroplexusReal E   = props[0];
    const EuroplexusReal n   = props[1];
    const EuroplexusReal C1  = E/(1-n*n);
    const EuroplexusReal C2  = n*C1;
    const EuroplexusReal C3  = (1-n)*C1;
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
  } // end of struct EuroplexusComputeIsotropicPlaneStressAlteredStiffnessTensor

  void
  EuroplexusComputeIsotropicStiffnessTensor3D(tfel::config::Types<3u,EuroplexusReal,false>::StiffnessTensor& C,
					      const EuroplexusReal* const props)
  {
    const EuroplexusReal E = props[0];
    const EuroplexusReal n = props[1];
    const EuroplexusReal l = E*n/((1.-2.*n)*(1+n));
    const EuroplexusReal G = E/(1+n);
    const EuroplexusReal C11 = l+G;
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
  } // end of struct EuroplexusComputeStiffnessTensor

  void
  EuroplexusComputeOrthotropicStiffnessTensor2D(tfel::config::Types<2u,EuroplexusReal,false>::StiffnessTensor& C,
						const EuroplexusReal* const props)
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
    const EuroplexusReal S11=1/props[0];
    // S22 = 1/E2
    const EuroplexusReal S22=1/props[1];
    // S22 = 1/E3
    const EuroplexusReal S33=1/props[2];
    // S12 = -n12/E1
    const EuroplexusReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const EuroplexusReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const EuroplexusReal S23=-props[4]/props[1];
    const EuroplexusReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),EuroplexusReal(0.));
    C(0,0)=(S22*S33-S23*S23)*inv_detS;
    C(0,1)=(S13*S23-S12*S33)*inv_detS;
    C(0,2)=(S12*S23-S13*S22)*inv_detS;
    C(1,0)=(S13*S23-S12*S33)*inv_detS;
    C(1,1)=(S11*S33-S13*S13)*inv_detS;
    C(1,2)=(S12*S13-S11*S23)*inv_detS;
    C(2,0)=(S12*S23-S13*S22)*inv_detS;
    C(2,1)=(S12*S13-S11*S23)*inv_detS;
    C(2,2)=(S11*S22-S12*S12)*inv_detS;
    C(3,3)=2.*props[6];
  } // end of struct EuroplexusComputeStiffnessTensor

  void
  EuroplexusComputeOrthotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,EuroplexusReal,false>::StiffnessTensor& C,
								const EuroplexusReal* const props)
  {
    using namespace std;
    // S11 = 1/E1
    const EuroplexusReal S11=1/props[0];
    // S22 = 1/E2
    const EuroplexusReal S22=1/props[1];
    // S22 = 1/E3
    const EuroplexusReal S33=1/props[2];
    // S12 = -n12/E1
    const EuroplexusReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const EuroplexusReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const EuroplexusReal S23=-props[4]/props[1];
    const EuroplexusReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),EuroplexusReal(0.));
    const EuroplexusReal C00=(S22*S33-S23*S23)*inv_detS;
    const EuroplexusReal C01=(S13*S23-S12*S33)*inv_detS;
    const EuroplexusReal C02=(S12*S23-S13*S22)*inv_detS;
    const EuroplexusReal C10=(S13*S23-S12*S33)*inv_detS;
    const EuroplexusReal C11=(S11*S33-S13*S13)*inv_detS;
    const EuroplexusReal C12=(S12*S13-S11*S23)*inv_detS;
    const EuroplexusReal C20=(S12*S23-S13*S22)*inv_detS;
    const EuroplexusReal C21=(S12*S13-S11*S23)*inv_detS;
    const EuroplexusReal C22=(S11*S22-S12*S12)*inv_detS;
    const EuroplexusReal tmp20 = C20/C22;
    const EuroplexusReal tmp21 = C21/C22;
    C(0,0)=C00-C02*tmp20;
    C(0,1)=C01-C02*tmp21;
    C(1,0)=C10-C12*tmp20;
    C(1,1)=C11-C12*tmp21;
    C(3,3)=2.*props[6];
  } // end of struct EuroplexusComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void
  EuroplexusComputeOrthotropicStiffnessTensor3D(tfel::config::Types<3u,EuroplexusReal,false>::StiffnessTensor& C,
						const EuroplexusReal* const props)
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
    const EuroplexusReal S11=1/props[0];
    // S22 = 1/E2
    const EuroplexusReal S22=1/props[1];
    // S22 = 1/E3
    const EuroplexusReal S33=1/props[2];
    // S12 = -n12/E1
    const EuroplexusReal S12=-props[3]/props[0];
    // S13 = -n13/E1
    const EuroplexusReal S13=-props[5]/props[0];
    // S23 = -n23/E2
    const EuroplexusReal S23=-props[4]/props[1];
    const EuroplexusReal inv_detS=1/(S11*S22*S33+2*S23*S13*S12-S11*S23*S23-S22*S13*S13-S33*S12*S12);
    // Matrice de raideur
    fill(C.begin(),C.end(),EuroplexusReal(0.));
    C(0,0)=(S22*S33-S23*S23)*inv_detS;
    C(1,1)=(S11*S33-S13*S13)*inv_detS;
    C(2,2)=(S11*S22-S12*S12)*inv_detS;
    C(0,1)=C(1,0)=(S13*S23-S12*S33)*inv_detS;
    C(0,2)=C(2,0)=(S12*S23-S13*S22)*inv_detS;
    C(1,2)=C(2,1)=(S12*S13-S11*S23)*inv_detS;
    C(3,3)=2.*props[6];
    C(4,4)=2.*props[8];
    C(5,5)=2.*props[7];
  } // end of struct EuroplexusComputeStiffnessTensor

} // end of namespace epx
