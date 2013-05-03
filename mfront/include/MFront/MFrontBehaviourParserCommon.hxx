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

#include"MFront/SupportedTypes.hxx"
#include"MFront/VarHandler.hxx"
#include"MFront/StaticVarHandler.hxx"
#include"MFront/ComputedVarHandler.hxx"
#include"MFront/BehaviourCharacteristic.hxx"
#include"MFront/UnaryLoadingDefinition.hxx"
#include"MFront/BoundsDescription.hxx"
#include"MFront/ParserBase.hxx"

#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"

namespace mfront{

  typedef std::vector<std::string> StringContainer;

  struct TFEL_VISIBILITY_EXPORT MFrontBehaviourParserCommon
    : public ParserBase,
      public SupportedTypes
  {

    virtual void
    setVerboseMode(void);

    virtual void
    setDebugMode(void);

    virtual void
    setWarningMode(void);

  protected:
       
    virtual std::string
    standardModifier(const std::string&,const bool);

    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    std::vector<std::string>
    getGeneratedIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);
    
    std::map<std::string,
	     std::pair<std::vector<std::string>,
		       std::vector<std::string> > >
    getSpecificTargets(void);
 
    bool
    isInternalStateVariable(const std::string&);
    
    bool
    isInternalStateVariableIncrement(const std::string&);
    
    bool
    isExternalStateVariable(const std::string&);
    
    virtual void
    readStringList(StringContainer&);
    
    void
      setInterfaces(const std::set<std::string>&);

    virtual void
      registerDefaultVarNames(void);
    
    virtual void
      treatUnknownKeyword(void);
    
    virtual void
      treatUpdateAuxiliaryStateVars(void);
    
    virtual void
      treatUsableInPurelyImplicitResolution(void);

    virtual void
      treatMaterial(void);

    virtual void
      treatParameter(void);

    virtual void
      treatLocalVar(void);

    virtual void
      treatInitLocalVars(void);

    virtual void
      treatOrthotropicBehaviour(void);

    virtual void
      treatIsotropicElasticBehaviour(void);

    virtual void
      treatIsotropicBehaviour(void);

    virtual void
      treatRequireStiffnessTensor(void);

    virtual void
      treatRequireThermalExpansionTensor(void);

    virtual void
      treatBehaviour(void);

    virtual void
      treatExternalStateVariables(void);

    virtual void
      treatInterface(void);

    virtual void
      treatStateVariables(void);

    virtual void
      treatAuxiliaryStateVariables(void);  

    virtual void
      treatComputedVar(void);

    virtual void
      treatIntegrator(void);

    virtual void
      treatCoef(void);

    virtual void
      treatUseQt(void);

    virtual void
      treatUnaryLoadingTest(void);

    virtual void
      treatBounds(void);

    virtual void
      treatPhysicalBounds(void);

    virtual void
      treatBounds(BoundsDescription&);

    virtual void
      writeIncludes(std::ofstream&);

    virtual void writeNamespaceBegin(std::ofstream&);
    
    virtual void writeNamespaceEnd(std::ofstream&);
    
    virtual void writeStandardTFELTypedefs(std::ofstream&); 
    
    virtual void checkBehaviourDataFile(void) const;
    
    virtual void writeBehaviourDataStandardTFELTypedefs(void);
    
    virtual void writeBehaviourDataStandardTFELIncludes(void);
    
    virtual void writeBehaviourDataFileHeader(void);
    
    virtual void writeBehaviourDataFileHeaderBegin(void);
    
    virtual void writeBehaviourDataFileHeaderEnd(void);
    
    virtual void writeBehaviourDataClassHeader(void);
    
    virtual void writeBehaviourDataGetName(void);
    
    virtual void writeBehaviourDataDisabledConstructors(void);
    
    virtual void writeBehaviourDataConstructors(void);
    
    virtual void writeBehaviourDataClassBegin(void);
    
    virtual void writeBehaviourDataClassEnd(void);
    
    virtual void writeBehaviourDataDefaultMembers(void);

    virtual void writeBehaviourDataCoefs(void);

    virtual void writeBehaviourDataStateVars(void);

    virtual void writeBehaviourDataAssignementOperator(void);
    
    virtual void writeBehaviourDataOutputOperator(void);

    virtual void writeBehaviourDataExport(void);

    virtual void writeBehaviourDataPublicMembers(void);

    virtual void writeBehaviourDataFile(void);
    
    void checkIntegrationDataFile(void) const;

    virtual void writeIntegrationDataStandardTFELTypedefs(void);

    virtual void writeIntegrationDataStandardTFELIncludes(void);

    virtual void writeIntegrationDataFileHeader(void);

    virtual void writeIntegrationDataFileHeaderBegin(void);

    virtual void writeIntegrationDataFileHeaderEnd(void);

    virtual void writeIntegrationDataClassHeader(void);

    virtual void writeIntegrationDataGetName(void);

    virtual void writeIntegrationDataDisabledConstructors(void);

    virtual void writeIntegrationDataConstructors(void);

    virtual void writeIntegrationDataScaleOperators(void);

    virtual void writeIntegrationDataClassBegin(void);

    virtual void writeIntegrationDataClassEnd(void);

    virtual void writeIntegrationDataDefaultMembers(void);

    virtual void writeIntegrationDataExternalStateVars(void);

    virtual void writeIntegrationDataFile(void);

    virtual void writeIntegrationDataOutputOperator(void);

    void checkBehaviourFile(void) const;

