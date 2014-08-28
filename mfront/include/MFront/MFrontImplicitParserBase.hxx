/*!
 * \file   mfront/include/MFront/MFrontImplicitParserBase.hxx
 * \brief  This file declares the MFrontImplicitParserBase class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTIMPLICITPARSERBASE_H_
#define _LIB_MFRONTIMPLICITPARSERBASE_H_ 

#include<string>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"MFront/MFrontBehaviourParserBase.hxx"

namespace mfront{

  // forward declaratin
  struct MFrontNonLinearSystemSolver;

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
    
    //! a simple alias
    typedef tfel::utilities::shared_ptr<MFrontNonLinearSystemSolver> Solver;

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
     * treat an unknown keyword. This method is overriden so the
     * solver may have specific keywords
     */
    virtual void
    treatUnknownKeyword(void);
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

    virtual void treatTheta(void);

    virtual void treatIterMax(void);

    virtual void treatEpsilon(void);

    virtual void treatPerturbationValueForNumericalJacobianComputation(void);

    virtual void treatAlgorithm(void);

    virtual void treatPredictor(void);

    virtual void treatComputeStress(void);

    virtual void treatCompareToNumericalJacobian(void);

    virtual void treatJacobianComparisonCriterium(void);

    virtual void treatInitJacobian(void);

    virtual void treatInitJacobianInvert(void);

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

    // let the MFrontBehaviourParserBase access specific keywords
    friend struct MFrontBehaviourParserBase<MFrontImplicitParserBase>;

    std::set<std::string> jacobianPartsUsedInIntegrator;

    std::set<std::string> integrationVariablesIncrementsUsedInPredictor;

    Solver solver;

  }; // end of struct MFrontImplicitParserBase

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSERBASE_H */
