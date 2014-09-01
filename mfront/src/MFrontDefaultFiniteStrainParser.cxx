/*!
 * \file   mfront/src/MFrontDefaultFiniteStrainParser.cxx
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

#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontDefaultFiniteStrainParser.hxx"

namespace mfront{

  MFrontDefaultFiniteStrainParser::MFrontDefaultFiniteStrainParser()
    : MFrontDefaultParserBase()
  {
    this->registerVariable("F0",false);
    this->registerVariable("F1",false);
    this->registerVariable("sig",false);
    this->mb.declareAsAFiniteStrainStandardBehaviour();
  }

  std::string
  MFrontDefaultFiniteStrainParser::getDescription(void)
  {
    return "this parser is the most generic one as it does not make any restriction "
           "on the behaviour or the integration method that may be used";
  } // end of MFrontDefaultFiniteStrainParser::getDescription
  
  std::string
  MFrontDefaultFiniteStrainParser::getName(void)
  {
    return "DefaultFiniteStrainParser";
  }

  void MFrontDefaultFiniteStrainParser::writeBehaviourParserSpecificIncludes(void)
  {
    MFrontDefaultParserBase::writeBehaviourParserSpecificIncludes();
    this->behaviourFile << "#include\"TFEL/Math/tensor.hxx\"\n";
  } // end of MFrontDefaultFiniteStrainParser::writeBehaviourParserSpecificIncludes

  MFrontDefaultFiniteStrainParser::~MFrontDefaultFiniteStrainParser()
  {} // end of MFrontDefaultFiniteStrainParser::~MFrontDefaultFiniteStrainParser

} // end of namespace mfront  
