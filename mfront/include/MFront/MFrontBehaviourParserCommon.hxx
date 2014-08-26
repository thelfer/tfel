/*!
 * \file   MFrontBehaviourParserCommon.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 mai 2008
 */

#ifndef _LIB_MFRONTBEHAVIOURPARSERCOMMON_HXX_
#define _LIB_MFRONTBEHAVIOURPARSERCOMMON_HXX_ 

#include<map>
#include<vector>
#include<set>
#include<string>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/SupportedTypes.hxx"
#include"MFront/ParserBase.hxx"
#include"MFront/MechanicalBehaviourDescription.hxx"

#include"MFront/MFrontVirtualParser.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"

namespace mfront{


  /*!
   * This class provides most functionnalities used by mechanical
   * behaviour parsers.
   *
   */
  struct TFEL_VISIBILITY_EXPORT MFrontBehaviourParserCommon
    : public MFrontVirtualParser,
      public ParserBase,
      public SupportedTypes
  {

    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);
	
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    virtual std::vector<std::string>
    getGeneratedIncludes(void);

    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);
    
    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(void);

  protected:
       
    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef tfel::material::ModellingHypothesis::Hypothesis Hypothesis;

    /*!
     * create a variable modifier from a method
     */
    template<typename T,typename T2>
    struct TFEL_VISIBILITY_LOCAL StandardVariableModifier
      : public VariableModifier
    {
      //! a simple alias
      typedef std::string (T2::* MPtr)(const Hypothesis,
				       const std::string&,
				       const bool);
      /*!
       * constructor
       */
      StandardVariableModifier(T&,
			       const Hypothesis,
			       const MPtr);
      /*!
       * \param[in] v : the variable name
       * \param[in] b : true if "this" shall be added
       */
      std::string
	exe(const std::string&,
	    const bool);
      /*!
       * destructor
       */
      ~StandardVariableModifier();
      
    private:
      
      T& instance;
      const Hypothesis hypothesis;
      const MPtr mptr;
    };
    /*!
     * create a standard variable modifier from an instance of a class
     * and a pointer to a member
     */
    template<typename T,typename T2>
    tfel::utilities::shared_ptr<VariableModifier>
    makeVariableModifier(T&,
			 const Hypothesis,
			 std::string (T2::*)(const Hypothesis,
					     const std::string&,
					     const bool));
    template<typename T,typename T2>
    struct TFEL_VISIBILITY_LOCAL StandardWordAnalyser
      : public WordAnalyser
    {
      //! a simple alias
      typedef void (T2::* MPtr)(const Hypothesis,
				const std::string&);
      /*!
       * constructor
       */
      StandardWordAnalyser(T&,const Hypothesis,
			   const MPtr);
      /*!
       * \param[in] k : the current word
       */
      void
      exe(const std::string&);
      /*!
       * destructor
       */
      ~StandardWordAnalyser();
      
    private:
      
      T& instance;
      const MPtr mptr;
      const Hypothesis hypothesis;
    };
    /*!
     * create a standard variable modifier from an instance of a class
     * and a pointer to a member
     */
    template<typename T,typename T2>
    tfel::utilities::shared_ptr<WordAnalyser>
    makeWordAnalyser(T&,const Hypothesis,
		     void (T2::*)(const Hypothesis,
				  const std::string&));
    /*!
     * option to code blocks
     */
    struct CodeBlockOptions
    {
      //! a simple alias
      typedef MechanicalBehaviourData::Mode Mode;
      //! a simple alias
      typedef MechanicalBehaviourData::Position Position;
      //! constructor
      CodeBlockOptions();
      //! position where the code block will be inserted (body by defaut)
      Position p;
      //! insertion mode (create or append by default)
      Mode m;
      //! list of hypothesis
      std::set<Hypothesis> hypotheses;
      //! list of untreated options
      std::vector<tfel::utilities::Token> untreated;
    };
    /*!
     * \return a string describing the behaviour type
     */
    virtual std::string
    convertBehaviourTypeToString(void) const;
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName(void) const;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    virtual void addMaterialLaw(const std::string&);
    /*!
     * \brief append the given code to the includes
     */
    virtual void appendToIncludes(const std::string&);
    /*!
     * \brief append the given code to the members
     */
    virtual void appendToMembers(const std::string&);
    /*!
     * \brief append the given code to the private code
     */
    virtual void appendToPrivateCode(const std::string&);
    /*!
     * \brief append the given code to the sources
     */
    virtual void appendToSources(const std::string&);
    /*!
     * \param[out] o : options to be read
     * \param[in]  s : allow specialisation
     */
    void readCodeBlockOptions(CodeBlockOptions&,
			      const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] child : a pointer to this
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] b     : add "this->" in front of variables
     * \param[in] s     : allow specialisation
     */
    template<typename T,
	     typename T2>
    CodeBlockOptions
    readCodeBlock(T&,
		  const std::string&,
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  const bool,const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] child : a pointer to this
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] b     : add "this->" in front of variables
     */
    template<typename T,
	     typename T2>
    void
    readCodeBlock(T&,
		  const CodeBlockOptions&,
		  const std::string&,
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] child : a pointer to this
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] a     : word analyser
     * \param[in] b     : add "this->" in front of variables
     * \param[in] s     : allow specialisation
     */
    template<typename T,
	     typename T2,
	     typename T3>
    CodeBlockOptions
    readCodeBlock(T&,
		  const std::string&,
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  void (T3::*)(const Hypothesis,
			       const std::string&),
		  const bool,const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] child : a pointer to this
     * \param[in] n     : name of the method read
     * \param[in] m     : modifier
     * \param[in] a     : word analyser
     * \param[in] b     : add "this->" in front of variables
     * \param[in] s     : allow specialisation
     */
    template<typename T,
	     typename T2,
	     typename T3>
    void
    readCodeBlock(T&,
		  const CodeBlockOptions&,
		  const std::string&,
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  void (T3::*)(const Hypothesis,
			       const std::string&),
		  const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] child : a pointer to this
     * \param[in] n1    : name of the first method read
     * \param[in] n2    : name of the second method read
     * \param[in] m1    : modifier
     * \param[in] m2    : modifier
     * \param[in] b     : add "this->" in front of variables
     * \param[in] s     : allow specialisation
     */
    template<typename T,
	     typename T2>
    CodeBlockOptions
    readCodeBlock(T&,
		  const std::string&,
		  const std::string&,
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  const bool,const bool);
    /*!
     * \brief read the next code block and adds it tho the mechanical
     * behaviour
     * \param[in] child : a pointer to this
     * \param[in] n1    : name of the first method read
     * \param[in] n2    : name of the second method read
     * \param[in] m1    : modifier
     * \param[in] m2    : modifier
     * \param[in] b     : add "this->" in front of variables
     */
    template<typename T,
	     typename T2>
    void
    readCodeBlock(T&,
		  const CodeBlockOptions&,
		  const std::string&,
		  const std::string&,
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  std::string (T2::*)(const Hypothesis,
				      const std::string&,
				      const bool),
		  const bool);
    /*!
     * \brief throw an exception is some options were not recognized
     */
    void
    treatUnsupportedCodeBlockOptions(const CodeBlockOptions&);
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void
    addStaticVariableDescription(const StaticVariableDescription&);
    /*!
     * disable the declaration of new variables
     * \param[in] h : modelling hypothesis
     */
    virtual void
    disableVariableDeclaration(const Hypothesis);
    /*!
     * write the output files
     */
    virtual void
    writeOutputFiles(void);
    /*!
     * \brief : an helper function to declare a timer
     * \param[out] os : output stream
     * \param[in]  v  : code block name
     * \param[in]  s  : time temporary variable suffix
     */
    virtual void
    writeStandardPerformanceProfiling(std::ostream&,
				      const std::string&,
				      const std::string& = "");
    /*!
     * \brief : an helper function writing the beginning of a
     * performance measurement
     * \param[out] os : output stream
     * \param[in]  v  : code block name
     * \param[in]  s  : time temporary variable suffix
     */
    virtual void
    writeStandardPerformanceProfilingBegin(std::ostream&,
					   const std::string&,
					   const std::string& = "");
    /*!
     * \brief : an helper function writing the end of a
     * performance measurement
     * \param[out] os : output stream
     */
    virtual void
    writeStandardPerformanceProfilingEnd(std::ostream&);
    /*!
     * \return the list of hypothesis a priori supported by
     * the parser.
     *
     * To enable other hypothesis or restrict the hypotheses
     * supported, the user must use the @ModellingHypothesis or
     * @ModellingHypotheses keywords.
     */
    virtual std::set<Hypothesis>
    getDefaultModellingHypotheses(void) const;
    /*!
     * \return true if the given modelling hypothesis is handled by
     * the parser
     *
     * Some parsers have restrictions on the modelling hypotheses
     * supported. For example, the isotropic behaviours were not able
     * to handle plane stress hypotheses when this comment was
     * written(but it was planned, so this comment may be outdated
     * now).
     *
     * The fact that this method returns true means that the user
     * *can* provide code to support this modelling hypothesis. For
     * example, to support plane stress in RungeKutta and Implicit
     * parsers, the user must provide some hand-crafted code. He must
     * enable this modelling hypothesis by calling explicitely
     * @ModellingHypothesis or @ModellingHypotheses keywords.
     */
    virtual bool
    isModellingHypothesisSupported(const Hypothesis) const;
    /*!
     *
     */
    virtual void
    endsInputFileProcessing();
    /*!
     * \brief the standard variable modifier
     * \param[in] h : modelling hypothesis
     * \param[in] v : variable name
     * \param[in] b : if true, shall add the "this" qualifier
     */
    virtual std::string
    standardModifier(const Hypothesis,
		     const std::string&,
		     const bool);
    
    virtual std::string
    predictionOperatorVariableModifier(const Hypothesis,
				       const std::string&,const bool);

    virtual std::string
    tangentOperatorVariableModifier(const Hypothesis,
				    const std::string&,
				    const bool);
    virtual void
    readStringList(std::vector<std::string>&);
    /*!
     * read a list of hypotheses
     * \param[out] h : list of hypotheses
     * \note by default, the returning set contains UNDEFINEDHYPOTHESIS
     */
    virtual void
    readHypothesesList(std::set<Hypothesis>&);
    /*!
     * \brief append the given modelling hypothesis to the set of hypothesis
     * \param[out] h : list of hypotheses
     * \param[in]  v : hypothesis to be inserted
     */
    void
    appendToHypothesesList(std::set<Hypothesis>&,
			   const std::string&) const;
    /*!
     * First read a set of Hypothesis. Then read a list variables and
     * assign them to mechanical data associated with those hypotheses.
     * \param[out] v  : the declared variables
     * \param[out] h  : modelling hypothesis on which the variables were declared
     * \param[in]  m  : method used to assign the variables
     * \param[in]  b1 : allow arrays of variables to be defined
     * \param[in]  b2 : for each variable read, add another variable
     *                  standing for the first variable increment
     * \param[in]  b3 : if true, allows variable declaration after that
     *                  a code block has been defined (this is a
     *                  priori only valid for local variables)
     */
    virtual void
    readVariableList(VariableDescriptionContainer&,
		     std::set<Hypothesis>&,
		     void (MechanicalBehaviourDescription::*)(const Hypothesis,
							      const VariableDescriptionContainer&),
		     const bool,const bool,const bool);

    /*!
     * Assign a list variables to mechanical data associated with the given hypotheses.
     * \param[out] h : modelling hypothesis on which the variables were declared
     * \param[out] v : the declared variables
     * \param[in]  m : method used to assign the variables
     * \param[in]  b : if true, allows variable declaration after that
     *                  a code block has been defined (this is a
     *                  priori only valid for local variables)
     */
    virtual void
    addVariableList(const std::set<Hypothesis>&,
		    const VariableDescriptionContainer&,
		    void (MechanicalBehaviourDescription::*)(const Hypothesis,
							     const VariableDescriptionContainer&),
		    const bool);

    /*!
     * set the interfaces to be used
     */
    virtual void
    setInterfaces(const std::set<std::string>&);
    /*!
     * set the analysers to be used
     */
    virtual void
    setAnalysers(const std::set<std::string>&);
    /*!
     * register the default variable names
     */
    virtual void
    registerDefaultVarNames(void);
    /*!
     * treat the @TangentOperator keyword
     */
    virtual void treatTangentOperator(void);
    /*!
     * treat the @IsTangentOperatorSymmetric keyword
     */
    virtual void treatIsTangentOperatorSymmetric(void);
    /*!
     * treat the @Material keyword
     */
    virtual void
    treatMaterial(void);
    /*!
     * treat the @Library keyword
     */
    virtual void
    treatLibrary(void);
    /*!
     * treat the @Profiling keyword
     */
    virtual void
    treatProfiling(void);

    virtual void
    treatModellingHypothesis(void);

    virtual void
    treatModellingHypotheses(void);
    
    virtual void
      treatUnknownKeyword(void);
    
    virtual void
      treatUpdateAuxiliaryStateVariables(void);
    
    virtual void
      treatUsableInPurelyImplicitResolution(void);

    virtual void
      treatParameter(void);

    virtual void
      treatLocalVar(void);

    virtual void
    treatComputeThermalExpansion(void);

    virtual void
      treatInitLocalVariables(void);

    virtual void
      treatOrthotropicBehaviour(void);

    virtual void
      treatIsotropicElasticBehaviour(void);

    virtual void
      treatIsotropicBehaviour(void);

    virtual void
    treatRequireStiffnessOperator(void);

    virtual void
    treatRequireStiffnessTensor(void);

    virtual void
    treatRequireThermalExpansionCoefficientTensor(void);

    virtual void
      treatBehaviour(void);

    virtual void
      treatInterface(void);

    virtual void
    treatStateVariable(void);

    virtual void
    treatAuxiliaryStateVariable(void);  

    virtual void
      treatExternalStateVariable(void);

    virtual void
      treatIntegrator(void);

    virtual void
      treatCoef(void);

    virtual void
      treatUseQt(void);

    virtual void
      treatBounds(void);

    virtual void
      treatPhysicalBounds(void);

    virtual void
    treatPredictionOperator(void);
    /*!
     * \param[out] d : bounds description
     * \param[in]  h : modelling hypothesis
     */
    virtual void
    treatBounds(BoundsDescription&,
		const Hypothesis);

    virtual void
    writeIncludes(std::ofstream&);

    virtual void
    writeNamespaceBegin(std::ofstream&);
    
    virtual void
    writeNamespaceEnd(std::ofstream&);

    virtual void
    writeIntegerConstants(std::ofstream&);
    
    virtual void
    writeStandardTFELTypedefs(std::ofstream&); 
    
    virtual void
    checkBehaviourDataFile(void) const;
    
    virtual void
    writeBehaviourDataStandardTFELTypedefs(void);
    
    virtual void
    writeBehaviourDataStandardTFELIncludes(void);
    
    virtual void
    writeBehaviourDataFileHeader(void);
    
    virtual void
    writeBehaviourDataFileHeaderBegin(void);
    
    virtual void
    writeBehaviourDataFileHeaderEnd(void);
    
    virtual void
    writeBehaviourDataClassHeader(void);
    
    virtual void
    writeBehaviourDataGetName(void);
    
    virtual void
    writeBehaviourDataDisabledConstructors(void);
    
    virtual void
    writeBehaviourDataConstructors(const Hypothesis);
    /*!
     * write interface's setters for the main variables
     */
    virtual void
    writeBehaviourDataMainVariablesSetters(void);
    
    virtual void
    writeBehaviourDataClassBegin(const Hypothesis);
    
    virtual void
    writeBehaviourDataClassEnd(void);
    
    virtual void
    writeBehaviourDataDefaultMembers(void);

    virtual void
    writeBehaviourDataMaterialProperties(const Hypothesis);

    virtual void
    writeBehaviourDataStateVariables(const Hypothesis);

    virtual void
    writeBehaviourDataAssignementOperator(const Hypothesis);
    
    virtual void
    writeBehaviourDataOutputOperator(const Hypothesis);

    virtual void
    writeBehaviourDataExport(const Hypothesis);

    virtual void
    writeBehaviourDataPublicMembers(void);

    virtual void
    writeBehaviourDataFileBegin(void);

    virtual void
    writeBehaviourDataFileEnd(void);

    virtual void
    writeBehaviourDataClass(const Hypothesis);
    
    virtual void
    writeBehaviourDataForwardDeclarations(void);

    virtual void
    checkIntegrationDataFile(void) const;

    virtual void
    writeIntegrationDataStandardTFELTypedefs(void);

    virtual void
    writeIntegrationDataStandardTFELIncludes(void);

    virtual void
    writeIntegrationDataFileHeader(void);

    virtual void
    writeIntegrationDataFileHeaderBegin(void);

    virtual void
    writeIntegrationDataFileHeaderEnd(void);

    virtual void
    writeIntegrationDataClassHeader(void);

    virtual void
    writeIntegrationDataGetName(void);

    virtual void
    writeIntegrationDataDisabledConstructors(void);

    virtual void
    writeIntegrationDataConstructors(const Hypothesis);
    /*!
     * write interface's setters for the main variables
     */
    virtual void
    writeIntegrationDataMainVariablesSetters(void);

    virtual void
    writeIntegrationDataScaleOperators(const Hypothesis);

    virtual void
    writeIntegrationDataClassBegin(const Hypothesis);

    virtual void
    writeIntegrationDataClassEnd(void);

    virtual void
    writeIntegrationDataDefaultMembers(void);

    virtual void
    writeIntegrationDataExternalStateVariables(const Hypothesis);

    virtual void
    writeIntegrationDataFileBegin(void);

    virtual void
    writeIntegrationDataFileEnd(void);

    virtual void
    writeIntegrationDataClass(const Hypothesis);

    virtual void
    writeIntegrationDataForwardDeclarations(void);

    virtual void
    writeIntegrationDataOutputOperator(const Hypothesis);

    void checkBehaviourFile(void) const;

    virtual void
    writeBehaviourStandardTFELTypedefs(void);

    virtual void
    writeBehaviourFileHeader(void);

    virtual void
    writeBehaviourFileHeaderBegin(void);

    virtual void
    writeBehaviourFileHeaderEnd(void);
    
    virtual void
    writeBehaviourFileBegin(void);

    virtual void
    writeBehaviourFileEnd(void);

    virtual void
    writeBehaviourClass(const Hypothesis);

    virtual void
    writeBehaviourForwardDeclarations(void);

    virtual void
    writeBehaviourProfiler(void);

    virtual void
    writeBehaviourParserSpecificInheritanceRelationship(void);

    virtual void
    writeBehaviourParserSpecificTypedefs(void);

    virtual void
    writeBehaviourParserSpecificMembers(const Hypothesis);

    virtual void
    writeBehaviourParserSpecificIncludes(void);

    virtual void
    writeBehaviourClassBegin(const Hypothesis);

    virtual void
    writeBehaviourGetModellingHypothesis(void);

    virtual void
    writeBehaviourClassEnd(void);

    virtual void
    writeBehaviourGetName(void);

    virtual void
    writeBehaviourPolicyVariable(void);

    virtual void
    writeBehaviourSetOutOfBoundsPolicy(void);

    virtual void
    writeBehaviourCheckBounds(const Hypothesis);

    virtual void
    writeBehaviourDisabledConstructors(void);

    virtual void
    writeBehaviourConstructors(const Hypothesis);

    /*!
     * \return behaviour constructor initializers.
     */
    virtual std::string
    getBehaviourConstructorsInitializers(const Hypothesis);
    /*!
     * write the behaviour's compouteStressFreeExpansion method, if
     * mandatory.
     */
    virtual void
    writeBehaviourComputeStressFreeExpansion(void);
    /*!
     * write the initalize method . This method is called after that
     * the main variables were set.
     */
    virtual void
    writeBehaviourInitializeMethod(const Hypothesis);
    /*!
     * write part of the constructor specific to the parser
     * \param[in] h : modelling hypothesis
     */
    virtual void
    writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis);

    virtual void
    writeBehaviourIntegrationVariablesIncrements(const Hypothesis);

    virtual void
    writeBehaviourLocalVariables(const Hypothesis);

    virtual void
    writeBehaviourIntegrationVariables(const Hypothesis);

    virtual void
    writeBehaviourParameters(const Hypothesis);

    virtual void
    writeBehaviourStaticVariables(const Hypothesis);

    virtual void
    writeBehaviourMembersFunc(void);

    virtual void
    writeBehaviourPrivate(void);

    virtual void
    writeBehaviourUpdateIntegrationVariables(const Hypothesis);

    virtual void
    writeBehaviourUpdateStateVariables(const Hypothesis);

    virtual void
    writeBehaviourUpdateAuxiliaryStateVariables(const Hypothesis);

    virtual void
    writeBehaviourIntegrator(const Hypothesis);

    virtual void
    writeBehaviourGetTimeStepScalingFactor(void);

    virtual void
    writeBehaviourUpdateExternalStateVariables(const Hypothesis);

    virtual void
    writeBehaviourOutputOperator(const Hypothesis);

    virtual void
    writeBehaviourDestructor(void);

    virtual void
    writeBehaviourTraits(void);

    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] b : true if the behaviour is defined for the given modelling hypothesis
     */
    virtual void
    writeBehaviourTraitsSpecialisation(const Hypothesis,
				       const bool);

    virtual void
    writeBehaviourIncludes(void);

    virtual void
    writeBehaviourLocalVariablesInitialisation(const Hypothesis);

    virtual void
    writeBehaviourParameterInitialisation(const Hypothesis);

    virtual void
    writeBehaviourParametersInitializers();

    virtual void
    writeBehaviourParametersInitializer(const Hypothesis);

    virtual void
    checkSrcFile(void) const;

    virtual void
    writeSrcFileHeader(void);

    virtual void
    writeSrcFileUserDefinedCode(void);

    virtual void
    writeSrcFileBehaviourProfiler(void);

    virtual void
    writeSrcFileParametersInitializers(void);

    virtual void
    writeSrcFileParametersInitializer(const Hypothesis);

    virtual void
    writeSrcFileStaticVariables(const Hypothesis);
    /*!
     * \brief write the source file
     */
    virtual void
    writeSrcFile(void);

    virtual void
    writeBehaviourComputePredictionOperator(const Hypothesis);

    virtual void
    writeBehaviourComputeTangentOperator(const Hypothesis);

    virtual void
    writeBehaviourGetTangentOperator();
    
    virtual void
    writeBehaviourTangentOperator();

    virtual void
    treatParameterMethod(const Hypothesis);
    
    /*!
     * \return true if the the given variable may have methods
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    virtual bool
    isCallableVariable(const Hypothesis,
		       const std::string&) const;
    /*!
     * treat a method
     * \param[in] h : modelling hypothesis
     */
    virtual void
    treatVariableMethod(const Hypothesis);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    virtual void
    treatUnknownVariableMethod(const Hypothesis,
			       const std::string&);

    virtual ~MFrontBehaviourParserCommon();

    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    void
    declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis,
									   const std::string&);
    /*!
     * analyse the inputs of a material property to check if it is
     * available either as a material property or as an external state
     * variable.
     *
     *  If one input is not found, an external state variable is
     * implicitely declared to allow the evaluation of this material
     * property.
     * 
     * \param[in] h  : modelling hypothesis
     * \param[in] mp : material property description
     */
    virtual void
    analyseMaterialProperty(const Hypothesis,
			    const MaterialPropertyDescription&);
    /*!
     * \brief if no tangent operator was provided, but that the
     * behaviour requires a stiffness matrix, this method creates a
     * minimal tangent operator for elasticity.
     * \note This method is not trivial because one has to take care
     * not to create artifical mechanical data specialisation
     * \note This method is meant to be used in the
     * endsInputFileProcessing method.
     */
    virtual void
    setMinimalTangentOperator(void);
    /*!
     * \brief if the compte final stress code is not available, create
     * it from the ComputeFinalStressCandidate code if it is
     * available.
     * \note This method is not trivial because one has to take care
     * not to create artifical mechanical data specialisation
     * \note This method is meant to be used in the
     * endsInputFileProcessing method.
     */
    virtual void
    setComputeFinalStressFromComputeFinalStressCandidateIfNecessary(void);
    /*!
     * \brief perform pedantic checks
     */
    virtual void
    doPedanticChecks(void) const;
    /*!
     * \return true if the user defined a block of code computing the
     * tangent operator
     * \param[in] h : modelling hypothesis
     */
    virtual bool
    hasUserDefinedTangentOperatorCode(const Hypothesis) const;
    /*!
     * constructor
     */
    MFrontBehaviourParserCommon();

    std::map<std::string,std::vector<std::string> > sourcesLibrairiesDependencies;

    std::set<std::string> registredKeyWords;

    std::vector<std::string> interfaces;

    std::vector<std::string> analysers;

    /*!
     * local variables initalizers. This variable to initialize local
     * variables defined by domains specific languages and shall not
     * be accessible to the end user.
     */
    std::string localVariablesInitializers;

    std::string behaviourFileName;
    std::string behaviourDataFileName;
    std::string integrationDataFileName;
    std::string srcFileName;

    std::ofstream behaviourFile;
    std::ofstream behaviourDataFile;
    std::ofstream integrationDataFile;
    std::ofstream srcFile;
    
    bool useStateVarTimeDerivative;
    bool explicitlyDeclaredUsableInPurelyImplicitResolution;

    MechanicalBehaviourDescription mb;

  }; // end of struct MFrontBehaviourParserCommon

} // end of namespace mfront  

#include"MFront/MFrontBehaviourParserCommon.ixx"

#endif /* _LIB_MFRONTBEHAVIOURPARSERCOMMON_HXX */

