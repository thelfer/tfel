/*!
 * \file   include/TFEL/Material/ThermoMechanicalBehaviour.hxx
 * \brief  This file declares the ThermoMechanicalBehaviour class
 * \author Thomas Helfer
 * \date   27 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_THERMOMECHANICALBEHAVIOUR_HXX
#define LIB_TFEL_THERMOMECHANICALBEHAVIOUR_HXX

#include "TFEL/Material/ThermalBehaviour.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"

namespace tfel {

  namespace material {

    /*!
     * \class ThermoMechanicalBehaviour
     * \brief This class declares an interface for thermomechanical behaviours.
     * \param unsigned short N, space dimension.
     * \param typename T, numerical type.
     * \param bool use_qt, says if one shall use quantities.
     * \author Thomas Helfer
     * \date   28 Jul 2006
     */
    template <unsigned short N, typename NumType, bool use_qt>
    class ThermoMechanicalBehaviour
        : public ThermalBehaviour<N, NumType, use_qt>,
          public MechanicalBehaviour<N, NumType, use_qt> {};

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_THERMOMECHANICALBEHAVIOUR_HXX */
