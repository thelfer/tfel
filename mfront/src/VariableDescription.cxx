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

#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"MFront/SupportedTypes.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/VariableDescription.hxx"

namespace mfront{

  const std::string VariableDescription::depth("depth");
  const std::string VariableDescription::initialValue("initialValue");
  const std::string VariableDescription::defaultValue("defaultValue");
  
  VariableDescription::VariableDescription() = default;

  VariableDescription::VariableDescription(const std::string& t,
					   const std::string& n,
					   const unsigned short s,
					   const size_t l)
    : VariableDescriptionBase(t,n,s,l)
  {} // end of VariableDescription::VariableDescription

  VariableDescription::VariableDescription(const VariableDescription&) = default;
  VariableDescription::VariableDescription(VariableDescription&&) = default;
  VariableDescription&
  VariableDescription::operator=(VariableDescription&&) = default;
  VariableDescription&
  VariableDescription::operator=(const VariableDescription&) = default;

  void VariableDescription::setGlossaryName(const std::string& g)
  {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("VariableDescription::setGlossaryName: "+m));}
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(!glossary.contains(g),"'"+g+"' is not a glossary name");
    throw_if(this->hasGlossaryName(),"the glossary name has already been defined "
	     "for variable '"+this->name+"'");
    throw_if(this->hasEntryName(),"an entry name has already been defined "
	     "for variable '"+this->name+"'");
    this->glossaryName = glossary.getGlossaryEntry(g).getKey();
  } // end of VariableDescription::setGlossaryName

  void VariableDescription::setEntryName(const std::string& e)
  {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("VariableDescription::setEntryName: "+m));}
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(glossary.contains(e),"'"+e+"' is a glossary name");
    throw_if(this->hasGlossaryName(),"the glossary name has already been defined "
	     "for variable '"+this->name+"'");
    throw_if(this->hasEntryName(),"an entry name has already been defined "
	     "for variable '"+this->name+"'");
    this->entryName = e;    
  } // end of VariableDescription::setEntryName
  
  bool VariableDescription::hasGlossaryName() const{
    return this->glossaryName.is<std::string>();
  }

  bool VariableDescription::hasEntryName() const{
    return this->entryName.is<std::string>();
  }
  
  const std::string& VariableDescription::getExternalName() const
  {
    if(this->hasGlossaryName()){
      return this->glossaryName.get<std::string>();
    }
    if(this->hasEntryName()){
      return this->entryName.get<std::string>();
    }
    return this->name;
  } // end of VariableDescription::getExternalName

  void VariableDescription::throwUndefinedAttribute(const std::string& n)
  {
    throw(std::runtime_error("VariableDescription::getAttribute : "
			     "no attribute named '"+n+"'"));
  } // end of VariableDescription::throwUndefinedAttribute

  void VariableDescription::setAttribute(const std::string& n,
					 const VariableAttribute& a,
					 const bool b)
  {
    auto throw_if = [](const bool c, const std::string& m){
      if(c){throw(std::runtime_error("VariableDescription::setAttribute: "+m));}
    };    
    if(b){
      auto p=this->attributes.find(n);
      if(p!=this->attributes.end()){
	throw_if(a.getTypeIndex()!=p->second.getTypeIndex(),
		 "attribute already exists with a different type");
	return;
      }
    }
    throw_if(!this->attributes.insert({n,a}).second,
	     "attribute '"+n+"' already declared");
  } // end of VariableDescription::setAttribute

  bool VariableDescription::hasAttribute(const std::string& n) const
  {
    return this->attributes.count(n)!=0u;
  } // end of VariableDescription::hasAttribute

  const std::map<std::string,VariableAttribute>&
  VariableDescription::getAttributes() const
  {
    return this->attributes;
  } // end of VariableDescription::getAttributes

  bool VariableDescription::hasBounds() const
  {
    return this->bounds.is<VariableBoundsDescription>();
  } // end of VariableDescription::hasBounds

  static void checkBoundsCompatibility(const VariableDescription& v,
				       const VariableBoundsDescription& b){
    auto throw_if = [](const bool c,const std::string& m){
      if(c){throw(std::runtime_error("checkBoundsCompatibility: "+m));}
    };    
    if((b.component!=-1)&&(b.component!=0)){
      const auto f = SupportedTypes::getTypeFlag(v.type);
      throw_if(f!=SupportedTypes::Scalar,
	       "invalid component value for a scalar "
	       "("+std::to_string(b.component)+")");
    }
    if(b.boundsType==VariableBoundsDescription::LOWERANDUPPER){
      throw_if(b.lowerBound>b.upperBound,
	       "invalid bounds value");
    }
  } // end of checkBoundsCompatibility
  
  void VariableDescription::setBounds(const VariableBoundsDescription& b)
  {
    if(this->hasBounds()){
      throw(std::runtime_error("VariableDescription::setBounds: "
			       "bounds have already been set on variable "
			       "'"+this->name+"'"));
    }
    checkBoundsCompatibility(*this,b);
    this->bounds = b;
  } // end of VariableDescription::setBounds

  const VariableBoundsDescription& VariableDescription::getBounds() const
  {
    if(!this->hasBounds()){
      throw(std::runtime_error("VariableDescription::setBounds: "
			       "no bounds set on variable "
			       "'"+this->name+"'"));
    }
    return this->bounds.get<VariableBoundsDescription>();
  } // end of VariableDescription::getBounds

  bool VariableDescription::hasPhysicalBounds() const
  {
    return this->physicalBounds.is<VariableBoundsDescription>();
  } // end of VariableDescription::hasPhysicalBounds
  
  void VariableDescription::setPhysicalBounds(const VariableBoundsDescription& b)
  {
    if(this->hasPhysicalBounds()){
      throw(std::runtime_error("VariableDescription::setPhysicalBounds: "
			       "bounds have already been set on variable "
			       "'"+this->name+"'"));
    }
    checkBoundsCompatibility(*this,b);
    this->physicalBounds = b;
  } // end of VariableDescription::setPhysicalBounds

  const VariableBoundsDescription& VariableDescription::getPhysicalBounds() const
  {
    if(!this->hasPhysicalBounds()){
      throw(std::runtime_error("VariableDescription::setPhysicalBounds: "
			       "no bounds set on variable "
			       "'"+this->name+"'"));
    }
    return this->physicalBounds.get<VariableBoundsDescription>();
  } // end of VariableDescription::getPhysicalBounds
  
  VariableDescription::~VariableDescription() = default;

  bool hasBounds(const VariableDescription& v){
    return v.hasBounds();
  } // end of hasBounds

  bool hasPhysicalBounds(const VariableDescription& v){
    return v.hasPhysicalBounds();
  } // end of hasPhysicalBounds
  
  VariableDescriptionContainer::VariableDescriptionContainer() = default;
  
  VariableDescriptionContainer::VariableDescriptionContainer(const std::initializer_list<VariableDescription>& l)
	  : std::vector<VariableDescription>(l)
  {}

  VariableDescriptionContainer::VariableDescriptionContainer(VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer::VariableDescriptionContainer(const VariableDescriptionContainer&) = default;
  VariableDescriptionContainer&
  VariableDescriptionContainer::operator=(VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer&
  VariableDescriptionContainer::operator=(const VariableDescriptionContainer&) = default;

  bool VariableDescriptionContainer::contains(const std::string& n) const
  {
    for(const auto& v : *this){
      if(v.name==n){
	return true;
      }
    }
    return false;
  } // end of VariableDescriptionContainer::contains

  std::vector<std::string> VariableDescriptionContainer::getExternalNames() const
  {
    std::vector<std::string> n;
    this->appendExternalNames(n);
    return n;
  }

  void VariableDescriptionContainer::getExternalNames(std::vector<std::string>& n) const
  {
    n.clear();
    this->appendExternalNames(n);
  }

  void VariableDescriptionContainer::appendExternalNames(std::vector<std::string>& n) const
  {
    for(const auto& v : *this){
      const auto name = v.getExternalName();
      if(v.arraySize==1u){
	n.push_back(name);
      } else {
	for(unsigned short i=0;i!=v.arraySize;++i){
	  std::ostringstream nb;
	  nb << '[' << i << ']';
	  n.push_back(name+nb.str());
	}
      }
    }
  } // end of VariableDescriptionContainer::appendExternalNames

  VariableDescription&
  VariableDescriptionContainer::getVariable(const std::string& n)
  {
    for(auto& v : *this){
      if(v.name==n){
	return v;
      }
    }
    throw(std::runtime_error("VariableDescriptionContainer::getVariable : "
			     "no variable named '"+n+"'"));
  }

  const VariableDescription&
  VariableDescriptionContainer::getVariableByExternalName(const std::string& n) const
  {
    for(auto& v : *this){
      if(v.getExternalName()==n){
	return v;
      }
    }
    throw(std::runtime_error("VariableDescriptionContainer::getVariableByExternalName : "
			     "no variable with external name '"+n+"'"));
  } // end of VariableDescriptionContainer::getVariableByExternalName
  
  const VariableDescription&
  VariableDescriptionContainer::getVariable(const std::string& n) const
  {
    for(const auto& v : *this){
      if(v.name==n){
	return v;
      }
    }
    throw(std::runtime_error("VariableDescriptionContainer::getVariable : "
			     "no variable named '"+n+"'"));
  }

  bool hasBounds(const VariableDescriptionContainer& c)
  {
    for(const auto& v:c){
      if(v.hasBounds()){
	return true;
      }
    }
    return false;
  } // end of hasBounds

  bool hasPhysicalBounds(const VariableDescriptionContainer& c)
  {
    for(const auto& v:c){
      if(v.hasPhysicalBounds()){
	return true;
      }
    }
    return false;
  } // end of hasPhysicalBounds
  
  VariableDescriptionContainer::~VariableDescriptionContainer() = default;

} // end of namespace mfront
