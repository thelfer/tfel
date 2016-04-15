/*! 
 * \file  mfront/src/CastemStressFreeExpansionHandler.cxx
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
#include"MFront/Castem/CastemStressFreeExpansionHandler.hxx"

namespace castem
{

  void CastemStandardSmallStrainStressFreeExpansionHandler(CastemReal * const e,
							   CastemReal * const de,
							   const CastemReal *const s0,
							   const CastemReal *const s1,
							   const CastemInt d)
  {
    using namespace std;
    static const CastemReal cste = CastemReal(2)/sqrt(CastemReal(2));
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
  } // end of CastemStandardSmallStrainStressFreeExpansionHandler

} // end of namespace castem
