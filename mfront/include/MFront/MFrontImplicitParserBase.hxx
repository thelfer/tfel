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

namespace mfront{

  struct MFrontImplicitParserBase
    : public MFrontBehaviourParserBase<MFrontImplicitParserBase>
  {

    MFrontImplicitParserBase();

    virtual ~MFrontImplicitParserBase();

  protected:

    enum Algorithm{
      NEWTONRAPHSON,
      POWELLDOGLEG_NEWTONRAPHSON,
      NEWTONRAPHSON_NJ,
      POWELLDOGLEG_NEWTONRAPHSON_NJ,
      BROYDEN,
      POWELLDOGLEG_BROYDEN,
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

    virtual void
    treatComputeFinalStress(void);

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

    virtual void writeLimitsOnIncrementValues(const std::string&);

    virtual void writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(void);

    virtual void writeLimitsOnIncrementValuesBasedOnStateVariablesIncrementsPhysicalBounds(void);

    /*!
     * write a step of the Powell dogleg method
     * \param[in] B  : name of the jacobian
     * \param[in] f  : name of values of f
     * \param[in] pn : Newton step
     */
    virtual void writePowellDogLegStep(const std::string&,
				       const std::string&,
				       const std::string&);


    virtual void treatTheta(void);

    virtual void treatIterMax(void);

    virtual void treatEpsilon(void);

    virtual void treatPowellDogLegTrustRegionSize(void);

    virtual void treatPertubationValueForNumericalJacobianComputation(void);

    virtual void treatAlgorithm(void);

    virtual void treatPredictor(void);

    virtual void treatComputeStress(void);

    virtual void treatStateVariables(void);

    virtual void treatAuxiliaryStateVariables(void);

    virtual void treatCompareToNumericalJacobian(void);

    virtual void treatJacobianComparisonCriterium(void);

    virtual void treatUseAcceleration(void);

    virtual void treatAccelerationTrigger(void);

    virtual void treatAccelerationPeriod(void);

    virtual void treatUseRelaxation(void);

    virtual void treatRelaxationTrigger(void);
    
    virtual void treatRelaxationCoefficient(void);

    virtual void treatInitJacobian(void);

    virtual void treatTangentOperator(void);

    virtual void treatIsTangentOperatorSymmetric(void);

    virtual void treatMaximumIncrementValuePerIteration(void);

    virtual void
    treatUnknownKeyword(void);

    /*!
     * \return a type able to do the mapping of a variable with the
     * vector used by the root-finding algorithm.
     * \param[in] v : variable
     */
    virtual std::string
    getVectorMappingClass(const VariableDescription&) const;

    // /*!
    //  * \return a type able to do the mapping of the derivate of a
    //  * variable with respect to a scalar with the jacobian matrix used
    //  * by the root-finding algorithm.
    //  * \param[in] v : variable
    //  */
    // virtual std::string
    // getMathObjectDerivateByScalarMappingClass(const VariableDescription&) const;

    // /*!
    //  * \return a type able to do the mapping of the derivate of a
    //  * scalar with respect to the variable with the jacobian matrix
    //  * used by the root-finding algorithm.
    //  * \param[in] v : variable
    //  */
    // virtual std::string
    // getScalarDerivateByMathObjectMappingClass(const VariableDescription&) const;

    // /*!
    //  * \return a type able to do the mapping of the derivate of the
    //  * first variable with respect to the second variable with the
    //  * jacobian matrix used by the root-finding algorithm.
    //  * \param[in] v1 : variable
    //  * \param[in] v2 : variable
    //  */
    // virtual std::string
    // getMathObjectDerivateByMathObjectMappingClass(const VariableDescription&,
    // 						     const VariableDescription&) const;

    virtual std::string
    getJacobianPart(const VariableDescription&,
		    const VariableDescription&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const std::string& = "this->jacobian",
		    const std::string& = "");

    friend struct MFrontBehaviourParserBase<MFrontImplicitParserBase>;

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

    bool hasUserDefinedComputeFinalStress;
    
    bool compareToNumericalJacobian;

    bool isConsistantTangentOperatorSymmetricDefined;

    bool useRelaxation;

    bool useAcceleration;

  }; // end of struct MFrontImplicitParserBase

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSERBASE_H */

