/*!
 * \file   mfront/src/DefaultFiniteStrainDSL.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include<fstream>
#include<stdexcept>

#include"MFront/AbstractBehaviourInterface.hxx"
#include"MFront/BehaviourInterfaceFactory.hxx"
#include"MFront/DefaultFiniteStrainDSL.hxx"

namespace mfront{

  DefaultFiniteStrainDSL::DefaultFiniteStrainDSL()
    : DefaultDSLBase()
  {
    this->mb.setParserName("DefaultFiniteStrain");
    this->mb.declareAsAFiniteStrainStandardBehaviour();
  }

  std::string
  DefaultFiniteStrainDSL::getDescription(void)
  {
    return "this parser is the most generic one as it does not make any restriction "
           "on the behaviour or the integration method that may be used";
  } // end of DefaultFiniteStrainDSL::getDescription
  
  std::string
  DefaultFiniteStrainDSL::getName(void)
  {
    return "DefaultFiniteStrainDSL";
  }

  void DefaultFiniteStrainDSL::writeBehaviourParserSpecificIncludes(void)
  {
    DefaultDSLBase::writeBehaviourParserSpecificIncludes();
    this->behaviourFile << "#include\"TFEL/Math/tensor.hxx\"\n";
  } // end of DefaultFiniteStrainDSL::writeBehaviourParserSpecificIncludes

  DefaultFiniteStrainDSL::~DefaultFiniteStrainDSL()
  {} // end of DefaultFiniteStrainDSL::~DefaultFiniteStrainDSL

} // end of namespace mfront  
