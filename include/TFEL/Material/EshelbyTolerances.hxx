/*!
 * \file   include/TFEL/Material/EshelbyTolerances.hxx
 * \brief  This file declares tolerance values relative to the floatting-point types
* \author Antoine Martin
 * \date   5 sept 2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBYTOLERANCES_HXX
#define LIB_TFEL_MATERIAL_ESHELBYTOLERANCES_HXX

#include <cmath>
#include <type_traits>

namespace tfel::material {

  /*!
   * \brief  a struct which contains tolerances values for Eshelby tensors
   */
  struct EshelbyTolerances {
  
    static constexpr float precf = float{8e-3} ;
    static constexpr double precd =  double{1.5e-4} ;
    static constexpr long double precld =  static_cast<long double>(1e-5);
    
    template <std::floating_point NumericType>
    TFEL_HOST_DEVICE static constexpr auto get() {
    if constexpr (std::same_as<NumericType, long double>) {
        return precld;
      } else if constexpr (std::same_as<NumericType, double>) {
        return precd;
      } else  {
        return precf;
      } 
    }
   };

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_ESHELBYTOLERANCES_HXX */
