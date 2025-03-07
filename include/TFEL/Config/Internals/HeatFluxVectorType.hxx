/*!
 * \file   include/TFEL/Config/Internals/HeatFluxVectorType.hxx
 * \brief  This file declares the  class
 * \author Thomas Helfer
 * \date   25 Sep 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_HEATFLUXVECTORTYPE_HXX
#define LIB_TFEL_HEATFLUXVECTORTYPE_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"

namespace tfel::config::internals {

  template <unsigned short N, typename T, bool use_qt>
  struct HeatFluxVectorType {
    typedef tfel::math::tvector<N,
                                tfel::math::qt<tfel::math::HeatFluxDensity, T>>
        type;
  };

  template <unsigned short N, typename T>
  struct HeatFluxVectorType<N, T, false> {
    typedef tfel::math::tvector<N, T> type;
  };

}  // end of namespace tfel::config::internals

#endif /* LIB_TFEL_HEATFLUXVECTORTYPE_HXX */
