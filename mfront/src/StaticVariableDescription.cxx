/*!
 * \file   StaticVariableDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   19 Jan 2007
 */

#include"MFront/StaticVariableDescription.hxx"

namespace mfront{

  StaticVariableDescription::StaticVariableDescription(const std::string& type_,
						       const std::string& name_,
						       const unsigned short line_,
						       const StaticVariableDescription::StaticVariableValueType value_)
    : VariableDescription(type_,name_,1u,line_),
      value(value_)
  {}

} // end of namespace mfront
