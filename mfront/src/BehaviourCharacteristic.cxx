/*!
 * \file   BehaviourCharacteristic.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#include"MFront/BehaviourCharacteristic.hxx"

namespace mfront{

  BehaviourCharacteristic::BehaviourCharacteristic()
    : usableInPurelyImplicitResolution(false),
      sTensor(false),
      aTensor(false),
      use_qt(false)      
  {} // end of BehaviourCharacteristic::BehaviourCharacteristic()

  BehaviourCharacteristic::BehaviourCharacteristic(const BehaviourCharacteristic& src)
    : pupirv(src.pupirv),
      usableInPurelyImplicitResolution(src.usableInPurelyImplicitResolution),
      sTensor(src.sTensor),
      aTensor(src.aTensor),
      use_qt(src.use_qt),
      type(src.type),
      etype(src.etype)
  {} // end of BehaviourCharacteristic::BehaviourCharacteristic()

  BehaviourCharacteristic&
  BehaviourCharacteristic::operator = (const BehaviourCharacteristic& src)
  {
    if(this!=&src){
      this->pupirv  = src.pupirv;
      this->use_qt  = src.use_qt;
      this->type    = src.type;
      this->etype    = src.etype;
      this->sTensor = src.sTensor;
      this->aTensor = src.aTensor;
      this->usableInPurelyImplicitResolution = src.usableInPurelyImplicitResolution;
    }
    return *this;
  } // end of BehaviourCharacteristic::BehaviourCharacteristic()
  
  void
  BehaviourCharacteristic::setUseQt(const bool b)
  {
    this->use_qt = b;
  } // end of BehaviourCharacteristic::setUseQt

  bool
  BehaviourCharacteristic::useQt(void) const
  {
    return this->use_qt;
  } // end of BehaviourCharacteristic::useQt

  BehaviourType
  BehaviourCharacteristic::getElasticBehaviourType() const
  {
    return this->etype;
  } // end of BehaviourCharacteristic::getElasticBehaviourType

  void
  BehaviourCharacteristic::setElasticBehaviourType(const BehaviourType t)
  {
    this->etype = t;
  } // end of BehaviourCharacteristic::setElasticBehaviourType

  BehaviourType
  BehaviourCharacteristic::getBehaviourType() const
  {
    return this->type;
  } // end of BehaviourCharacteristic::getBehaviourType

  void
  BehaviourCharacteristic::setBehaviourType(const BehaviourType t)
  {
    this->type = t;
  } // end of BehaviourCharacteristic::setBehaviourType

  bool
  BehaviourCharacteristic::requiresStiffnessTensor(void) const
  {
    return this->sTensor; 
  } // end of BehaviourCharacteristic::requiresStiffnessTensor
  
  void
  BehaviourCharacteristic::setRequireStiffnessTensor(const bool b)
  {
    this->sTensor = b;
  } // end of BehaviourCharacteristic::setRequireStiffnessTensor

  bool
  BehaviourCharacteristic::requiresThermalExpansionTensor(void) const
  {
    return this->aTensor; 
  } // end of BehaviourCharacteristic::requiresThermalExpansionTensor
  
  void
  BehaviourCharacteristic::setRequireThermalExpansionTensor(const bool b)
  {
    this->aTensor = b;
  } // end of BehaviourCharacteristic::setRequireThermalExpansionTensor
  
  bool
  BehaviourCharacteristic::isUsableInPurelyImplicitResolution(void) const
  {
    return this->usableInPurelyImplicitResolution;
  } // end of BehaviourCharacteristic::isUsableInPurelyImplicitResolution

  void
  BehaviourCharacteristic::setUsableInPurelyImplicitResolution(const bool b)
  {
    this->usableInPurelyImplicitResolution = b;
  } // end of BehaviourCharacteristic::setUsableInPurelyImplicitResolution

  void
  BehaviourCharacteristic::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string& n)
  {
    this->pupirv.insert(n);
  } // end of BehaviourCharacteristic::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  const std::set<std::string>&
  BehaviourCharacteristic::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution(void) const
  {
    return this->pupirv;
  } // end of BehaviourCharacteristic::getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution


} // end of namespace mfront
