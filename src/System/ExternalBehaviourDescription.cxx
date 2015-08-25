/*!
 * \file   src/System/ExternalBehaviourDescription.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   15 June 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<algorithm>
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"TFEL/System/ExternalBehaviourDescription.hxx"

namespace tfel
{
  namespace system
  {
    
    ExternalBehaviourDescription::ExternalBehaviourDescription(const std::string& l,
							       const std::string& f,
							       const std::string& h){
      ExternalLibraryManager& elm = ExternalLibraryManager::getExternalLibraryManager();
      const std::vector<std::string> hypotheses = elm.getSupportedModellingHypotheses(l,f);
      if(std::find(hypotheses.begin(),hypotheses.end(),h)==hypotheses.end()){
	throw(std::runtime_error("ExternalBehaviourDescription::ExternalBehaviourDescription : "
				 "unsupported hypothesis '"+h+"' for behaviour '"+l+"'"
				 "in library '"+l+"'"));
      }
      this->source  = elm.getSource(l,f);
      this->btype   = elm.getUMATBehaviourType(l,f);
      this->stype   = elm.getUMATSymmetryType(l,f);
      this->etype   = elm.getUMATElasticSymmetryType(l,f);
      this->isUPUIR = elm.isUMATBehaviourUsableInPurelyImplicitResolution(l,f,h);
      this->mpnames = elm.getUMATMaterialPropertiesNames(l,f,h);
      this->ivnames = elm.getUMATInternalStateVariablesNames(l,f,h);
      this->ivtypes = elm.getUMATInternalStateVariablesTypes(l,f,h);
      this->evnames = elm.getUMATExternalStateVariablesNames(l,f,h);
      this->requiresStiffnessTensor =
	elm.getUMATRequiresStiffnessTensor(l,f,h);
      this->requiresThermalExpansionCoefficientTensor =
	elm.getUMATRequiresThermalExpansionCoefficientTensor(l,f,h);
    }

    ExternalBehaviourDescription::ExternalBehaviourDescription()
    {} // end of ExternalBehaviourDescription::ExternalBehaviourDescription

    ExternalBehaviourDescription::ExternalBehaviourDescription(const ExternalBehaviourDescription& src)
    {
      this->operator=(src);
    }

    ExternalBehaviourDescription&
    ExternalBehaviourDescription::operator=(const ExternalBehaviourDescription& src){
      this->mpnames = src.mpnames;
      this->ivnames = src.ivnames;
      this->ivtypes = src.ivtypes;
      this->evnames = src.evnames;
      this->source  = src.source ;
      this->btype   = src.btype  ;
      this->stype   = src.stype  ;
      this->etype   = src.etype  ;
      this->isUPUIR = src.isUPUIR;
      this->requiresStiffnessTensor = src.requiresStiffnessTensor;
      this->requiresThermalExpansionCoefficientTensor = src.requiresThermalExpansionCoefficientTensor;
      return *this;
    }
    
    ExternalBehaviourDescription::~ExternalBehaviourDescription()
    {}
        
  } // end of namespace system

} // end of namespace tfel
