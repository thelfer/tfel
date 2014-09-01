/*!
 * \file   mfront/src/StaticVariableDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   19 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include<stdexcept>

#include"MFront/StaticVariableDescription.hxx"

namespace mfront{

  StaticVariableDescription::StaticVariableDescription(const std::string& type_,
						       const std::string& name_,
						       const unsigned int line_,
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

  const StaticVariableDescription&
  StaticVariableDescriptionContainer::get(const std::string& n) const
  {
    using namespace std;
    StaticVariableDescriptionContainer::const_iterator p;
    for(p=this->begin();p!=this->end();++p){
      if(p->name==n){
	return *p;
      }
    }
    string msg("StaticVariableDescriptionContainer::get : "
	       "no static variable named '"+n+"'");
    throw(runtime_error(msg));
    return *(static_cast<StaticVariableDescription*>(0));
  } // end of StaticVariableDescriptionContainer::get

} // end of namespace mfront
