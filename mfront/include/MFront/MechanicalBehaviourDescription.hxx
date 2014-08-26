/*! 
 * \file  MechanicalBehaviourDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 mars 2014
 */

#ifndef _LIB_MFRONT_MECHANICALBEHAVIOURDESCRIPTION_H_
#define _LIB_MFRONT_MECHANICALBEHAVIOURDESCRIPTION_H_ 

#include<set>
#include<map>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/CodeBlock.hxx"
#include"MFront/SupportedTypes.hxx"
#include"MFront/MechanicalBehaviourAttribute.hxx"
#include"MFront/MechanicalBehaviourData.hxx"
#include"MFront/MechanicalBehaviourSymmetryType.hxx"

namespace mfront
{

  /*!
   * This structure describes a mechanical behaviour
   *
   * This class handles the fact that the mechanical behaviour
   * data may depend on the modelling hypothesis considered,
   * altough most of the time they won't.
   *
   * This class thus stores a default mechanical behaviour data
   * which is intendend to be valid for most hypotheses, and specific
   * mechanical behaviour datas when required.
   */
  struct MechanicalBehaviourDescription
    : public tfel::material::MechanicalBehaviourBase,
      public SupportedTypes  
  {
    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef tfel::material::ModellingHypothesis::Hypothesis Hypothesis;
    //! a simple alias
    typedef MechanicalBehaviourData::Mode Mode;
    //! a simple alias
    typedef MechanicalBehaviourData::Position Position;
    //! attribute name
    static const std::string requiresStiffnessTensor;
    //! attribute name
    static const std::string requiresUnAlteredStiffnessTensor;
    //! attribute name
    static const std::string requiresThermalExpansionCoefficientTensor;
    //! attribute name
    static const std::string setRequireThermalExpansionCoefficientTensor;
    /*!
     * constructor
     */
    MechanicalBehaviourDescription();
    /*!
     * \brief set the behaviour name
     * \param[in] b : behaviour name
     */
    void setBehaviourName(const std::string&);
    /*!
     * \return the behaviour name
     */
    const std::string&
    getBehaviourName(void) const;
    /*!
     * \brief set the material name
     * \param[in] m : material name
     */
    void setMaterialName(const std::string&);
    /*!
     * \return the material name
     */
    const std::string&
    getMaterialName(void) const;
    /*!
     * \brief set the library name
     * \param[in] l : library name
     */
    void setLibrary(const std::string&);
    /*!
     * \return the material name
     */
    const std::string&
    getLibrary(void) const;
    /*!
     * \brief set the class name
     * \param[in] n : class name
     */
    void setClassName(const std::string&);
    /*!
     * \return the class name
     */
    const std::string&
    getClassName(void) const;
    /*!
     * \brief append the given code to the sources
     * \param[in] s : sources
     */
    void appendToSources(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getSources(void) const;
    /*!
     * \brief append the given code to the members
     * \param[in] s : members
     */
    void appendToMembers(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getMembers(void) const;
    /*!
     * \brief append the given code to the members
     * \param[in] s : members
     */
    void appendToPrivateCode(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getPrivateCode(void) const;
    /*!
     * \brief append the given code to the includes
     * \param[in] s : includes
     */
    void appendToIncludes(const std::string&);
    /*!
     * \return the material
     */
    const std::string&
    getIncludes(void) const;
    /*!
     * add a material law
     */
    void addMaterialLaw(const std::string&);
    /*!
     * \return the list of material laws
     */
    const std::vector<std::string>&
    getMaterialLaws(void) const;
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
     * \brief set the behaviour to be a small strain standard
     * behaviour
     */
    void declareAsASmallStrainStandardBehaviour(void);
    /*!
     * \brief set the behaviour to be a finite strain standard
     * behaviour
     */
    void declareAsAFiniteStrainStandardBehaviour(void);
    /*!
     * \brief set the behaviour to be a cohesive zone model
     */
    void declareAsACohesiveZoneModel(void);
    /*!
     * \return the type of the stiffness operator
     */
    std::string
    getStiffnessOperatorType(void) const;
    /*!
     * \return a type suitable for storing stress-free expansion
     */
    std::string
    getStressFreeExpansionType(void) const;

    bool useQt(void) const;

    void setUseQt(const bool);

    bool
    isDrivingVariableName(const std::string&) const;

    bool
    isDrivingVariableIncrementName(const std::string&) const;

    /*!
     * \return the behaviour type
     */
    BehaviourType
    getBehaviourType(void) const;
    /*!
     * \return the symmetry type of the behaviour
     */
    MechanicalBehaviourSymmetryType
    getSymmetryType() const;

    void
    setSymmetryType(const MechanicalBehaviourSymmetryType);

    MechanicalBehaviourSymmetryType
    getElasticSymmetryType() const;

    void
    setElasticSymmetryType(const MechanicalBehaviourSymmetryType);
    /*!
     * \return a mechanical behaviour data associated with the
     * given modelling hypothesis
     * \param[in] h : modelling hypothesis
     */
    const MechanicalBehaviourData&
    getMechanicalBehaviourData(const ModellingHypothesis::Hypothesis&) const;
    /*!
     * \return true if modelling hypotheses are defined.
     */
    bool
    areModellingHypothesesDefined(void) const;
    /*!
     * \return all the modelling hypotheses supported by the
     * behaviour.
     */
    const std::set<Hypothesis>&
    getModellingHypotheses(void) const;
    /*!
     * \return all the modelling hypotheses which are distint.
     *
     * This method returns, among all the supported modelling
     * hypotheses (see getModellingHypotheses), the ones which have
     * specialised mechanical data and, if at least one modelling
     * hypothesis has no specialisation, UNDEFINEDHYPOTHESIS.
     *
     * This method has been introduced for iteration purpose in
     * behaviour parsers.
     */
    std::set<Hypothesis>
    getDistinctModellingHypotheses(void) const;
    /*!
     * \return true if the given modelling hypothesis is supported
     * \param[in] h : modelling hypothesis
     * \see hasSpecialisedMechanicalData
     */
    bool
    isModellingHypothesisSupported(const Hypothesis) const;
    /*!
     * \brief set the list of supported modelling hypotheses
     * \param[in] h : supported modelling hypothesis
     */
    void
    setModellingHypotheses(const std::set<Hypothesis>&);
    /*!
     * \brief add material properties
     * \param[in] h : modelling hypothesis
     * \param[in] v : material properties added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addMaterialProperties(const Hypothesis,
			  const VariableDescriptionContainer&);
    /*!
     * \brief add a material property
     * \param[in] h : modelling hypothesis
     * \param[in] v : material property added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addMaterialProperty(const Hypothesis,
			const VariableDescription&);
    /*!
     * \brief add a state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variable added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addIntegrationVariable(const Hypothesis,
			   const VariableDescription&);
    /*!
     * \brief add state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variables added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addIntegrationVariables(const Hypothesis,
			    const VariableDescriptionContainer&);
    /*!
     * \brief add a state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variable added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addStateVariable(const Hypothesis,
		     const VariableDescription&);
    /*!
     * \brief add state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : state variables added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void
    addStateVariables(const Hypothesis,
		      const VariableDescriptionContainer&);
    /*!
     * \brief add auxiliary state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : auxiliary state variables added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the auxiliary state
     * variables to the default data and to all the specialisations
     */
    void
    addAuxiliaryStateVariables(const Hypothesis,
			       const VariableDescriptionContainer&);
    /*!
     * \brief add an auxiliary state variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : auxiliary state variable added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the auxiliary state
     * variables to the default data and to all the specialisations
     */
    void
    addAuxiliaryStateVariable(const Hypothesis,
			      const VariableDescription&);
    /*!
     * \brief add external state variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : external state variables added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the external state
     * variables to the default data and to all the specialisations
     */
    void
    addExternalStateVariables(const Hypothesis,
			      const VariableDescriptionContainer&);
    /*!
     * \brief add external state variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : external state variable added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the external state
     * variables to the default data and to all the specialisations
     */
    void
    addExternalStateVariable(const Hypothesis,
			     const VariableDescription&);
    /*!
     * \brief add a local variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : local variable added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variables to
     * the default data and to all the specialisations
     */
    void
    addLocalVariable(const Hypothesis,
		     const VariableDescription&);
    /*!
     * \brief add local variables
     * \param[in] h : modelling hypothesis
     * \param[in] v : local variables added
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variables to
     * the default data and to all the specialisations
     */
    void
    addLocalVariables(const Hypothesis,
		      const VariableDescriptionContainer&);
    /*!
     * \brief add a static variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : static variable
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variable to
     * the default data and to all the specialisations
     */
    void
    addStaticVariable(const Hypothesis,
		      const StaticVariableDescription&);
    /*!
     * \brief add a material property
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addParameter(const Hypothesis,
		 const VariableDescription&);
    /*!
     * \return true if the given name is the one of a material
     * property.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isMaterialPropertyName(const Hypothesis,
			   const std::string&) const;
    /*!
     * \return true if the given name is the one of a local variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isLocalVariableName(const Hypothesis,
			const std::string&) const;
    /*!
     * \return true if the given name is the one of an persistent
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isPersistentVariableName(const Hypothesis,
			      const std::string&) const;
    /*!
     * \return true if the given name is the one of an integration
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isIntegrationVariableName(const Hypothesis,
			      const std::string&) const;
    /*!
     * \return true if the given name is the one of an integration
     * variable increment.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isIntegrationVariableIncrementName(const Hypothesis,
				       const std::string&) const;
    /*!
     * \return true if the given name is the one of an state
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isStateVariableName(const Hypothesis,
			const std::string&) const;
    /*!
     * \return true if the given name is the one of an state
     * variable increment.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isStateVariableIncrementName(const Hypothesis,
				 const std::string&) const;
    /*!
     * \return true if the given name is the one of an auxiliary
     * internal state variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isAuxiliaryStateVariableName(const Hypothesis,
				 const std::string&) const;
    /*!
     * \return true if the given name is the one of an external state
     * variable.
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isExternalStateVariableName(const Hypothesis,
				const std::string&) const;
    /*!
     * \return true if the given name is the one of an external state
     * variable increment
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isExternalStateVariableIncrementName(const Hypothesis,
					 const std::string&) const;
    /*!
     * \return true if the given name is the one of a static variable
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isStaticVariableName(const Hypothesis,
			 const std::string&) const;
    /*!
     * \brief check if one has to include tvector.hxx or vector.hxx
     * \param[in] b1 : requires true if one has to include tvector.hxx
     * \param[in] b2 : requires true if one has to include vector.hxx
     */
    void
    requiresTVectorOrVectorIncludes(bool&,bool&);   
    /*!
     * \return true if all mechanical data are specialised
     * This means that the default mechanical data is useless
     */
    bool areAllMechanicalDataSpecialised(void) const;
    /*!
     * \return true if all mechanical data associated with the given
     * modelling hypothesis are specialised.
     * \param[in] h : modelling hypotheses considered
     */
    bool areAllMechanicalDataSpecialised(const std::set<Hypothesis>&) const;
    /*!
     * \return true if the mechanical data associated with the given
     * modelling hypothesis is specialised.
     * \param[in] h : modelling hypothesis considered
     */
    bool hasSpecialisedMechanicalData(const Hypothesis) const;
    /*!
     * \return true if thermal expansion coefficient were defined
     */
    bool
    areThermalExpansionCoefficientsDefined(void) const;
    /*!
     * \return the thermal expansion coefficients
     */
    const std::vector<tfel::utilities::shared_ptr<MaterialPropertyDescription> >&
    getThermalExpansionCoefficients(void) const;
    /*!
     * set the behaviour thermal expansion coefficient (isotropic behaviour)
     * \param[in] a : thermal expansion
     */
    void setThermalExpansionCoefficient(const tfel::utilities::shared_ptr<MaterialPropertyDescription>&);
    /*!
     * set the behaviour thermal expansions coefficient (orthotropic behaviour)
     * \param[in] a1 : thermal expansion in the first direction
     * \param[in] a2 : thermal expansion in the second direction
     * \param[in] a3 : thermal expansion in the third  direction
     */
    void setThermalExpansionCoefficients(const tfel::utilities::shared_ptr<MaterialPropertyDescription>&,
					 const tfel::utilities::shared_ptr<MaterialPropertyDescription>&,
					 const tfel::utilities::shared_ptr<MaterialPropertyDescription>&);
    /*!
     * \return the glossary names associated with the variables
     * contained in the given container
     * \param[in] h : modelling Hypothesis
     * \param[in] v : variables for which glossary names are requested
     */
    std::vector<std::string>
    getGlossaryNames(const Hypothesis,
		     const VarContainer&) const;
    /*!
     * get the glossary names associated with the variables
     * contained in the given container
     * \param[out] n : names
     * \param[in]  h : modelling Hypothesis
     * \param[in]  v : variables for which glossary names are requested
     */
    void
    getGlossaryNames(std::vector<std::string>&,
		     const Hypothesis,
		     const VarContainer&) const;
    /*!
     * get the glossary names associated with the variables
     * contained in the given container
     * \param[out] n : names
     * \param[in]  h : modelling Hypothesis
     * \param[in]  v : variables for which glossary names are requested
     */
    void
    appendGlossaryNames(std::vector<std::string>&,
			const Hypothesis,
			const VarContainer&) const;
    /*!
     * \return true if the given variable is used in a code block
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isVariableUsedInCodeBlocks(const Hypothesis,
			       const std::string&) const;
    /*!
     * \return true if the given name is a parameter name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isParameterName(const Hypothesis,
		    const std::string&) const;
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const Hypothesis,
			     const std::string&,
			     const double);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const Hypothesis,
			     const std::string&,
			     const int);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const Hypothesis,
			     const std::string&,
			     const unsigned short);
    /*!
     * \return the default value of a paramater
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter default value
     */
    double
    getFloattingPointParameterDefaultValue(const Hypothesis,
					   const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter default value
     */
    int
    getIntegerParameterDefaultValue(const Hypothesis,
				    const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] h : modelling hypothesis
     * \param[in] v : parameter default value
     */
    unsigned short
    getUnsignedShortParameterDefaultValue(const Hypothesis,
					  const std::string&) const;
    /*!
     * associate a glossary name to a variable
     * \param[in] h : modelling Hypothesis
     * \param[in] n : variable name
     * \param[in] g : glossary name
     */
    void
    setGlossaryName(const Hypothesis,
		    const std::string&,
		    const std::string&);
    /*!
     * \return true if the given name is an glossary name
     * \param[in] h : modelling Hypothesis
     * \param[in] n : name
     */
    bool
    isGlossaryName(const Hypothesis,
		   const std::string&) const;
    /*!
     * associate an entry name to a variable
     * \param[in] h : modelling Hypothesis
     * \param[in] n : variable name
     * \param[in] e : entry name
     */
    void
    setEntryName(const Hypothesis,
		 const std::string&,
		 const std::string&);
    /*!
     * get the glossary name or the entry name of a variable
     * \param[in] h : modelling Hypothesis
     * \param[in] n : variable name
     */
    std::string
    getGlossaryName(const Hypothesis h,
		    const std::string& n) const;
    /*!
     * \return the name of the variable associated with the given
     * glossary or entry name
     * \param[in] h : modelling Hypothesis
     * \param[in] n : name
     */
    std::string
    getVariableNameFromGlossaryNameOrEntryName(const Hypothesis,
					       const std::string&) const;
    /*!
     * \return true if the given name is an entry name
     * \param[in] h : modelling Hypothesis
     * \param[in] n : name
     */
    bool
    isEntryName(const Hypothesis,
		const std::string&) const;
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
     *
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    void
    declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis,
									   const std::string&);
    /*!
     * \brief set if this behaviour can be used in a purely implicit
     * resolution.
     * \param[in] h : modelling hypothesis
     * \param[in] b : new value of the
     * usableInPurelyImplicitResolution member
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    void
    setUsableInPurelyImplicitResolution(const Hypothesis,
					const bool);
    /*!
     * \brief declare or update a code block
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     * \param[in] c : code
     * \param[in] m : mode
     * \param[in] p : position
     */
    void setCode(const Hypothesis,
		 const std::string&,
		 const CodeBlock&,
		 const Mode,
		 const Position);
    /*!
     * \brief return the name of the code blocks defined so far
     * \param[in] h : modelling hypothesis
     */
    std::vector<std::string>
    getCodeBlockNames(const Hypothesis) const;
    /*!
     * \return the code with the given name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    const CodeBlock&
    getCode(const Hypothesis,
	    const std::string&) const;
    /*!
     * \return the code with the given name
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasCode(const Hypothesis,
	    const std::string&) const;
    /*!
     * \brief set the bound description for a given variable
     * \param[in] h : modelling hypothesis
     * \param[in] b : bounds description
     */
    void
    setBounds(const Hypothesis,
	      const BoundsDescription&);
    /*!
     * \brief set a mechanical attribute
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const Hypothesis,
		      const std::string&,
		      const MechanicalBehaviourAttribute&,
		      const bool = false);
    /*!
     * \return a mechanical attribute
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    template<typename T>
    const T&
    getAttribute(const Hypothesis,
		 const std::string&) const;
    /*!
     * \return a mechanical attribute if it exists or the default
     * value
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     * \param[in] v : default value
     */
    template<typename T>
    const T
    getAttribute(const Hypothesis,
		 const std::string&,
		 const T&) const;
    /*!
     * \return true an attribute with the given name has been declared
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasAttribute(const Hypothesis,
		 const std::string&) const;
    /*!
     * \return true a parameter with the given name has been declared
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    hasParameter(const Hypothesis,
		 const std::string&) const;
    /*!
     * \return true the given modelling hypothesis has a
     * parameter.
     * \param[in] h : modelling hypothesis \param[in] n :
     * name
     */
    bool
    hasParameters(const Hypothesis) const;
    /*!
     * \return true if at least one modelling hypothesis has a
     * parameter.
     */
    bool
    hasParameters(void) const;
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void
    setAttribute(const std::string&,
		 const MechanicalBehaviourAttribute&,
		 const bool);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool
    hasAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename tfel::meta::EnableIf<
      tfel::meta::TLCountNbrOfT<T,MechanicalBehaviourAttributeTypes>::value==1, 
      T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename tfel::meta::EnableIf<
      tfel::meta::TLCountNbrOfT<T,MechanicalBehaviourAttributeTypes>::value==1, 
      const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename tfel::meta::EnableIf<
      tfel::meta::TLCountNbrOfT<T,MechanicalBehaviourAttributeTypes>::value==1, 
      T>::type
    getAttribute(const std::string&,
		 const T&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string,MechanicalBehaviourAttribute>&
    getAttributes(void) const;
    /*!
     * \brief explicitely register a variable
     * \param[in] h : modelling hypothesis
     * \param[in] v : variable name
     */
    void registerVariable(const Hypothesis,
			  const std::string&);
  private:
    /*!
     * \brief throw an exception saying that no attribute with the given name exists
     */
    static void throwUndefinedAttribute(const std::string&);
    /*!
     * update the class name
     */
    void updateClassName();
    /*!
     * \brief create the mechanical behaviour data associated with the
     * given modelling hypothesis if necessary, and returns it.
     * \param[in] h : modelling hypothesis
     */
    MechanicalBehaviourData&
    getMechanicalBehaviourData2(const ModellingHypothesis::Hypothesis&);
    /*!
     * \call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : argument given to the behaviour data's method
     */
    template<typename Res,
	     typename Arg1>
    Res
    getData(const Hypothesis,
	    Res (MechanicalBehaviourData:: *)(const Arg1&) const,
	    const Arg1&) const;
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1>
    void
    callMechanicalBehaviourData(const Hypothesis,
    				void (MechanicalBehaviourData:: *)(const Arg1&),
    				const Arg1&);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : first  argument given to the behaviour data's method
     * \param[in] b : second argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1,
	     typename Arg2>
    void
    callMechanicalBehaviourData(const Hypothesis,
    				void (MechanicalBehaviourData:: *)(const Arg1&,
								   const Arg2),
    				const Arg1&,const Arg2);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : first  argument given to the behaviour data's method
     * \param[in] b : second argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1,
	     typename Arg2>
    void
    callMechanicalBehaviourData(const Hypothesis,
    				void (MechanicalBehaviourData:: *)(const Arg1&,
								   const Arg2&),
    				const Arg1&,const Arg2&);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h : modelling hypothesis
     * \param[in] m : behaviour data method
     * \param[in] a : argument given to the behaviour data's method
     * \note if h is Hypothesis::UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called.
     */
    template<typename Arg1>
    void
    callMechanicalBehaviourData(const Hypothesis,
				void (MechanicalBehaviourData:: *)(const Arg1),
				const Arg1);
    /*!
     * \brief add variables to the behaviour data
     * \param[in] h : modelling hypothesis
     * \param[in] v : variables to be added
     * \param[in] m : method retrieving the variable container     
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addVariables(const Hypothesis,
		 const VariableDescriptionContainer&,
		 void (MechanicalBehaviourData::*)(const VariableDescription&));
    /*!
     * \brief add a variable to the behaviour data
     * \param[in] h : modelling hypothesis
     * \param[in] v : variable to be added
     * \param[in] m : method retrieving the variable container     
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void
    addVariable(const Hypothesis,
		const VariableDescription&,
		void (MechanicalBehaviourData::*)(const VariableDescription&));
    /*!
     * \brief add variables to the behaviour data
     * \param[out] b : behaviour data
     * \param[in]  v : variables to be added
     * \param[in]  m : method retrieving the variable container     
     */
    void
    addVariables(MechanicalBehaviourData&,
		 const VariableDescriptionContainer&,
		 void (MechanicalBehaviourData::*)(const VariableDescription&));
    /*!
     * \brief add a variable to the behaviour data
     * \param[out] b : behaviour data
     * \param[in]  v : variable to be added
     * \param[in]  m : method retrieving the variable container     
     */
    void
    addVariable(MechanicalBehaviourData&,
		const VariableDescription&,
		void (MechanicalBehaviourData::*)(const VariableDescription&));
    /*!
     * \brief check that the given hypothesis is supported
     * \param[in] h : modelling hypothesis
     */
    void
    checkModellingHypothesis(const Hypothesis&) const;
    //! a simple alias
    typedef tfel::utilities::shared_ptr<MechanicalBehaviourData> MBDPtr;
    /*!
     * behaviour attributes
     */
    std::map<std::string,
	     MechanicalBehaviourAttribute> attributes;
    /*!
     * behaviour name
     */
    std::string behaviour;
    /*!
     * library name
     */
    std::string library;
    /*!
     * material name
     */
    std::string material;
    /*!
     * name of the generated class
     */
    std::string className;
    /*!
     * included header files
     */
    std::string includes;
    /*!
     * specific sources
     */
    std::string sources;
    /*!
     * private code
     */
    std::string privateCode;
    /*!
     * class member
     */
    std::string members;
    /*!
     * list of modelling hypotheses for
     * which the behaviour is defined
     */
    mutable std::set<Hypothesis> hypotheses;
    /*!
     * list of modelling hypotheses for
     * which this class returned a mechanical data
     */
    mutable std::set<Hypothesis> requestedHypotheses;
    /*!
     * default mechanical behaviour data
     */
    MechanicalBehaviourData md;
    /*!
     * specialisations
     */
    std::map<Hypothesis,MBDPtr> smd;
    /*!
     * main variables, association of a driving variable and a
     * thermodynamicforce
     */
    std::map<DrivingVariable,
	     ThermodynamicForce> mvariables;
    /*!
     * average thermal coefficient
     * For isotropic   behaviours, only one thermal expansion coefficient must be defined.
     * For orthotropic behaviours, three thermal expansions coefficient must be defined.
     */
    std::vector<tfel::utilities::shared_ptr<MaterialPropertyDescription> > thermalExpansionCoefficients;
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
    MechanicalBehaviourSymmetryType stype;
    /*!
     * symmetry of elastic behaviour (isotropic or orthotropic)
     */
    MechanicalBehaviourSymmetryType estype;
    /*!
     * list of material laws used
     */
    std::vector<std::string> materialLaws;
  }; // end of struct MechanicalBehaviourDescription

} // end of namespace mfront

#include"MFront/MechanicalBehaviourDescription.ixx"

#endif /* _LIB_MFRONT_MECHANICALBEHAVIOURDESCRIPTION_H */
