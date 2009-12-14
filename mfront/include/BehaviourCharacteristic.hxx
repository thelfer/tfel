/*!
 * \file   BehaviourCharacteristic.hxx
 * \brief  This file declares the BehaviourCharacteristic class
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#ifndef _LIB_MFRONT_BEHAVIOURCHARACTERISTIC_H_
#define _LIB_MFRONT_BEHAVIOURCHARACTERISTIC_H_ 

namespace mfront{

  enum BehaviourType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum BehaviourType

  struct BehaviourCharacteristic
  {

    BehaviourCharacteristic();

    BehaviourCharacteristic(const BehaviourCharacteristic&);

    BehaviourCharacteristic&
    operator = (const BehaviourCharacteristic&);

    BehaviourType
    getBehaviourType() const;

    void
    setBehaviourType(const BehaviourType);

    void setUseQt(const bool);

    bool requiresStiffnessTensor(void) const;

    void setRequireStiffnessTensor(const bool);

    bool requiresThermalExpansionTensor(void) const;

    void setRequireThermalExpansionTensor(const bool);

    bool useQt(void) const;

  private:
    bool sTensor;
    bool aTensor;
    bool use_qt;
    BehaviourType type;
  }; // end of struct BehaviourCharacteristic

} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURCHARACTERISTIC_H */

