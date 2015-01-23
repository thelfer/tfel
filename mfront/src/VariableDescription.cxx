/*!
 * \file   mfront/src/VariableDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"MFront/VariableDescription.hxx"

namespace mfront{

  VariableDescription::VariableDescription(const std::string& t,
					   const std::string& n,
					   const unsigned short s,
					   const unsigned int l)
    : type(t),
      name(n),
      arraySize(s),
      lineNumber(l)
  {
    using namespace std;
    if(this->arraySize==0){
      string msg("VariableDescription::VariableDescription : ");
      msg += "invalid array size";
      throw(runtime_error(msg));
    }
  } // end of VariableDescription::VariableDescription

  const std::string&
  VariableDescription::getExternalName(const std::map<std::string,std::string>& glossaryNames,
				       const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    map<string,string>::const_iterator p2;
    map<string,string>::const_iterator p3;
    if((p2=glossaryNames.find(this->name))!=glossaryNames.end()){
      return p2->second;
    } else if((p3=entryNames.find(this->name))!=entryNames.end()){
      return p3->second;
    }
    return this->name;
  } // end of VariableDescription::getExternalName

  VariableDescription::~VariableDescription()
  {} // end of VariableDescription::~VariableDescription

  bool
  VariableDescriptionContainer::contains(const std::string& n) const
  {
    VariableDescriptionContainer::const_iterator p;
    for(p=this->begin();p!=this->end();++p){
      if(p->name==n){
	return true;
      }
    }
    return false;
  } // end of VariableDescriptionContainer::contains

  std::vector<std::string>
  VariableDescriptionContainer::getExternalNames(const std::map<std::string,std::string>& glossaryNames,
						 const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    vector<string> n;
    this->appendExternalNames(n,glossaryNames,entryNames);
    return n;
  }

  void
  VariableDescriptionContainer::getExternalNames(std::vector<std::string>& n,
						 const std::map<std::string,std::string>& glossaryNames,
						 const std::map<std::string,std::string>& entryNames) const
  {
    n.clear();
    this->appendExternalNames(n,glossaryNames,entryNames);
  }

  void
  VariableDescriptionContainer::appendExternalNames(std::vector<std::string>& n,
						    const std::map<std::string,std::string>& glossaryNames,
						    const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for(p=this->begin();p!=this->end();++p){
      const string name = p->getExternalName(glossaryNames,entryNames);
      if(p->arraySize==1u){
	n.push_back(name);
      } else {
	for(unsigned short i=0;i!=p->arraySize;++i){
	  ostringstream nb;
	  nb << '[' << i << ']';
	  n.push_back(name+nb.str());
	}
      }
    }
  } // end of VariableDescriptionContainer::appendExternalNames

  const VariableDescription&
  VariableDescriptionContainer::getVariable(const std::string& n)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for(p=this->begin();p!=this->end();++p){
      if(p->name==n){
	return *p;
      }
    }
    string msg("VariableDescriptionContainer::getVariable : "
	       "no variable named '"+n+"'");
    throw(runtime_error(msg));
    return *(this->end());
  }

  VariableDescriptionContainer::~VariableDescriptionContainer()
  {} // end of VariableDescriptionContainer::~VariableDescriptionContainer

} // end of namespace mfront
