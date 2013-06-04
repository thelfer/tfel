/*!
 * \file   MechanicalBehaviourDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#include<stdexcept>

#include"MFront/MechanicalBehaviourDescription.hxx"

namespace mfront{

  MechanicalBehaviourDescription::MechanicalBehaviourDescription()
    : usableInPurelyImplicitResolution(false),
      sTensor(false),
      aTensor(false),
      use_qt(false)      
  {} // end of MechanicalBehaviourDescription::MechanicalBehaviourDescription()

  std::vector<BoundsDescription>&
  MechanicalBehaviourDescription::getBounds(void)
  {
    return this->bounds;
  } // end of MechanicalBehaviourDescription::getBoundsDescriptions

  const std::vector<BoundsDescription>&
  MechanicalBehaviourDescription::getBounds(void) const
  {
    return this->bounds;
  } // end of MechanicalBehaviourDescription::getBoundsDescriptions

  const VarHandler&
  MechanicalBehaviourDescription::getStateVariableHandler(const std::string& v) const
  {
    return this->getVariableHandler(this->getStateVariables(),v);
  } // end of MechanicalBehaviourDescription::getStateVariableHandler

  const VarHandler&
  MechanicalBehaviourDescription::getVariableHandler(const VarContainer& cont,
						  const std::string& v) const
  {
    using namespace std;
    VarContainer::const_iterator p;
    for(p=cont.begin();p!=cont.end();++p){
      if(p->name==v){
	return *p;
      }
    }
    string msg("MechanicalBehaviourDescription::getVariableHandler : ");
    msg += "no state variable '"+v+"'";
    throw(runtime_error(msg));
    return *(static_cast<VarHandler*>(0));
  } // end of MechanicalBehaviourDescription::getVariableHandler

  bool
  MechanicalBehaviourDescription::isDrivingVariableName(const std::string& n) const
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(dv.name==n){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::isDrivingVariableName

  bool
  MechanicalBehaviourDescription::isDrivingVariableIncrementName(const std::string& n) const
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator pm;
    for(pm=this->getMainVariables().begin();pm!=this->getMainVariables().end();++pm){
      const DrivingVariable& dv = pm->first;
      if(dv.increment_known){
	if("d"+dv.name==n){
	  return true;
	}
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::isDrivingVariableIncrementName

  bool
  MechanicalBehaviourDescription::isMaterialPropertyName(const std::string& n) const
  {
    return MechanicalBehaviourDescription::contains(this->getMaterialProperties(),n);
  } // end of MechanicalBehaviourDescription::isMaterialPropertyName

  bool
  MechanicalBehaviourDescription::isLocalVariableName(const std::string& n) const
  {
    return MechanicalBehaviourDescription::contains(this->getLocalVariables(),n);
  } // end of MechanicalBehaviourDescription::isLocalVariableName

  bool
  MechanicalBehaviourDescription::isParameterName(const std::string& n) const
  {
    return MechanicalBehaviourDescription::contains(this->getParameters(),n);
  } // end of MechanicalBehaviourDescription::isParameterName
  
  bool
  MechanicalBehaviourDescription::isInternalStateVariableName(const std::string& n) const
  {
    return MechanicalBehaviourDescription::contains(this->getStateVariables(),n);
  } // end of MechanicalBehaviourDescription::isInternalStateVariableName

  bool
  MechanicalBehaviourDescription::isInternalStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return MechanicalBehaviourDescription::contains(this->getStateVariables(),n.substr(1));
  } // end of MechanicalBehaviourDescription::isInternalStateVariableName

  bool
  MechanicalBehaviourDescription::isAuxiliaryInternalStateVariableName(const std::string& n) const
  {
    return MechanicalBehaviourDescription::contains(this->getAuxiliaryStateVariables(),n);
  } // end of MechanicalBehaviourDescription::isInternalStateVariableName
  
  bool
  MechanicalBehaviourDescription::isExternalStateVariableName(const std::string& n) const
  {
    return MechanicalBehaviourDescription::contains(this->getExternalStateVariables(),n);
  } // end of MechanicalBehaviourDescription::isExternalStateVariableName

  bool
  MechanicalBehaviourDescription::isExternalStateVariableIncrementName(const std::string& n) const
  {
    if(n.size()<2){
      return false;
    }
    if(n[0]!='d'){
      return false;
    }
    return MechanicalBehaviourDescription::contains(this->getExternalStateVariables(),n.substr(1));
  } // end of MechanicalBehaviourDescription::isExternalStateVariableName

  bool
  MechanicalBehaviourDescription::contains(const VarContainer& v,
					const std::string& n)
  {
    VarContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      if(p->name==n){
	return true;
      }
    }
    return false;
  } // end of MechanicalBehaviourDescription::contains

  VarContainer&
  MechanicalBehaviourDescription::getMaterialProperties(void)
  {
    return this->materialProperties;
  } // end of MechanicalBehaviourDescription::getMaterialProperties

  const VarContainer&
  MechanicalBehaviourDescription::getMaterialProperties(void) const
  {
    return this->materialProperties;
  } // end of MechanicalBehaviourDescription::getMaterialProperties

  VarContainer&
  MechanicalBehaviourDescription::getStateVariables(void)
  {
    return this->stateVariables;
  } // end of MechanicalBehaviourDescription::getStateVariables

  const VarContainer&
  MechanicalBehaviourDescription::getStateVariables(void) const
  {
    return this->stateVariables;
  } // end of MechanicalBehaviourDescription::getStateVariables

  VarContainer&
  MechanicalBehaviourDescription::getAuxiliaryStateVariables(void)
  {
    return this->auxiliaryStateVariables;
  } // end of MechanicalBehaviourDescription::getAuxiliaryStateVariables

  const VarContainer&
  MechanicalBehaviourDescription::getAuxiliaryStateVariables(void) const
  {
    return this->auxiliaryStateVariables;
  } // end of MechanicalBehaviourDescription::getAuxiliaryStateVariables

  VarContainer&
  MechanicalBehaviourDescription::getExternalStateVariables(void)
  {
    return this->externalStateVariables;
  } // end of MechanicalBehaviourDescription::getExternalStateVariables

  const VarContainer&
  MechanicalBehaviourDescription::getExternalStateVariables(void) const
  {
    return this->externalStateVariables;
  } // end of MechanicalBehaviourDescription::getExternalStateVariables

  VarContainer&
  MechanicalBehaviourDescription::getLocalVariables(void)
  {
    return this->localVariables;
  } // end of MechanicalBehaviourDescription::getLocalVariables

  const VarContainer&
  MechanicalBehaviourDescription::getLocalVariables(void) const
  {
    return this->localVariables;
  } // end of MechanicalBehaviourDescription::getLocalVariables

  VarContainer&
  MechanicalBehaviourDescription::getParameters(void)
  {
    return this->parameters;
  } // end of MechanicalBehaviourDescription::getParameters
  
  const VarContainer&
  MechanicalBehaviourDescription::getParameters(void) const
  {
    return this->parameters;
  } // end of MechanicalBehaviourDescription::getParameters

  std::map<std::string,double>&
  MechanicalBehaviourDescription::getParametersDefaultValues()
  {
    return this->parametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getParametersDefaultValues

  const std::map<std::string,double>&
  MechanicalBehaviourDescription::getParametersDefaultValues() const
  {
    return this->parametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getParametersDefaultValues

  std::map<std::string,int>&
  MechanicalBehaviourDescription::getIntegerParametersDefaultValues()
  {
    return this->iParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getIntegerParametersDefaultValues

  const std::map<std::string,int>&
  MechanicalBehaviourDescription::getIntegerParametersDefaultValues() const
  {
    return this->iParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getIntegerParametersDefaultValues

  std::map<std::string,unsigned short>&
  MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues()
  {
    return this->uParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues

  const std::map<std::string,unsigned short>&
  MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues() const
  {
    return this->uParametersDefaultValues;
  } // end of MechanicalBehaviourDescription::getUnsignedShortParametersDefaultValues

  // MechanicalBehaviourDescription::MechanicalBehaviourDescription(const MechanicalBehaviourDescription& src)
  //   : pupirv(src.pupirv),
  //     usableInPurelyImplicitResolution(src.usableInPurelyImplicitResolution),
  //     sTensor(src.sTensor),
  //     aTensor(src.aTensor),
  //     use_qt(src.use_qt),
  //     type(src.type),
  //     etype(src.etype)
  // {} // end of MechanicalBehaviourDescription::MechanicalBehaviourDescription()

  // MechanicalBehaviourDescription&
  // MechanicalBehaviourDescription::operator = (const MechanicalBehaviourDescription& src)
  // {
  //   if(this!=&src){
  //     this->pupirv  = src.pupirv;
  //     this->use_qt  = src.use_qt;
  //     this->type    = src.type;
  //     this->etype    = src.etype;
  //     this->sTensor = src.sTensor;
  //     this->aTensor = src.aTensor;
  //     this->usableInPurelyImplicitResolution = src.usableInPurelyImplicitResolution;
  //   }
  //   return *this;
  // } // end of MechanicalBehaviourDescription::MechanicalBehaviourDescription()

  std::map<DrivingVariable,
	   ThermodynamicForce>&
  MechanicalBehaviourDescription::getMainVariables(void)
  {
    return this->mvariables;
  } // end of MechanicalBehaviourDescription::getMainVariables

  const std::map<DrivingVariable,
		 ThermodynamicForce>&
  MechanicalBehaviourDescription::getMainVariables(void) const
  {
    return this->mvariables;
  } // end of MechanicalBehaviourDescription::getMainVariables
  
  void
  MechanicalBehaviourDescription::setUseQt(const bool b)
  {
    this->use_qt = b;
  } // end of MechanicalBehaviourDescription::setUseQt

  bool
  MechanicalBehaviourDescription::useQt(void) const
  {
    return this->use_qt;
  } // end of MechanicalBehaviourDescription::useQt

  BehaviourType
  MechanicalBehaviourDescription::getElasticBehaviourType() const
  {
    return this->etype;
  } // end of MechanicalBehaviourDescription::getElasticBehaviourType

  void
  MechanicalBehaviourDescription::setElasticBehaviourType(const BehaviourType t)
  {
    this->etype = t;
  } // end of MechanicalBehaviourDescription::setElasticBehaviourType

  BehaviourType
  MechanicalBehaviourDescription::getBehaviourType() const
  {
    return this->type;
  } // end of MechanicalBehaviourDescription::getBehaviourType

  void
  MechanicalBehaviourDescription::setBehaviourType(const BehaviourType t)
  {
    this->type = t;
  } // end of MechanicalBehaviourDescription::setBehaviourType

  bool
  MechanicalBehaviourDescription::requiresStiffnessTensor(void) const
  {
    return this->sTensor; 
  } // end of MechanicalBehaviourDescription::requiresStiffnessTensor
  
  void
  MechanicalBehaviourDescription::setRequireStiffnessTensor(const bool b)
  {
    this->sTensor = b;
  } // end of MechanicalBehaviourDescription::setRequireStiffnessTensor

  bool
  MechanicalBehaviourDescription::requiresThermalExpansionTensor(void) const
  {
    return this->aTensor; 
  } // end of MechanicalBehaviourDescription::requiresThermalExpansionTensor
  
  void
  MechanicalBehaviourDescription::setRequireThermalExpansionTensor(const bool b)
  {
    this->aTensor = b;
  } // end of MechanicalBehaviourDescription::setRequireThermalExpansionTensor
  
  bool
  MechanicalBehaviourDescription::isUsableInPurelyImplicitResolution(void) const
  {
    return this->usableInPurelyImplicitResolution;
  } // end of MechanicalBehaviourDescription::isUsableInPurelyImplicitResolution

  void
  MechanicalBehaviourDescription::setUsableInPurelyImplicitResolution(const bool b)
  {
    this->usableInPurelyImplicitResolution = b;
  } // end of MechanicalBehaviourDescription::setUsableInPurelyImplicitResolution

  void
  MechanicalBehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string& n)
  {
    this->pupirv.insert(n);
  } // end of MechanicalBehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  const std::set<std::string>&
  MechanicalBehaviourDescription::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution(void) const
  {
    return this->pupirv;
  } // end of MechanicalBehaviourDescription::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution


} // end of namespace mfront
