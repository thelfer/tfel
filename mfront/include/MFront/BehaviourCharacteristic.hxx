/*!
 * \file   BehaviourCharacteristic.hxx
 * \brief  This file declares the BehaviourCharacteristic class
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#ifndef _LIB_MFRONT_BEHAVIOURCHARACTERISTIC_H_
#define _LIB_MFRONT_BEHAVIOURCHARACTERISTIC_H_ 

#include<set>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace mfront{

  /*!
   * For the castem finite element code (the only one at the moment of
   * writing this comment), its is mandatory to distinguish isotropic
   * and orthotropic behaviours
   */
  enum BehaviourType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum BehaviourType

  /*!
   * This structure gathers various behaviour characteristic
   */
  struct TFEL_VISIBILITY_EXPORT BehaviourCharacteristic
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

    /*!
     * This method has been introduced to optimize the mechanical
     * resolution in licos : a purely implicit resolution only
     * evaluate external loadings at the end of the time step. This
     * implies that no increment of an external loading is used in the
     * behaviour law.
     */
    bool
    isUsableInPurelyImplicitResolution(void) const;

    /*!
     * \brief set if this behaviour can be used in a purely implicit
     * resolution.
     * \param[in] b : new value of the
     * usableInPurelyImplicitResolution member
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    void
    setUsableInPurelyImplicitResolution(const bool);

    /*!
     * \brief declares an external state variable to be probably
     * unusable in a purely implicit resolution. This means that its
     * increment is used somewhere.
     *
     * The term "probably" refers to the fact that it may happens that
     * the increment is only used to get the value of the external
     * state variable at a given date. For example, in a standard
     * resolution (non implicit), the value of \(T+\theta\,\Delta\,T\)
     * gives the temperature value at
     * \(t+\theta\,\Delta\,t\). However, this expression is still
     * valid in a purely implicit resolution.
     */
    void
    declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string&);

    /*!
     * \brief get all the external state variables found to be
     * probably unusable in a purely implicit resolution.
     */
    const std::set<std::string>&
    getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution(void) const;

  private:

    /*!
     * variables flagged as probably unusable in purely implicit
     * resolutions
     */
    std::set<std::string> pupirv;
    /*!
     * boolean stating if this behaviour can be used in a purely
     * implicit resolution.
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    bool usableInPurelyImplicitResolution;
    /*!
     * requires stiffness tensor
     */
    bool sTensor;
    /*!
     * requires thermal expansion tensor
     */
    bool aTensor;
    /*!
     * use units
     */
    bool use_qt;
    /*!
     * type of behaviour (isotropic or orthotropic)
     */
    BehaviourType type;
    
  }; // end of struct BehaviourCharacteristic

} // end of namespace mfront

#endif /* _LIB_MFRONT_BEHAVIOURCHARACTERISTIC_H */

