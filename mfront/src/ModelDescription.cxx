/*! 
 * \file  ModelDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 janv. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Glossary/Glossary.hxx"
#include"MFront/ModelDescription.hxx"

namespace mfront
{

  ModelDescription::Function::Function() = default;
  ModelDescription::Function::Function(const ModelDescription::Function&) = default;
  ModelDescription::Function::Function(ModelDescription::Function&&) = default;
  ModelDescription::Function&
  ModelDescription::Function::operator=(const ModelDescription::Function&) = default;
  ModelDescription::Function&
  ModelDescription::Function::operator=(ModelDescription::Function&&) = default;
  ModelDescription::Function::~Function() = default;

  ModelDescription::ModelDescription() = default;
  ModelDescription::ModelDescription(const ModelDescription&) = default;
  ModelDescription::ModelDescription(ModelDescription&&) = default;
  ModelDescription&
  ModelDescription::operator=(const ModelDescription&) = default;
  ModelDescription&
  ModelDescription::operator=(ModelDescription&&) = default;

  void
  ModelDescription::reserveName(const std::string& n){
    if(!this->reservedNames.insert(n).second){
      throw(std::runtime_error("ModelDescription::reserveName: "
			       "name '"+n+"' already reserved"));
    }
  }
      
  void
  ModelDescription::registerMemberName(const std::string& n)
  {
    this->reserveName(n);
    if(!this->memberNames.insert(n).second){
      throw(std::runtime_error("ModelDescription::registerMemberName: "
			       "name '"+n+"' already reserved"));
    }
  } // end of ModelDescription::registerMemberName

  void
  ModelDescription::registerStaticMemberName(const std::string& n)
  {
    this->reserveName(n);
    if(!this->staticMemberNames.insert(n).second){
      throw(std::runtime_error("ModelDescription::registerStaticMemberName: "
			       "name '"+n+"' already reserved"));
    }
  } // end of ModelDescription::registerStaticMemberName
  
  const std::string&
  ModelDescription::getExternalName(const std::string& n) const{
    this->checkVariableExistence(n);
    auto p=this->glossaryNames.find(n);
    if(p!=this->glossaryNames.end()){
      return p->second;
    }
    p=this->entryNames.find(n);
    if(p!=this->entryNames.end()){
      return p->second;
    }
    return n;
  } // end of ModelDescription::getExternalName
  
  void
  ModelDescription::checkVariableExistence(const std::string& v) const{
    if((!this->inputs.contains(v))&&
       (!this->outputs.contains(v))&&
       (!this->parameters.contains(v))&&
       (!this->staticVars.contains(v))&&
       (!this->constantMaterialProperties.contains(v))){
      throw(std::runtime_error("ModelDescription::checkVariableExistence: "
			       "no variable named '"+v+"'"));
    }
  } // end of ModelDescription::checkVariableExistence

  void ModelDescription::setGlossaryName(const std::string& v,
					 const std::string& g){
    this->checkVariableExistence(v);
    if(!tfel::glossary::Glossary::getGlossary().contains(g)){
      throw(std::runtime_error("ModelDescription::setGlossaryName: "
			       "no glossary name '"+g+"'"));
    }
    if((this->glossaryNames.find(v)!=this->glossaryNames.end())||
       (this->entryNames.find(v)!=this->entryNames.end())){
      throw(std::runtime_error("ModelDescription::setGlossaryName: "
			       "an external name has already been set "
			       "for variable '"+v+"'"));
    }
    this->glossaryNames.insert({v,g});
  }

  void ModelDescription::setEntryName(const std::string& v,
				      const std::string& e){
    this->checkVariableExistence(v);
    if(tfel::glossary::Glossary::getGlossary().contains(e)){
      throw(std::runtime_error("ModelDescription::setEntryName: "
			       "'"+e+"' is a glossary name"));
    }
    if((this->glossaryNames.find(v)!=this->glossaryNames.end())||
       (this->entryNames.find(v)!=this->entryNames.end())){
      throw(std::runtime_error("ModelDescription::setEntryName: "
			       "an external name has already been set "
			       "for variable '"+v+"'"));
    }
    this->reserveName(e);
    this->entryNames.insert({v,e});
  }

  std::set<std::string>&
  ModelDescription::getReservedNames(void){
    return this->reservedNames;
  }
  
  const std::set<std::string>&
  ModelDescription::getReservedNames(void) const{
    return this->reservedNames;
  }
  
  ModelDescription::~ModelDescription() = default;
  
} // end of namespace mfront
