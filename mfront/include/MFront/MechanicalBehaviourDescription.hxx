/*!
 * \file   MechanicalBehaviourDescription.hxx
 * \brief  This file declares the MechanicalBehaviourDescription class
 * \author Helfer Thomas
 * \date   18 Jan 2007
 */

#ifndef _LIB_MFRONT_MECHANICALBEHAVIOURDESCRIPTION_H_
#define _LIB_MFRONT_MECHANICALBEHAVIOURDESCRIPTION_H_ 

#include<set>
#include<map>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/VarHandler.hxx"
#include"MFront/DrivingVariable.hxx"
#include"MFront/BoundsDescription.hxx"
#include"MFront/ThermodynamicForce.hxx"
#include"MFront/SupportedTypes.hxx"

namespace mfront{

  /*!
   * For the castem finite element code (the only one at the moment of
   * writing this comment), its is mandatory to distinguish isotropic
   * and orthotropic behaviours
   */
  enum SymmetryType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum SymmetryType

  /*!
   * This structure gathers various behaviour characteristic
   */
  struct TFEL_VISIBILITY_EXPORT MechanicalBehaviourDescription
    : private SupportedTypes
  {
    /*!
     * An indication of the type of the behaviour treated
     * If the behaviour is a small strain standard behaviour, then:
     * - the only driving variable is the total strain 'eto' (symmetric tensor)
     * - the only thermodynamic force is the stress    'sig' (symmetric tensor)
     * If the behaviour is a cohesive zone model, then:
     * - the only driving variable is the opening displacement (tvector)
     * - the only thermodynamic force is the traction (tvector)
     */
    enum BehaviourType {
      GENERALBEHAVIOUR              = -1,
      SMALLSTRAINSTANDARDBEHAVIOUR  =  0,
      FINITESTRAINSTANDARDBEHAVIOUR =  1,
      COHESIVEZONEMODEL             =  2
    }; // end of enum MFrontBehaviourType
    /*!
     * constructor
     */
    MechanicalBehaviourDescription();
    /*!
     * \return the size of the main variables
     */
    std::pair<SupportedTypes::TypeSize,
	      SupportedTypes::TypeSize>
    getMainVariablesSize(void) const;
    /*!
     * \brief add a new main variable
     * \note using this method means that the behaviour type is always
     * 'GENERALBEHAVIOUR'. This can't be changed afterwards.
     */
    void
    addMainVariable(const DrivingVariable&,
		    const ThermodynamicForce&);
    /*!
     * \return the main variables of the behaviour
     */
    const std::map<DrivingVariable,
		   ThermodynamicForce>&
    getMainVariables(void) const;
    /*!
     * \return the behaviour type
     */
    BehaviourType
    getBehaviourType(void) const;
    /*!
     * \brief set the behaviour to be a small strain standard
     * behaviour
     */
    void declareAsASmallStrainStandardBehaviour(void);
    /*!
     * \brief set the behaviour to be a cohesive zone model
     */
    void declareAsACohesiveZoneModel(void);
    /*!
     * \return the symmetry type of the behaviour
     */
    SymmetryType
    getSymmetryType() const;

    void
    setSymmetryType(const SymmetryType);

    SymmetryType
    getElasticSymmetryType() const;

    void
    setElasticSymmetryType(const SymmetryType);

    void setUseQt(const bool);
    /*!
     * \return the type of the stiffness operator
     */
    std::string
    getStiffnessOperatorType(void) const;

    bool requiresStiffnessOperator(void) const;

    void setRequireStiffnessOperator(const bool);

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

    VarContainer&
    getMaterialProperties(void);

    const VarContainer&
    getMaterialProperties(void) const;

    VarContainer&
    getStateVariables(void);

    const VarContainer&
    getStateVariables(void) const;

    VarContainer&
    getAuxiliaryStateVariables(void);

    const VarContainer&
    getAuxiliaryStateVariables(void) const;

    VarContainer&
    getExternalStateVariables(void);

    const VarContainer&
    getExternalStateVariables(void) const;

    VarContainer&
    getLocalVariables(void);

    const VarContainer&
    getLocalVariables(void) const;

    VarContainer&
    getParameters(void);

    const VarContainer&
    getParameters(void) const;

    std::map<std::string,double>&
    getParametersDefaultValues();

    const std::map<std::string,double>&
    getParametersDefaultValues() const;

    std::map<std::string,int>&
    getIntegerParametersDefaultValues();

    const std::map<std::string,int>&
    getIntegerParametersDefaultValues() const;

    std::map<std::string,unsigned short>&
    getUnsignedShortParametersDefaultValues();

    const std::map<std::string,unsigned short>&
    getUnsignedShortParametersDefaultValues() const;

    const VarHandler&
    getStateVariableHandler(const std::string&) const;

    const VarHandler&
    getVariableHandler(const VarContainer&,
		       const std::string&) const;

    bool
    isDrivingVariableName(const std::string&) const;

    bool
    isDrivingVariableIncrementName(const std::string&) const;

    bool
    isMaterialPropertyName(const std::string&) const;

    bool
    isLocalVariableName(const std::string&) const;

    bool
    isParameterName(const std::string&) const;

    bool
    isInternalStateVariableName(const std::string&) const;

    bool
    isInternalStateVariableIncrementName(const std::string&) const;

    bool
    isAuxiliaryInternalStateVariableName(const std::string&) const;

    bool
    isExternalStateVariableName(const std::string&) const;

    bool
    isExternalStateVariableIncrementName(const std::string&) const;

    std::vector<BoundsDescription>&
    getBounds(void);

    const std::vector<BoundsDescription>&
    getBounds(void) const;

  private:
    /*!
     * And helper method
     */
    static bool
    contains(const VarContainer&,
	     const std::string&);
    //! copy constructor (disabled)
    MechanicalBehaviourDescription(const MechanicalBehaviourDescription&);
    //! assignement operator constructor (disabled)
    MechanicalBehaviourDescription&
    operator = (const MechanicalBehaviourDescription&);
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
    bool sOperator;
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
    /*!
     * symmetry of behaviour (isotropic or orthotropic)
     */
    SymmetryType stype;
    /*!
     * symmetry of elastic behaviour (isotropic or orthotropic)
     */
    SymmetryType estype;
    /*!
     * main variables, association of a driving variable and a
     * thermodynamicforce
     */
    std::map<DrivingVariable,
	     ThermodynamicForce> mvariables;

    VarContainer materialProperties;
    VarContainer stateVariables;
    VarContainer auxiliaryStateVariables;
    VarContainer externalStateVariables;
    VarContainer localVariables;
    VarContainer parameters;

    std::map<std::string,double>         parametersDefaultValues;
    std::map<std::string,int>            iParametersDefaultValues;
    std::map<std::string,unsigned short> uParametersDefaultValues;
    std::vector<BoundsDescription>       bounds;

  }; // end of struct MechanicalBehaviourDescription

} // end of namespace mfront

#endif /* _LIB_MFRONT_MECHANICALBEHAVIOURDESCRIPTION_H */

