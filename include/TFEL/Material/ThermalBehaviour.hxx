/*!
 * \file   include/TFEL/Material/ThermalBehaviour.hxx
 * \brief  This file declares the ThermalBehaviour class
 * \author Helfer Thomas
 * \date   22 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_THERMALBEHAVIOUR_H_
#define LIB_TFEL_THERMALBEHAVIOUR_H_

#include "TFEL/Material/ThermalBehaviourData.hxx"

namespace tfel {

  namespace material {

    /*!
     * \class ThermalBehaviour
     * \brief This class declares an interface for thermal behaviours.
     * \param unsigned short N, space dimension.
     * \param typename T, numerical type.
     * \param bool use_qt, says if one shall use quantities.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template <unsigned short N, typename NumType, bool use_qt>
    class ThermalBehaviour {
      typedef ThermalBehaviourData<N, NumType, use_qt> ThermalBehaviourData;

     public:
      virtual ~ThermalBehaviour(){};
    };

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_THERMALBEHAVIOUR_H_ */
