/*! 
 * \file  mfront/mtest/Behaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/Evolution.hxx"

namespace mtest
{

  void
  Behaviour::setOptionalMaterialPropertyDefaultValue(EvolutionManager& mp,
						     const EvolutionManager& evm,
						     const std::string& n,
						     const real v)
  {
    if(evm.find(n)==evm.end()){
      if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL2){
	auto& log = mfront::getLogStream();
	log << "MTestStandardUmatBehaviour::setOptionalMaterialPropertiesDefaultValues : "
	    << "set material property '" << n << "' to default value\n";
      }
      if(!mp.insert({n,make_evolution(v)}).second){
	throw(std::runtime_error("MTestStandardUmatBehaviour::setOptionalMaterialPropertiesDefaultValues: "
				 "default value for material property '"+n+"' already declared"));
      }
    }
  } // end of Behaviour::setOptionalMaterialPropertyDefaultValue

  Behaviour::~Behaviour() = default;

} // end of namespace mtest

