/*! 
 * \file  mfront/mtest/MTestBehaviour.cxx
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
#include"MFront/MTestBehaviour.hxx"
#include"MFront/MTestEvolution.hxx"

namespace mfront
{

  void
  MTestBehaviour::setOptionalMaterialPropertyDefaultValue(MTestEvolutionManager& mp,
							  const MTestEvolutionManager& evm,
							  const std::string& n,
							  const real v)
  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    typedef MTestEvolutionManager::value_type MVType;
    if(evm.find(n)==evm.end()){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	ostream& log = getLogStream();
	log << "MTestStandardUmatBehaviour::setOptionalMaterialPropertiesDefaultValues : "
	    << "set material property '" << n << "' to default value" << endl;
      }
      if(!mp.insert(MVType(n,shared_ptr<MTestEvolution>(new MTestConstantEvolution(v)))).second){
	string msg("MTestStandardUmatBehaviour::setOptionalMaterialPropertiesDefaultValues : "
		   "default value for material property '"+n+"' already declared");
	throw(runtime_error(msg));
      }
    }
  } // end of MTestBehaviour::setOptionalMaterialPropertyDefaultValue


  MTestBehaviour::~MTestBehaviour()
  {} // end of MTestBehaviour::~MTestBehaviour

} // end of namespace mfront

