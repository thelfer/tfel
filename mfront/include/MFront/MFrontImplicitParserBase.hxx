/*!
 * \file   MFrontImplicitParserBase.hxx
 * \brief  This file declares the MFrontImplicitParserBase class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTIMPLICITPARSERBASE_H_
#define _LIB_MFRONTIMPLICITPARSERBASE_H_ 

#include<string>
#include"MFront/MFrontBehaviourParserBase.hxx"
#include"MFront/MFrontVirtualParser.hxx"

namespace mfront{

  struct MFrontImplicitParserBase
    : public MFrontVirtualParser,
      public MFrontBehaviourParserBase<MFrontImplicitParserBase>
  {

    MFrontImplicitParserBase();

    virtual void setVerboseMode();

    virtual void setDebugMode();

    virtual void setWarningMode();

    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    virtual void
    getKeywordsList(std::vector<std::string>&) const;

    virtual void treatFile(const std::string&);

    virtual void writeOutputFiles();

    virtual void
    setInterfaces(const std::set<std::string>&);

    virtual std::map<std::string,
		     std::vector<std::string> >
    getGlobalIncludes(void);

    virtual std::map<std::string,
	     std::vector<std::string> >
    getGlobalDependencies(void);

    virtual std::map<std::string,
		     std::vector<std::string> >
    getGeneratedSources(void);

    virtual std::vector<std::string>
    getGeneratedIncludes(void);

    virtual std::map<std::string,
		     std::vector<std::string> >
    getLibrariesDependencies(void);

    virtual std::map<std::string,
		     std::pair<std::vector<std::string>,
			       std::vector<std::string> > >
    getSpecificTargets(void);

    virtual ~MFrontImplicitParserBase();

  protected:

    enum Algorithm{
      NEWTONRAPHSON,
      NEWTONRAPHSON_NR,
      BROYDEN,
      BROYDEN2,
      DEFAULT
    };

    virtual bool
    isJacobianPart(const std::string&);

    virtual void
    predictorAnalyser(const std::string&);
    
    virtual void
    integratorAnalyser(const std::string&);

    virtual std::string
    tangentOperatorVariableModifier(const std::string&,const bool);

    virtual std::string
    integratorVariableModifier(const std::string&,const bool);

    virtual std::string
    computeStressVariableModifier1(const std::string&,const bool);

    virtual std::string
    computeStressVariableModifier2(const std::string&,const bool);

    virtual void
    treatUnknownVariableMethod(const std::string&);

    virtual void
    treatIntegrator(void);

    virtual void endsInputFileProcessing(void);

    virtual void writeBehaviourIntegrator(void);

    virtual void writeBehaviourParserSpecificIncludes(void);

    virtual void writeBehaviourParserSpecificTypedefs(void);

    virtual void writeBehaviourParserSpecificMembers(void);

    virtual void writeBehaviourStateVarsIncrements(void);
    
    virtual void writeBehaviourConstructors(void);

    virtual void writeBehaviourParserSpecificConstructorPart(void);

    virtual void writeBehaviourStaticVars(void);

    virtual void writeComputeNumericalJacobian(void);

    virtual void writeBehaviourComputeTangentOperator(void);

    virtual void writeGetPartialJacobianInvert(void);

    virtual void treatTheta(void);

    virtual void treatIterMax(void);

    virtual void treatEpsilon(void);

    virtual void treatPertubationValueForNumericalJacobianComputation(void);

    virtual void treatAlgorithm(void);

    virtual void treatPredictor(void);

    virtual void treatComputeStress(void);

    virtual void treatStateVariables(void);

    virtual void treatAuxiliaryStateVariables(void);

    virtual void treatCompareToNumericalJacobian(void);

    virtual void treatJacobianComparisonCriterium(void);

    virtual void generateOutputFiles(void);

    virtual void treatUseAcceleration(void);

    virtual void treatAccelerationTrigger(void);

    virtual void treatAccelerationPeriod(void);

    virtual void treatUseRelaxation(void);

    virtual void treatRelaxationTrigger(void);
    
    virtual void treatRelaxationCoefficient(void);

    virtual void treatInitJacobian(void);

    virtual void treatTangentOperator(void);

    virtual void treatIsTangentOperatorSymmetric(void);

    virtual void
    treatUnknownKeyword(void);

    std::string
    getJacobianPart(const VarHandler&,
		    const VarHandler&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const std::string& = "this->jacobian",
		    const std::string& = "");

    friend class MFrontBehaviourParserBase<MFrontImplicitParserBase>;

    std::set<std::string> jacobianPartsUsedInIntegrator;

    std::set<std::string> internalStateVariableIncrementsUsedInPredictor;

    // normalisation factors
    std::map<std::string,std::string> nf;

    std::string computeStress;

    std::string computeFinalStress;

    std::string predictor;

    std::string initJacobian;

    std::string tangentOperator;

    Algorithm algorithm;

    bool compareToNumericalJacobian;

    bool isConsistantTangentOperatorSymmetricDefined;

    bool useRelaxation;

    bool useAcceleration;

  }; // end of struct MFrontImplicitParserBase

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSERBASE_H */

