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

  bool
  StaticVariableDescriptionContainer::contains(const std::string& n) const
  {
    StaticVariableDescriptionContainer::const_iterator p;
    for(p=this->begin();p!=this->end();++p){
      if(p->name==n){
	return true;
      }
    }
    return false;
  } // end of StaticVariableDescriptionContainer::contains

} // end of namespace mfront
