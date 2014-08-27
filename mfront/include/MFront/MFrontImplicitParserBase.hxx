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

  /*!
   * Base class for all parser based on an implicit scheme
   */
  struct MFrontImplicitParserBase
    : public MFrontBehaviourParserBase<MFrontImplicitParserBase>
  {
    /*!
     * \brief constructor
     */
    MFrontImplicitParserBase();
    /*!
     * \brief destructor
     */
    virtual ~MFrontImplicitParserBase();

  protected:

    /*!
     * list of available algorithms
     */
    enum Algorithm{
      NEWTONRAPHSON,                 //!< Standard Newton-Raphson algorithm 
      POWELLDOGLEG_NEWTONRAPHSON,    //!< Powell's dogLeg algorithm based
				     //!  on the standard Newton-Raphson
				     //!  algorithm
      NEWTONRAPHSON_NJ,              //!< Newton-Raphson algorithm
				     //!  using a numerical jacobian
      POWELLDOGLEG_NEWTONRAPHSON_NJ, //!< Powell's dogLeg algorithm
				     //!  based on the Newton-Raphson
				     //!  and numerical evaluation of
				     //!  the jacobian				     
      BROYDEN,                       //!  Girst Broyden algorithm
      POWELLDOGLEG_BROYDEN,          //!< Powell's dogLeg algorithm
				     //!  based on the first Broyden algorithm
      BROYDEN2,                      //<! Second Broyden algorithm
      LEVENBERGMARQUARDT,            //<! Standard Levenberg-Marquardt
				     //   algorithm based on the Newton algorithm
      LEVENBERGMARQUARDT_NJ,         //<! Standard Levenberg-Marquardt
				     //   algorithm based on the Newton
				     //   algorithm with numerical jacobian
      DEFAULT                        //<! Default value
    };

    virtual bool
    isJacobianPart(const Hypothesis,
		   const std::string&);

    virtual void
    predictorAnalyser(const Hypothesis,
		      const std::string&);
    
    virtual void
    integratorAnalyser(const Hypothesis,
		       const std::string&);

    virtual std::string
    tangentOperatorVariableModifier(const Hypothesis,
				    const std::string&,
				    const bool);

    virtual std::string
    integratorVariableModifier(const Hypothesis,
			       const std::string&,
			       const bool);

    virtual std::string
    computeStressVariableModifier1(const Hypothesis,
				   const std::string&,
				   const bool);

    virtual std::string
    computeStressVariableModifier2(const Hypothesis,
				   const std::string&,
				   const bool);
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] n : variable name
     */
    virtual void
    treatUnknownVariableMethod(const Hypothesis,
			       const std::string&);
    /*!
     *
     */
    virtual void
    treatStateVariable(void);
    /*!
     *
     */
    virtual void
    treatIntegrationVariable(void);

    virtual void
    treatIntegrator(void);

    virtual void
    treatComputeFinalStress(void);

    virtual void endsInputFileProcessing(void);

    virtual void writeBehaviourIntegrator(const Hypothesis);

    virtual void writeLevenbergMarquardtAlgorithm(const Hypothesis h);

    virtual void writeStandardImplicitAlgorithm(const Hypothesis h);

    virtual void writeComparisonToNumericalJacobian(const Hypothesis);

    virtual void writeComputeFdF(const Hypothesis);

    virtual void writeBehaviourParserSpecificIncludes(void);

    virtual void writeBehaviourParserSpecificTypedefs(void);

    virtual void writeBehaviourParserSpecificMembers(const Hypothesis);

    virtual void writeBehaviourIntegrationVariablesIncrements(const Hypothesis);
    
    virtual std::string
    getBehaviourConstructorsInitializers(const Hypothesis);

    virtual std::string
    getIntegrationVariablesIncrementsInitializers(const VariableDescriptionContainer&,
						  const bool) const;

    virtual void writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis);

    virtual void writeComputeNumericalJacobian(const Hypothesis);

    virtual void writeGetPartialJacobianInvert(const Hypothesis);

    virtual void writeLimitsOnIncrementValues(const Hypothesis,
					      const std::string&);

    virtual void writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(const Hypothesis);

    virtual void writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(const Hypothesis);

    /*!
     * write a step of the Powell dogleg method
     * \param[in] h  : modelling hypothesis
     * \param[in] B  : name of the jacobian
     * \param[in] f  : name of values of f
     * \param[in] pn : Newton step
     */
    virtual void writePowellDogLegStep(const Hypothesis,
				       const std::string&,
				       const std::string&,
				       const std::string&);


    virtual void treatTheta(void);

    virtual void treatIterMax(void);

    virtual void treatEpsilon(void);

    virtual void treatPowellDogLegTrustRegionSize(void);

    virtual void treatPerturbationValueForNumericalJacobianComputation(void);

    virtual void treatAlgorithm(void);

    virtual void treatPredictor(void);

    virtual void treatComputeStress(void);

    virtual void treatCompareToNumericalJacobian(void);

    virtual void treatJacobianComparisonCriterium(void);

    virtual void treatUseAcceleration(void);

    virtual void treatAccelerationTrigger(void);

    virtual void treatAccelerationPeriod(void);

    virtual void treatUseRelaxation(void);

    virtual void treatRelaxationTrigger(void);
    
    virtual void treatRelaxationCoefficient(void);

    virtual void treatInitJacobian(void);

    virtual void treatMaximumIncrementValuePerIteration(void);
    /*!
     * \return true if the the given variable may have methods
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    virtual bool
    isCallableVariable(const Hypothesis,
		       const std::string&) const;
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

    // let the MFrontBehaviourParserBase access specific keywords
    friend struct MFrontBehaviourParserBase<MFrontImplicitParserBase>;

    std::set<std::string> jacobianPartsUsedInIntegrator;

    std::set<std::string> integrationVariablesIncrementsUsedInPredictor;

    // normalisation factors
    std::map<std::string,std::string> nf;

    Algorithm algorithm;

  }; // end of struct MFrontImplicitParserBase

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSERBASE_H */