    virtual void writeBehaviourStandardTFELTypedefs(void);

    virtual void writeBehaviourFileHeader(void);

    virtual void writeBehaviourFileHeaderBegin(void);

    virtual void writeBehaviourFileHeaderEnd(void);
    
    virtual void writeBehaviourFile(void);

    virtual void writeBehaviourParserSpecificInheritanceRelationship(void);

    virtual void writeBehaviourParserSpecificTypedefs(void);

    virtual void writeBehaviourParserSpecificMembers(void);

    virtual void writeBehaviourParserSpecificIncludes(void);

    virtual void writeBehaviourParserSpecificConstructorPart(void);

    virtual void writeBehaviourClassBegin(void);

    virtual void writeBehaviourGetModellingHypothesis(void);

    virtual void writeBehaviourClassEnd(void);

    virtual void writeBehaviourGetName(void);

    virtual void writeBehaviourPolicyVariable(void);

    virtual void writeBehaviourSetOutOfBoundsPolicy(void);

    virtual void writeBehaviourCheckBounds(void);

    virtual void writeBehaviourDisabledConstructors(void);

    virtual void writeBehaviourConstructors(void);

    void writeBehaviourConstructors(const std::string&,
				    const std::string&,
				    const std::string& = "");

    virtual void writeBehaviourStateVarsIncrements(void);

    virtual void writeBehaviourLocalVars(void);

    virtual void writeBehaviourParameters(void);

    virtual void writeBehaviourComputedVars(void);

    virtual void writeBehaviourStaticVars(void);

    virtual void writeBehaviourMembersFunc(void);

    virtual void writeBehaviourPrivate(void);

    virtual void writeBehaviourUpdateStateVars(void);

    virtual void writeBehaviourUpdateAuxiliaryStateVars(void);

    virtual void writeBehaviourIntegrator(void);

    virtual void writeBehaviourGetTimeStepScalingFactor(void);

    virtual void writeBehaviourUpdateExternalStateVariables(void);

    virtual void writeBehaviourOutputOperator(void);

    virtual void writeBehaviourDestructor(void);

    virtual void writeBehaviourTraits(void);

    virtual void writeBehaviourIncludeBehaviourData(void);

    virtual void writeBehaviourParameterInitialisation(void);

    virtual void writeBehaviourParametersInitializer(void);

    virtual void checkSrcFile(void) const;

    virtual void writeSrcFileHeader(void);

    virtual void writeSrcFileUserDefinedCode(void);

    virtual void writeSrcFileParametersInitializer(void);

    virtual void writeSrcFileStaticVars(void);

    virtual void writeSrcFile(void);

    virtual void writeUnaryLoadingTestFiles(void);

    virtual void writeBehaviourComputePredictionOperator(void);

    virtual void writeBehaviourComputeTangentOperator(void);

    virtual void writeBehaviourGetTangentOperator();
    
    virtual void writeBehaviourTangentStiffnessOperator();

    virtual void treatParameterMethod(void);

    virtual void treatVariableMethod(void);

    /*!
     * \param n : variable name
     */
    virtual void treatUnknownVariableMethod(const std::string&);

    virtual ~MFrontBehaviourParserCommon();

    const VarHandler&
    getStateVariableHandler(const std::string&) const;

    /*!
     * \param n : variable name
     */
    void
      declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string&);

    void updateClassName();

    bool
      contains(const VarContainer&,
	       const std::string&) const;

    bool
      isMaterialPropertyName(const std::string&) const;

    bool
      isLocalVariableName(const std::string&) const;

    bool
      isParameterName(const std::string&) const;

    bool
      isInternalStateVariableName(const std::string&) const;

    bool
      isAuxiliaryInternalStateVariableName(const std::string&) const;

    bool
      isExternalStateVariableName(const std::string&) const;

    bool
      isExternalStateVariableIncrementName(const std::string&) const;

    bool
      isCoefficientName(void);

    MFrontBehaviourParserCommon();
    
    std::map<std::string,std::vector<std::string> > sourcesLibrairiesDependencies;

    std::set<std::string> registredKeyWords;

    std::map<std::string,std::string> glossaryNames;

    std::map<std::string,std::string> entryNames;

    std::map<std::string,double>            parametersDefaultValues;
    std::map<std::string,int>               iParametersDefaultValues;
    std::map<std::string,unsigned short>    uParametersDefaultValues;

    VarContainer coefsHolder;
    VarContainer stateVarsHolder;
    VarContainer auxiliaryStateVarsHolder;
    VarContainer externalStateVarsHolder;
    VarContainer localVarsHolder;
    VarContainer parametersHolder;
    ComputedVarContainer computedVars;

    StringContainer interfaces;
    std::vector<UnaryLoadingDefinition> unaryLoadingDefinitions;
    std::vector<BoundsDescription>      boundsDescriptions;
    std::vector<BoundsDescription>      physicalBoundsDescriptions;

    std::string initLocalVars;
    std::string integrator;
    std::string updateAuxiliaryStateVars;

    std::string behaviour;
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
    bool hasConsistantTangentOperator;
    bool isConsistantTangentOperatorSymmetric;
    bool hasPredictionOperator;
    bool hasTimeStepScalingFactor;

    BehaviourCharacteristic behaviourCharacteristic;

  }; // end of struct MFrontBehaviourParserCommon

} // end of namespace mfront  

#endif /* _LIB_MFRONTBEHAVIOURPARSERCOMMON_HXX */

