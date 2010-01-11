/*!
 * \file   MechanicalIntegrationData.hxx
 * \brief  This file implements the MechanicalIntegrationData Class
 * \author Helfer Thomas
 * \date   09 Aug 2006
 */

#ifndef _LIB_TFEL_MECHANICALINTEGRATIONDATA_H_
#define _LIB_TFEL_MECHANICALINTEGRATIONDATA_H_ 

#include<vector>
#include<algorithm>
#include<iostream>

#include "Config/TFELTypes.hxx"
#include "Metaprogramming/StaticAssert.hxx"
#include "Material/MechanicalBehaviourTraits.hxx"
#include "Material/MaterialException.hxx"

namespace tfel{
  
  namespace material {

    /*!
     * \class MechanicalIntegrationData
     * \param unsigned short N, space dimension
     * \param typename NumType, numerical type
     * \param bool use_qt, says if one uses quantities
     * \author Helfer Thomas
     * \date   09 Aug 2006
     */    
    template<unsigned short N, typename NumType, bool use_qt>
    class MechanicalIntegrationData{

      typedef tfel::config::Types<N,NumType,use_qt> Types;

      typedef typename Types::real            real; 
      typedef typename Types::temperature     temperature; 
      typedef typename Types::StrainStensor   StrainStensor; 
      typedef typename Types::StressStensor   StressStensor; 
      typedef typename Types::StiffnessTensor StiffnessTensor; 

      MechanicalIntegrationData();

    public:
      
      StrainStensor     deto;
      std::vector<real> dexternal_variables;
      real dt;
      temperature dT;
      
      MechanicalIntegrationData(StrainStensor& deto_,
				const std::vector<real>& dext_var_,
				const double dt_,
				const double dT_)
	: deto(deto_), 
	  dexternal_variables(dext_var_),
	  dt(dt_), 
	  dT(dT_)
      {}

      ~MechanicalIntegrationData()
      {}

    };

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MECHANICALINTEGRATIONDATA_H */

