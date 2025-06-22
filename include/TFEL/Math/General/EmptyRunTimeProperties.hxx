/*!
 * \file   include/TFEL/Math/General/EmptyRunTimeProperties.hxx
 * \brief  This file describes the EmptyRunTimeProperties class
 * \author Helfer Thomas
 * \date   03 oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_EMPTYRUNTIMEPROPERTIES_HXX_
#define LIB_TFEL_EMPTYRUNTIMEPROPERTIES_HXX_

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace math {

    /*!
     * \class EmptyRunTimeProperties
     * \brief This class is aimed to used for mathematical objects whose
     * size are known at compile-tme.
     * \see tvector, tmatrix, stensor
     */
    struct EmptyRunTimeProperties {
      EmptyRunTimeProperties() = default;
      EmptyRunTimeProperties(const EmptyRunTimeProperties&) = default;
      TFEL_MATH_INLINE constexpr EmptyRunTimeProperties(
          const EmptyRunTimeProperties&, const EmptyRunTimeProperties&) {}
      EmptyRunTimeProperties(EmptyRunTimeProperties&&) = default;
      EmptyRunTimeProperties& operator=(const EmptyRunTimeProperties&) =
          default;
    };

    inline constexpr bool operator==(const EmptyRunTimeProperties&,
                                     const EmptyRunTimeProperties&) {
      return true;
    }

    inline constexpr bool operator!=(const EmptyRunTimeProperties&,
                                     const EmptyRunTimeProperties&) {
      return false;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_EMPTYRUNTIMEPROPERTIES_HXX_ */
