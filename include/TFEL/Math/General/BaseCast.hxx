/*!
 * \file   include/TFEL/Math/General/BaseCast.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   17 jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_BASECAST_HXX
#define LIB_TFEL_BASECAST_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/Forward/qt.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    TFEL_MATH_INLINE const typename tfel::typetraits::BaseType<T>::type
    base_cast(const T x) {
      return x;
    }

    template <typename T, typename Unit>
    TFEL_MATH_INLINE const typename tfel::typetraits::BaseType<
        qt<T, Unit>>::type
    base_cast(const qt<T, Unit> x) {
      return x.getValue();
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_BASECAST_HXX */
