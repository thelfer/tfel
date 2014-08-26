/*!
 * \file   MechanicalVariableDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<stdexcept>

#include"MFront/MechanicalVariableDescription.hxx"

namespace mfront{

  MechanicalVariableDescription::MechanicalVariableDescription(const std::string& type_,
							       const std::string& name_,
							       const unsigned short arraySize_,
							       const unsigned short lineNumber_,
							       const tfel::material::ModellingHypothesis::Hypothesis h)
    : VariableDescription(type_,name_,arraySize_,lineNumber_),
      hypothesis(h)
  {} // end of MechanicalVariableDescription::MechanicalVariableDescription

} // end of namespace mfront
