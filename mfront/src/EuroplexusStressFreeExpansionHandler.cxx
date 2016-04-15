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
#include"TFEL/Math/Stensor/StensorView.hxx"
#include"TFEL/Math/Stensor/ConstStensorView.hxx"
#include"MFront/Europlexus/EuroplexusStressFreeExpansionHandler.hxx"

namespace epx
{

  void EuroplexusStandardSmallStrainStressFreeExpansionHandler(EuroplexusReal * const e,
							       EuroplexusReal * const de,
							       const EuroplexusReal *const s0,
							       const EuroplexusReal *const s1,
							       const EuroplexusInt d)
  {
    if(d==0){
      tfel::math::StensorView<3u,EuroplexusReal> se(e);
      tfel::math::StensorView<3u,EuroplexusReal> sde(de);
      tfel::math::ConstStensorView<3u,EuroplexusReal> se0(s0);
      tfel::math::ConstStensorView<3u,EuroplexusReal> se1(s1);
      se  -= se0;
      sde -= se1-se0;
    } else {
      tfel::math::StensorView<2u,EuroplexusReal> se(e);
      tfel::math::StensorView<2u,EuroplexusReal> sde(de);
      tfel::math::ConstStensorView<2u,EuroplexusReal> se0(s0);
      tfel::math::ConstStensorView<2u,EuroplexusReal> se1(s1);
      se  -= se0;
      sde -= se1-se0;
    }
  } // end of EuroplexusStandardSmallStrainStressFreeExpansionHandler

} // end of namespace epx
