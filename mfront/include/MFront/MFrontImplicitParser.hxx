/*!
 * \file   MFrontImplicitParser.hxx
 * \brief  This file declares the MFrontImplicitParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTIMPLICITPARSER_H_
#define _LIB_MFRONTIMPLICITPARSER_H_ 

#include<string>
#include"MFront/MFrontBehaviourParserBase.hxx"
#include"MFront/MFrontVirtualParser.hxx"

namespace mfront{

  struct MFrontImplicitParser
    : public MFrontVirtualParser,
      public MFrontBehaviourParserBase<MFrontImplicitParser>
  {
    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontImplicitParser();

    void setVerboseMode();

    void setDebugMode();

    void setWarningMode();

    void treatFile(const std::string&);

    void writeOutputFiles();

    void
    setInterfaces(const std::set<std::string>&);

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

    ~MFrontImplicitParser();

  private:

    enum Algorithm{
      NEWTONRAPHSON,
      NEWTONRAPHSON_NR,
      BROYDEN,
      BROYDEN2,
      DEFAULT
    };

    std::string
    variableModifier1(const std::string&,const bool);

    std::string
    variableModifier2(const std::string&,const bool);

    virtual void
    treatUnknownVariableMethod(const std::string&);

    void endsInputFileProcessing(void);

    void writeBehaviourIntegrator(void);

    void writeBehaviourParserSpecificIncludes(void);

    void writeBehaviourParserSpecificTypedefs(void);

    void writeBehaviourParserSpecificMembers(void);

    void writeBehaviourStateVarsIncrements(void);
    
    void writeBehaviourConstructors(void);

    void writeBehaviourParserSpecificConstructorPart(void);

    void writeBehaviourStaticVars(void);

    void writeComputeNumericalJacobian(void);

    void writeBehaviourComputeTangentOperator(void);

    void writeGetPartialJacobianInvert(void);

    void treatTheta(void);

    void treatIterMax(void);

    void treatEpsilon(void);

    void treatAlgorithm(void);

    void treatPredictor(void);

    void treatComputeStress(void);

    void treatStateVariables(void);

    void treatAuxiliaryStateVariables(void);

    void treatCompareToNumericalJacobian(void);

    void treatJacobianComparisonCriterium(void);

    void generateOutputFiles(void);

    void treatUseAcceleration(void);

    void treatAccelerationTrigger(void);

    void treatAccelerationPeriod(void);

    void treatUseRelaxation(void);

    void treatRelaxationTrigger(void);
    
    void treatRelaxationCoefficient(void);

    void treatInitJacobian(void);

    void treatTangentOperator(void);

    std::string
    getJacobianPart(const VarHandler&,
		    const VarHandler&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const std::string& = "this->jacobian",
		    const std::string& = "");

    friend class MFrontBehaviourParserBase<MFrontImplicitParser>;

    // error normalisation factors
    std::map<std::string,std::string> enf;

    std::string computeStress;

    std::string computeFinalStress;

    std::string predictor;

    std::string initJacobian;

    std::string tangentOperator;

    Algorithm algorithm;

    bool compareToNumericalJacobian;

    bool useRelaxation;

    bool useAcceleration;

  }; // end of struct MFrontImplicitParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSER_H */

