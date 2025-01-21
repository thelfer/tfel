/*!
 * \file   include/TFEL/Material/LinearHomogenizationBounds.ixx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file defines some homogenization schemes based on solution of
 * Eshelby problem. \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_IXX
#define LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_IXX

#include <cmath>

namespace tfel::material::homogenization::elasticity {

  template <unsigned short int d, unsigned int N, typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<d, StressType>
  computeVoigtStiffness(const std::array<real,N> &tab_f,
                      const std::array<tfel::math::st2tost2<d, StressType>,N> &tab_C){
    tfel::math::st2tost2<d,StressType> C_V=tab_f[0]*tab_C[0];
    for (int i=1;i<N;i++){
      C_V+=tab_f[i]*tab_C[i];
    }
    return C_V;
  }

  template <unsigned short int d, unsigned int N, typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<d, StressType>
  computeReussStiffness(const std::array<real,N> &tab_f,
                      const std::array<tfel::math::st2tost2<d, StressType>,N> &tab_C){
    tfel::math::st2tost2<d,tfel::math::invert_type<StressType>> S_R=tab_f[0]*invert(tab_C[0]);
    for (int i=1;i<N;i++){
      S_R+=tab_f[i]*invert(tab_C[i]);
    }
    return invert(S_R);
  }
  
  template <unsigned short int d, unsigned int N, typename real, typename StressType>
  TFEL_HOST_DEVICE const std::pair<StressType, real>
  computeIsotropicHashinShtrikmanBounds(const std::array<real,N> &tab_f,
                      const std::array<StressType,N> &tab_E,
                      const std::array<real,N> &tab_nu){
                      
   }
                      
   template <typename real, typename StressType>
  TFEL_HOST_DEVICE const std::pair<StressType, real>
  computeThreePointMiltonBounds(const std::array<real,2> &tab_f,
                      const std::array<StressType,2> &tab_E,
                      const std::array<real,2> &tab_nu){
                      
                                          
  }


  

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONBOUNDS_IXX */
