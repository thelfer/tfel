/*!
 * \file   ThermalBehaviourData.hxx
 * \brief  This file implements the ThermalBehaviourData Class
 * \author Helfer Thomas
 * \date   09 Aug 2006
 */

#ifndef _LIB_TFEL_THERMALBEHAVIOURDATA_H_
#define _LIB_TFEL_THERMALBEHAVIOURDATA_H_ 

#include<vector>
#include<algorithm>
#include<iostream>

#include "Config/TFELTypes.hxx"
#include "Metaprogramming/StaticAssert.hxx"
#include "Material/MaterialException.hxx"

namespace tfel{
  
  namespace material {

    /*!
     * \class ThermalBehaviourData
     * \param unsigned short N, space dimension
     * \param typename NumType, numerical type
     * \param bool use_qt, says if one uses quantities
     * \author Helfer Thomas
     * \date   09 Aug 2006
     */    
    template<unsigned short N, typename NumType, bool use_qt>
    class ThermalBehaviourData{

      typedef tfel::config::Types<N,NumType,use_qt> Types;

      typedef typename Types::HeatFluxVector HeatFluxVector;
      typedef typename Types::HeatFluxVectorDerivative HeatFluxVectorDerivative;
      typedef typename Types::ThermalConductivityMatrix ThermalConductivityMatrix;

    public:

      HeatFluxVector j;
      HeatFluxVectorDerivative dj_dT;
      ThermalConductivityMatrix k;

    };

    template<typename Behaviour>
    struct CheckThermalDataValidity
    {};


  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_THERMALBEHAVIOURDATA_H */

