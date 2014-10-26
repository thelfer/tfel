/*!
 * \file   mfront/include/MFront/ImplicitDSLBase.hxx
 * \brief  This file declares the ImplicitDSLBase class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTIMPLICITPARSERBASE_H_
#define _LIB_MFRONTIMPLICITPARSERBASE_H_ 

#include<string>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"MFront/BehaviourDSLBase.hxx"

namespace mfront{

  // forward declaratin
  struct NonLinearSystemSolver;

  /*!
   * Base class for all parser based on an implicit scheme
   */
  struct ImplicitDSLBase
    : public BehaviourDSLBase<ImplicitDSLBase>
  {
    /*!
     * \brief constructor
     */
    ImplicitDSLBase();
    /*!
     * \brief destructor
     */
    virtual ~ImplicitDSLBase();

  protected:
    
    //! a simple alias
    typedef tfel::utilities::shared_ptr<NonLinearSystemSolver> Solver;

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

    // let the BehaviourDSLBase access specific keywords
    friend struct BehaviourDSLBase<ImplicitDSLBase>;

    std::set<std::string> jacobianPartsUsedInIntegrator;

    std::set<std::string> integrationVariablesIncrementsUsedInPredictor;

    Solver solver;

  }; // end of struct ImplicitDSLBase

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSERBASE_H */
