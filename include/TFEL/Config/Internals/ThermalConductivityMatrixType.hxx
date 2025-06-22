/*!
 * \file   include/TFEL/Config/Internals/ThermalConductivityMatrixType.hxx
 * \brief  This file declares the  class
 * \author Thomas Helfer
 * \date   25 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_THERMALCONDUCTIVITYMATRIXTYPE_HXX
#define LIB_TFEL_THERMALCONDUCTIVITYMATRIXTYPE_HXX

#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"

namespace tfel::config::internals {

  template <unsigned short N, typename T, bool use_qt>
  struct ThermalConductivityMatrixType {
    typedef tfel::math::
        tmatrix<N, N, tfel::math::qt<tfel::math::ThermalConductivity, T>>
            type;
  };

  template <unsigned short N, typename T>
  struct ThermalConductivityMatrixType<N, T, false> {
    typedef tfel::math::tmatrix<N, N, T> type;
  };

  template <typename T, bool use_qt>
  struct ThermalConductivityMatrixType<1u, T, use_qt> {
    typedef tfel::math::qt<tfel::math::ThermalConductivity, T> type;
  };

  template <typename T>
  struct ThermalConductivityMatrixType<1u, T, false> {
    typedef T type;
  };

}  // end of namespace tfel::config::internals

#endif /* LIB_TFEL_THERMALCONDUCTIVITYMATRIXTYPE_HXX */
