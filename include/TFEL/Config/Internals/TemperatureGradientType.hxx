/*!
 * \file   TemperatureGradientType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13/02/2019
 */

#ifndef LIB_TFEL_CONFIG_INTERNALS_TEMPERATUREGRADIENTTYPE_HXX
#define LIB_TFEL_CONFIG_INTERNALS_TEMPERATUREGRADIENTTYPE_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"

namespace tfel::config::internals {

  template <unsigned short N, typename T, bool use_qt>
  struct TemperatureGradientType {
    typedef tfel::math::
        tvector<N, tfel::math::qt<tfel::math::TemperatureGradient, T>>
            type;
  };  // end of struct TemperatureGradientVectorType

  template <unsigned short N, typename T>
  struct TemperatureGradientType<N, T, false> {
    typedef tfel::math::tvector<N, T> type;
  };  // end of struct TemperatureGradientVectorType

}  // end of namespace tfel::config::internals

#endif /* LIB_TFEL_CONFIG_INTERNALS_TEMPERATUREGRADIENTTYPE_HXX */
