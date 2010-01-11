/*!
 * \file   MechanicalBehaviourData.hxx
 * \brief  This file implements the MechanicalBehaviourData Class
 * \author Helfer Thomas
 * \date   09 Aug 2006
 */

#ifndef _LIB_TFEL_MECHANICALBEHAVIOURDATA_H_
#define _LIB_TFEL_MECHANICALBEHAVIOURDATA_H_ 

#include<vector>
#include<algorithm>
#include<iostream>

#include "Config/TFELTypes.hxx"
#include "Metaprogramming/StaticAssert.hxx"
#include "Material/MechanicalBehaviourTraits.hxx"
#include "Material/MaterialException.hxx"

namespace tfel
{
  
  namespace material
  {

    /*!
     * \class MechanicalBehaviourData
     * \param unsigned short N, space dimension
     * \param typename NumType, numerical type
     * \param bool use_qt, says if one uses quantities
     * \author Helfer Thomas
     * \date   09 Aug 2006
     */    
    template<unsigned short N, typename NumType, bool use_qt>
    class MechanicalBehaviourData
    {

      typedef tfel::config::Types<N,NumType,use_qt> Types;

      typedef typename Types::real            real; 
      typedef typename Types::temperature     temperature; 
      typedef typename Types::StrainStensor   StrainStensor; 
      typedef typename Types::StressStensor   StressStensor; 
      typedef typename Types::StiffnessTensor StiffnessTensor; 

      MechanicalBehaviourData();

    public:
      
      StiffnessTensor D;
      StressStensor sig;
      StrainStensor eto;
      std::vector<real> material_properties;
      std::vector<real> internal_variables;
      std::vector<real> external_variables;
      temperature T;
      
      MechanicalBehaviourData(const StressStensor& sig_,
			      const StrainStensor& eto_,
			      const StiffnessTensor& D_,
			      const std::vector<real>& mat_,
			      const std::vector<real>& int_var_,
			      const std::vector<real>& ext_var_,
			      const double T_)
	: D(D_), sig(sig_), eto(eto_), 
	  material_properties(mat_),
	  internal_variables(int_var_),
	  external_variables(ext_var_),
	  T(T_)
      {}

      ~MechanicalBehaviourData()
      {}

    }; // end of class MechanicalBehaviourData

    template<typename Behaviour>
    struct CheckMechanicalDataValidity
    {
      typedef MechanicalBehaviourTraits<Behaviour> Traits;
      static const unsigned short DIME = Traits::dimension;      
      static const unsigned short mat_size = Traits::material_properties_nb; 
      static const unsigned short ext_size = Traits::external_variables_nb;   
      static const unsigned short int_size = Traits::internal_variables_nb;   

      template<unsigned short N, typename NumType, bool use_qt>
      static void exe(const MechanicalBehaviourData<N,NumType,use_qt>& data)
      {
	using namespace std;
	TFEL_STATIC_ASSERT(DIME==N);
	if(data.material_properties.size()!=mat_size){
	  string msg ("Datas' material properties size");
	  msg += " does not match the behaviour's size";
	  throw(MaterialException(msg));
	}
	if(data.external_variables.size()!=ext_size){
	  string msg("Datas' external variables size");
	  msg +=" does not match the behaviour's size";
	  throw(MaterialException(msg));
	}
	if(data.dexternal_variables.size()!=ext_size){
	  string msg("Datas' dexternal variables size");
	  msg+=" does not match the behaviour's size";
	  throw(MaterialException(msg));
	}
	if(data.internal_variables.size()!=int_size){
	  string msg("Datas' internal variables size");
	  msg+=" does not match the behaviour's size";
	  throw(MaterialException(msg));
	}
      }

    }; // end of struct CheckMechanicalDataValidity

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MECHANICALBEHAVIOURDATA_H */

