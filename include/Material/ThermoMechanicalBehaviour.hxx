/*!
 * \file   ThermoMechanicalBehaviour.hxx
 * \brief  This file declares the ThermoMechanicalBehaviour class
 * \author Helfer Thomas
 * \date   27 Sep 2006
 */

#ifndef _LIB_TFEL_THERMOMECHANICALBEHAVIOUR_H_
#define _LIB_TFEL_THERMOMECHANICALBEHAVIOUR_H_ 

#include"Material/ThermalBehaviour.hxx"
#include"Material/MechanicalBehaviour.hxx"

namespace tfel{

  namespace material{

    /*!
     * \class ThermoMechanicalBehaviour
     * \brief This class declares an interface for thermomechanical behaviours.
     * \param unsigned short N, space dimension.
     * \param typename T, numerical type.
     * \param bool use_qt, says if one shall use quantities.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template<unsigned short N,typename NumType, bool use_qt>
    class ThermoMechanicalBehaviour
      : public ThermalBehaviour<N,NumType,use_qt>,
	public MechanicalBehaviour<N,NumType,use_qt>
    {};


  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_THERMOMECHANICALBEHAVIOUR_H */

