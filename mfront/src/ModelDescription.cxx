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

#include<algorithm>
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
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

  std::pair<std::string,unsigned short>
  ModelDescription::decomposeVariableName(const std::string& n) const
  {
    auto get = [&n](const VariableDescriptionContainer& vc)
      -> std::pair<std::string,unsigned short>
    {
      using size_type = unsigned short;
      for(const auto& v:vc){
	if(v.name==n){
	  return {v.name,0u};
	}
	const auto d = v.getAttribute<size_type>(VariableDescription::depth,0);
	for(size_type j=1;j<=d;++j){
	  auto fn = v.name + "_" + std::to_string(j);
	  if(fn==n){
	    return {v.name,j};
	  }
	}
      }
      return {};
    };
    auto r = get(this->outputs);
    if(!r.first.empty()){
      return r;
    }
    r = get(this->inputs);
    if(r.first.empty()){
      throw(std::runtime_error("decomposeVariableName: "
			       "field name '"+n+"' has not been found"));
    }
    return r;
  } // end of ModelDescription::decomposeVariableName

  ModelDescription::ModelDescription() = default;
  ModelDescription::ModelDescription(const ModelDescription&) = default;
  ModelDescription::ModelDescription(ModelDescription&&) = default;
  ModelDescription&
  ModelDescription::operator=(const ModelDescription&) = default;
  ModelDescription&
  ModelDescription::operator=(ModelDescription&&) = default;

  void ModelDescription::reserveName(const std::string& n){
    if(!this->reservedNames.insert(n).second){
      throw(std::runtime_error("ModelDescription::reserveName: "
			       "name '"+n+"' already reserved"));
    }
  }

  bool ModelDescription::isNameReserved(const std::string& n) const{
    return this->reservedNames.count(n)!=0;
  }
  
  void ModelDescription::registerMemberName(const std::string& n)
  {
    this->reserveName(n);
    if(!this->memberNames.insert(n).second){
      throw(std::runtime_error("ModelDescription::registerMemberName: "
			       "name '"+n+"' already reserved"));
    }
  } // end of ModelDescription::registerMemberName

  void ModelDescription::registerStaticMemberName(const std::string& n)
  {
    this->reserveName(n);
    if(!this->staticMemberNames.insert(n).second){
      throw(std::runtime_error("ModelDescription::registerStaticMemberName: "
			       "name '"+n+"' already reserved"));
    }
  } // end of ModelDescription::registerStaticMemberName
  
  VariableDescription&
  ModelDescription::getVariableDescription(const std::string& n)
  {
    if(this->outputs.contains(n)){
      return this->outputs.getVariable(n);
    }
    if(this->inputs.contains(n)){
      return this->inputs.getVariable(n);
    }
    if(this->parameters.contains(n)){
      return this->parameters.getVariable(n);
    }
    if(this->constantMaterialProperties.contains(n)){
      return this->constantMaterialProperties.getVariable(n);
    }
    throw(std::runtime_error("ModelDescription::getVariableDescription: "
			     "No variable named '"+n+"'.\n"
			     "'"+n+"' is neither:\n"
			     "- An output.\n"
			     "- An input.\n"
			     "- A parameter.\n"
			     "- A constant material properties."));
  } // end of ModelDescription::getVariableDescription

  const VariableDescription&
  ModelDescription::getVariableDescription(const std::string& n) const
  {
    if(this->outputs.contains(n)){
      return this->outputs.getVariable(n);
    }
    if(this->inputs.contains(n)){
      return this->inputs.getVariable(n);
    }
    if(this->parameters.contains(n)){
      return this->parameters.getVariable(n);
    }
    if(this->constantMaterialProperties.contains(n)){
      return this->constantMaterialProperties.getVariable(n);
    }
    throw(std::runtime_error("ModelDescription::getVariableDescription: "
			     "No variable named '"+n+"'.\n"
			     "'"+n+"' is neither:\n"
			     "- An output.\n"
			     "- An input.\n"
			     "- A parameter.\n"
			     "- A constant material properties."));
  } // end of ModelDescription::getVariableDescription
  
  void ModelDescription::checkVariableExistence(const std::string& v) const{
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
    const auto& glossary = tfel::glossary::Glossary::getGlossary();
    if(!glossary.contains(g)){
      throw(std::runtime_error("ModelDescription::setGlossaryName: "
			       "no glossary name '"+g+"'"));
    }
    if((this->glossaryNames.find(v)!=this->glossaryNames.end())||
       (this->entryNames.find(v)!=this->entryNames.end())){
      throw(std::runtime_error("ModelDescription::setGlossaryName: "
			       "an external name has already been set "
			       "for variable '"+v+"'"));
    }
    if(v!=g){
      this->reserveName(g);
    }
    const auto gn = glossary.getGlossaryEntry(g).getKey();
    this->getVariableDescription(v).setGlossaryName(g);
    this->glossaryNames.insert({v,gn});
  }

  void ModelDescription::setEntryName(const std::string& v,
				      const std::string& e){
    this->checkVariableExistence(v);
    if(!tfel::utilities::CxxTokenizer::isValidIdentifier(e,false)){
      throw(std::runtime_error("ModelDescription::setEntryName: "
			       "'"+e+"' is a not a valid entry name"));
    }
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
    if(v!=e){
      this->reserveName(e);
    }
    this->getVariableDescription(v).setEntryName(e);
    this->entryNames.insert({v,e});
  }

  std::set<std::string>&
  ModelDescription::getReservedNames(){
    return this->reservedNames;
  }
  
  const std::set<std::string>&
  ModelDescription::getReservedNames() const{
    return this->reservedNames;
  }

  void ModelDescription::addMaterialLaw(const std::string& m)
  {
    if(std::find(this->materialLaws.begin(),
		 this->materialLaws.end(),m)==this->materialLaws.end()){
      this->materialLaws.push_back(m);
    }
  } // end of ModelDescription::addMaterialLaw

  void ModelDescription::appendToIncludes(const std::string& c)
  {
    this->includes+=c;
    if(!this->includes.empty()){
      if(*(this->includes.rbegin())!='\n'){
	this->includes+='\n';
      }
    }
  } // end of ModelDescription::appendToIncludes

  void ModelDescription::appendToMembers(const std::string& c)
  {
    this->members+=c;
    if(!this->members.empty()){
      if(*(this->members.rbegin())!='\n'){
	this->members+='\n';
      }
    }
  } // end of ModelDescription::appendToMembers

  void ModelDescription::appendToPrivateCode(const std::string& c)
  {
    this->privateCode+=c;
    if(!this->privateCode.empty()){
      if(*(this->privateCode.rbegin())!='\n'){
	this->privateCode+='\n';
      }
    }
  } // end of ModelDescription::appendToPrivateCode

  void ModelDescription::appendToSources(const std::string& c)
  {
    this->sources+=c;
    if(!this->sources.empty()){
      if(*(this->sources.rbegin())!='\n'){
	this->sources+='\n';
      }
    }
  } // end of ModelDescription::appendToSources
  
  ModelDescription::~ModelDescription() = default;
  
} // end of namespace mfront
