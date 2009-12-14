/*!
 * \file   BehaviourCharacteristic.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#include"BehaviourCharacteristic.hxx"

namespace mfront{

  BehaviourCharacteristic::BehaviourCharacteristic()
    : sTensor(false),
      aTensor(false),
      use_qt(false)
  {} // end of BehaviourCharacteristic::BehaviourCharacteristic()

  BehaviourCharacteristic::BehaviourCharacteristic(const BehaviourCharacteristic& src)
    : sTensor(src.sTensor),
      aTensor(src.aTensor),
      use_qt(src.use_qt),
      type(src.type)
  {} // end of BehaviourCharacteristic::BehaviourCharacteristic()

  BehaviourCharacteristic&
  BehaviourCharacteristic::operator = (const BehaviourCharacteristic& src)
  {
    // self assignement dont have to be checked
    this->use_qt   = src.use_qt;
    this->type    = src.type;
    this->sTensor = src.sTensor;
    this->aTensor = src.aTensor;
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

} // end of namespace mfront
