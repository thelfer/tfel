/*!
 * \file   include/TFEL/Material/ThermalBehaviourData.hxx
 * \brief  This file implements the ThermalBehaviourData Class
 * \author Thomas Helfer
 * \date   09 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_THERMALBEHAVIOURDATA_HXX
#define LIB_TFEL_MATERIAL_THERMALBEHAVIOURDATA_HXX

#include <vector>
#include <algorithm>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/MaterialException.hxx"

namespace tfel::material {

  /*!
   * \class ThermalBehaviourData
   * \param unsigned short N, space dimension
   * \param typename NumType, numerical type
   * \param bool use_qt, says if one uses quantities
   * \author Thomas Helfer
   * \date   09 Aug 2006
   */
  template <unsigned short N, typename NumType, bool use_qt>
  class ThermalBehaviourData {
    typedef tfel::config::Types<N, NumType, use_qt> Types;

    typedef typename Types::HeatFluxVector HeatFluxVector;
    typedef typename Types::HeatFluxVectorDerivative HeatFluxVectorDerivative;
    typedef typename Types::ThermalConductivityMatrix ThermalConductivityMatrix;

   public:
    HeatFluxVector j;
    HeatFluxVectorDerivative dj_dT;
    ThermalConductivityMatrix k;
  };

  template <typename Behaviour>
  struct CheckThermalDataValidity {};

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_THERMALBEHAVIOURDATA_HXX */
