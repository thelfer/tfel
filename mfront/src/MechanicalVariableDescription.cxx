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

  MechanicalVariableDescription::MechanicalVariableDescription(const std::string&   t,
							       const std::string&   n,
							       const unsigned short s,
							       const unsigned short l,
							       const tfel::material::ModellingHypothesis::Hypothesis h)
    : VariableDescription(t,n,s,l),
      hypothesis(h)
  {} // end of MechanicalVariableDescription::MechanicalVariableDescription

  MechanicalVariableDescription::MechanicalVariableDescription(const VariableDescription& v,
							       const tfel::material::ModellingHypothesis::Hypothesis h)
    : VariableDescription(v),
      hypothesis(h)
  {} // end of MechanicalVariableDescription::MechanicalVariableDescription

} // end of namespace mfront
