/*! 
 * \file  mfront/src/UMATStressFreeExpansionHandler.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 mars 2014
 */

#include<cmath>
#include"MFront/UMAT/UMATStressFreeExpansionHandler.hxx"

namespace umat
{

  void UMATStandardSmallStrainStressFreeExpansionHandler(UMATReal * const e,
							 UMATReal * const de,
							 const UMATReal *const s0,
							 const UMATReal *const s1,
							 const UMATInt d)
  {
    using namespace std;
    static const UMATReal cste = UMATReal(2)/sqrt(UMATReal(2));
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
  } // end of UMATStandardSmallStrainStressFreeExpansionHandler

} // end of namespace umat
