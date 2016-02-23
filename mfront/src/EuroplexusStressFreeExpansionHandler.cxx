/*! 
 * \file  mfront/src/EuroplexusStressFreeExpansionHandler.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include"MFront/Europlexus/EuroplexusStressFreeExpansionHandler.hxx"

namespace epx
{

  void EuroplexusStandardSmallStrainStressFreeExpansionHandler(EuroplexusReal * const e,
							       EuroplexusReal * const de,
							       const EuroplexusReal *const s0,
							       const EuroplexusReal *const s1,
							       const EuroplexusInt d)
  {
    using namespace std;
    static const EuroplexusReal cste = EuroplexusReal(2)/sqrt(EuroplexusReal(2));
    e[0]  -= s0[0];
    e[1]  -= s0[1];
    e[2]  -= s0[2];
    de[0] -= (s1[0]-s0[0]);
    de[1] -= (s1[1]-s0[1]);
    de[2] -= (s1[2]-s0[2]);
    if(d==2){
      e[3]  -= s0[3]*cste;
      de[3] -= (s1[3]-s0[3])*cste;
    }
    if(d==3){
      e[3]  -= s0[3]*cste;
      e[4]  -= s0[3]*cste;
      e[5]  -= s0[5]*cste;
      de[3] -= (s1[3]-s0[3])*cste;
      de[4] -= (s1[4]-s0[4])*cste;
      de[5] -= (s1[5]-s0[5])*cste;
    }
  } // end of EuroplexusStandardSmallStrainStressFreeExpansionHandler

} // end of namespace epx
