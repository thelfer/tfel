/*!
 * \file   VariableDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<sstream>
#include<stdexcept>

#include"MFront/VariableDescription.hxx"

namespace mfront{

  VariableDescription::VariableDescription(const std::string& t,
					   const std::string& n,
					   const unsigned short s,
					   const unsigned short l)
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
  VariableDescription::getGlossaryName(const std::map<std::string,std::string>& glossaryNames,
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
  } // end of MFrontUMATInterfaceBase::getGlossaryName

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
  VariableDescriptionContainer::getGlossaryNames(const std::map<std::string,std::string>& glossaryNames,
						 const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    vector<string> n;
    this->appendGlossaryNames(n,glossaryNames,entryNames);
    return n;
  }

  void
  VariableDescriptionContainer::getGlossaryNames(std::vector<std::string>& n,
						 const std::map<std::string,std::string>& glossaryNames,
						 const std::map<std::string,std::string>& entryNames) const
  {
    n.clear();
    this->appendGlossaryNames(n,glossaryNames,entryNames);
  }

  void
  VariableDescriptionContainer::appendGlossaryNames(std::vector<std::string>& n,
						    const std::map<std::string,std::string>& glossaryNames,
						    const std::map<std::string,std::string>& entryNames) const
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for(p=this->begin();p!=this->end();++p){
      const string name = p->getGlossaryName(glossaryNames,entryNames);
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
  } // end of VariableDescriptionContainer::appendGlossaryNames

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


} // end of namespace mfront
