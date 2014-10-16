/*! 
 * \file  mfront/include/MFront/MFrontUMATInterfaceBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTUMATINTERFACEBASE_H_
#define _LIB_MFRONT_MFRONTUMATINTERFACEBASE_H_ 

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/SupportedTypes.hxx"
#include"MFront/MechanicalBehaviourDescription.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"

namespace mfront{
  
  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct MFrontUMATInterfaceBase
    : public SupportedTypes,
      public MFrontBehaviourVirtualInterface
  {
    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef ModellingHypothesis::Hypothesis Hypothesis;

    MFrontUMATInterfaceBase();

    /*!
     * set if dynamically allocated arrays are allowed
     * \param[in] b : boolean
     */
    virtual void
    allowDynamicallyAllocatedArrays(const bool);

    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const MechanicalBehaviourDescription&) const;

    virtual void
    exportMechanicalData(std::ofstream&,
			 const Hypothesis,
			 const MechanicalBehaviourDescription&) const;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile           : output file
     * \param[in] mb                      : mechanical behaviour description
     * \param[in] initStateVarsIncrements : constructor part assigning
     *                                      default value (zero) to state
     *                                      variable increments
     */
    virtual void
    writeBehaviourConstructor(std::ofstream&,
			      const MechanicalBehaviourDescription&,
			      const std::string&) const;

    virtual void
    writeBehaviourDataConstructor(std::ofstream&,
				  const Hypothesis,
				  const MechanicalBehaviourDescription&) const;

    /*!
     * write the initialisation of the driving variables
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeBehaviourDataMainVariablesSetters(std::ofstream&,
					   const MechanicalBehaviourDescription&) const;
    
    virtual void
    writeIntegrationDataConstructor(std::ofstream&,
				    const Hypothesis,
				    const MechanicalBehaviourDescription&) const;
    /*!
     * write the initialisation of the driving variables
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeIntegrationDataMainVariablesSetters(std::ofstream&,
					     const MechanicalBehaviourDescription&) const;


    virtual void
    reset(void);
    
    virtual ~MFrontUMATInterfaceBase();

  protected:

    /*!
     * The umat interface may require additionnal material properties,
     * to compute the stiffness matrix or to compute the thermal
     * expansions. Some additional material properties may also be
     * required by the solver (mass density, orthotropic basis for the
     * Cast3M solver) which are of no interest for the behaviour. On
     * the orher hand, some material properties are highly needed by
     * the behaviour (Young modulus and Poisson ratio for example).
     * This structure allows the computation of an indirection map
     * between the data given by the solver and the ones needed by the
     * interface.
     */
    struct UMATMaterialProperty
    {
      /*!
       * constructor
       * \param[in] t : type
       * \param[in] n : name
       * \param[in] v : usual variable name
       * \param[in] a : array size
       * \param[in] o : offset
       * \param[in] d : dummy variable
       */
      UMATMaterialProperty(const std::string&,
			   const std::string&,
			   const std::string&,
			   const unsigned short,
			   const SupportedTypes::TypeSize,
			   const bool);
      /*!
       *  Type of the variable.
       *  If the variable has been declared as an array (see below),
       *  this field holds the type contained by the array.
       */
      std::string type;
      /*!
       * \brief glossary name of the material property
       */
      std::string name;
      /*!
       * \brief variable name of the material property
       */
      std::string var_name;
      //! if the variable has been declared as an array, this field
      //  contains a value greater than 1
      unsigned short arraySize;
      /*!
       * offset of the variable in the indirection map
       */
      SupportedTypes::TypeSize offset;
      /*!
       * If true, this variable is not considered in the compoutation
       * of the indirection map. For example, the Cast3M solver may
       * require a variable called 'V1X' for the first component of
       * the first direction of the orthotropic basis.  The variable
       * shall not prevent the user of defining a material property
       * named 'V1X'.
       */
       bool dummy;
    }; // end of struct UMATMaterialProperty
    /*!
     * an helper function returning a material property
     * \param[in] mprops : material properties
     * \param[in] n      : glossary name
     * \return the umat material property with the given material
     * property name
     */
    static const UMATMaterialProperty&
    findUMATMaterialProperty(const std::vector<UMATMaterialProperty>&,
			     const std::string&);

    virtual std::string
    getInterfaceName(void) const = 0;

    virtual std::string
    getLibraryName(const MechanicalBehaviourDescription&) const = 0;

    virtual bool
    readBooleanValue(const std::string&,
		     tfel::utilities::CxxTokenizer::TokensContainer::const_iterator&,
		     const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) const;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and whose second members is an offset giving the number of
     * imposed material properties.
     * \param[in] mb : mechanical behaviour description
     * \param[in] h  : modelling hypothesis
     */
    virtual std::pair<std::vector<UMATMaterialProperty>,
		      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const MechanicalBehaviourDescription&,
				const Hypothesis) const = 0;
    /*!
     * \brief append a variable to material property list
     * \param[out] l : list of material properties
     * \param[in]  t : type of the variable
     * \param[in]  n : name of the variable
     * \param[in]  v : variable name
     * \param[in]  b : if true, this is a dummy variable
     */
    virtual void
    appendToMaterialPropertiesList(std::vector<UMATMaterialProperty>&,
				   const std::string&,
				   const std::string&,
				   const std::string&,
				   const bool) const;
    /*!
     * \brief complete the list of material properties.
     * This method shall be called by the buildMaterialPropertiesList
     * after that the mandatory material properties list have been
     * filled.
     * \param[out] mprops : list of material properties
     * \param[in]  mb     : mechanical behaviour description
     * \param[in]  h      : modelling hypothesis
     */
    virtual void
    completeMaterialPropertiesList(std::vector<UMATMaterialProperty>&,
				   const MechanicalBehaviourDescription&,
				   const Hypothesis) const;
    /*!
     * \return the name of the function generated by the Cyrano interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string
    getFunctionName(const std::string&) const = 0;
    /*!
     * \return a name used to create symbols for the UMAT++ interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     * \param[in] h : modelling hypothesis
     */
    virtual std::string
    getSymbolName(const std::string&,
		  const Hypothesis) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  h      : modelling hypothesis
     * \param[in]  mb     : mechanical behaviour description
     * \param[in]  v      : variables to be initialized
     * \param[in]  iv     : indirection vector
     * \param[in]  o      : offset in the indirection vector
     * \param[in]  src    : name of the array from which the variables are initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     */
    virtual void
    writeMaterialPropertiesInitializersInBehaviourDataConstructorI(std::ostream&,
								   const Hypothesis,
								   const MechanicalBehaviourDescription&,
								   const std::vector<UMATMaterialProperty>&,
								   const SupportedTypes::TypeSize,
								   const std::string&,
								   const std::string&,
								   const std::string&) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  h      : modelling hypothesis
     * \param[in]  mb     : mechanical behaviour description
     * \param[in]  v      : variables to be initialized
     * \param[in]  iv     : indirection vector
     * \param[in]  o      : offset in the indirection vector
     * \param[in]  src    : name of the array from which the variables are initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     */
    virtual void
    writeMaterialPropertiesInitializersInBehaviourDataConstructorII(std::ostream&,
								    const Hypothesis,
								    const MechanicalBehaviourDescription&,
								    const std::vector<UMATMaterialProperty>&,
								    const SupportedTypes::TypeSize,
								    const std::string&,
								    const std::string&,
								    const std::string&) const;
    /*!
     * \param[out] f    : output stream
     * \param[in]  name : name of the umat function
     * \param[in]  h    : modelling hypothesis
     * \param[in]  v    : variables external names and array size
     * \param[in]  t    : class of variable treated used as the name for the generated symbol (for example,  'InternalStateVariables')
     * \param[in]  o    : offset
     */
    virtual void
    writeExternalNames(std::ostream&,
		       const std::string&,
		       const Hypothesis&,
		       const std::vector<std::string>&,
		       const std::string&) const;

    virtual void
    checkParametersType(bool&,
			bool&,
			bool&,
			const VariableDescriptionContainer&) const;
    /*!
     * \return the header define
     */
    virtual std::string
    getHeaderDefine(const MechanicalBehaviourDescription&) const;

    virtual void
    getExtraSrcIncludes(std::ostream&,
			const MechanicalBehaviourDescription&) const;

    virtual void
    writeVisibilityDefines(std::ostream&) const;

    virtual void
    writeSetParametersFunctionsDeclarations(std::ostream&,
					    const std::string&,
					    const MechanicalBehaviourDescription&) const;

    virtual void
    writeSetParametersFunctionsImplementations(std::ostream&,
					       const std::string&,
					       const MechanicalBehaviourDescription&) const;

    /*!
     * write the first part of the code used to generate a MTest file
     */
    virtual void
    generateMTestFile1(std::ostream&) const;

    /*!
     * write the second part of the code used to generate a MTest file
     */
    virtual void
    generateMTestFile2(std::ostream&,
    		       const MechanicalBehaviourDescription::BehaviourType,
    		       const std::string&,
    		       const std::string&,
    		       const MechanicalBehaviourDescription&) const;

    virtual void
    writeMTestFileGeneratorSetRotationMatrix(std::ostream&,
					     const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    writeUMATxxSpecificSymbols(std::ostream&,
			       const std::string&,
			       const MechanicalBehaviourDescription&,
			       const MFrontFileDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    generateUMATxxGeneralSymbols(std::ostream&,
				 const std::string&,
				 const MechanicalBehaviourDescription&,
				 const MFrontFileDescription &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    generateUMATxxSymbols(std::ostream&,
			  const std::string&,
			  const Hypothesis,
			  const MechanicalBehaviourDescription&,
			  const MFrontFileDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxBehaviourTypeSymbols(std::ostream&,
				    const std::string&,
				    const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    writeUMATxxSourceFileSymbols(std::ostream&,
				 const std::string&,
				 const MechanicalBehaviourDescription&,
				 const MFrontFileDescription&) const;
    /*!
     * \param[in] out   : output file
     * \param[in] name  : name of the behaviour as defined by interface
     *                    (generally taking into account the material
     *                     and the behaviour name)
     * \param[in] h     : modelling hypothesis
     * \param[in] mb    : behaviour description
     */
    virtual void
    writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(std::ostream&,
							 const std::string&,
							 const Hypothesis,
							 const MechanicalBehaviourDescription&) const;
    /*!
     * \brief write UMAT++ symbols associated with supported modelling hypothesis
     * \param[in] out : output file
     * \param[in] n   : name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     * \param[in] mb  : behaviour description
     */
    virtual void
    writeUMATxxSupportedModellingHypothesis(std::ostream&,
					    const std::string&,
					    const MechanicalBehaviourDescription&) const;
    /*!
     * \brief write UMAT++ symbols associated with material properties for the given hypothesis
     * \param[in] out : output file
     * \param[in] n   : name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     * \param[in] h   : modelling hypothesis
     * \param[in] mb  : behaviour description
     */
    virtual void
    writeUMATxxMaterialPropertiesSymbols(std::ostream&,
					 const std::string&,
					 const Hypothesis,
					 const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour 
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxStateVariablesSymbols(std::ostream&,
				     const std::string&,
				     const Hypothesis,
				     const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour 
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxExternalStateVariablesSymbols(std::ostream&,
					     const std::string&,
					     const Hypothesis,
					     const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour 
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxRequirementsSymbols(std::ostream&,
				   const std::string&,
				   const Hypothesis,
				   const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxSymmetryTypeSymbols(std::ostream&,
				    const std::string&,
				    const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     */
    virtual void
    writeUMATxxElasticSymmetryTypeSymbols(std::ostream&,
					   const std::string&,
					   const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    writeUMATxxAdditionalSymbols(std::ostream&,
				 const std::string&,
				 const Hypothesis,
				 const MechanicalBehaviourDescription&,
				 const MFrontFileDescription&) const = 0;
    /*!
     * \param[in] out : output file
     */
    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const = 0;

    /*!
     * \brief associates each distinct modelling hypothesis to appropriate tests
     * \param[in] mb : behaviour description
     */
    virtual std::map<Hypothesis,std::string>
    gatherModellingHypothesesAndTests(const MechanicalBehaviourDescription&) const;
    /*!
     * \return true if the interface handles the given modelling hypothesis
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : mechanical behaviour description
     */
    virtual bool
    isModellingHypothesisHandled(const Hypothesis,
				 const MechanicalBehaviourDescription&) const;
    
    virtual std::string
    getModellingHypothesisTest(const Hypothesis) const = 0;

    bool generateMTestFile;
    
  }; // end of struct MFrontUMATInterfaceBase

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTUMATINTERFACEBASE_H */

