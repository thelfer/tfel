/*!
 * \file   include/TFEL/Math/General/Tolerances.hxx
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

#ifndef LIB_TFEL_MATH_TOLERANCES_HXX
#define LIB_TFEL_MATH_TOLERANCES_HXX

#include <cmath>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::math {

  /*!
   * \brief  a struct which contains tolerances values for Eshelby tensors
   */
  template <std::floating_point NumericType>
  struct EshelbyTolerances {
  
    const NumericType precf = 8e-3 ;
    const NumericType precd =  1.5e-4 ;
    const NumericType precld =  1e-5 ;
    
    TFEL_HOST_DEVICE static constexpr auto get() {
    if constexpr (std::same_as<NumericType, long double>) {
        return NumericType(1e-5);
      } else if constexpr (std::same_as<NumericType, double>) {
        return NumericType(1.5e-4);
      } else if constexpr (std::same_as<NumericType, float>) {
        return NumericType(8e-3);
      } else {
        return NumericType(8e-3);
      }
    }
    
   };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TOLERANCES_HXX */
