/*!
 * \file   mfront/include/MFront/ImplicitDSLBase.hxx
 * \brief  This file declares the ImplicitDSLBase class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTIMPLICITPARSERBASE_HXX
#define LIB_MFRONTIMPLICITPARSERBASE_HXX

#include <string>
#include <memory>
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  // forward declaratin
  struct NonLinearSystemSolver;

  //! \brief Base class for all parser based on an implicit scheme
  struct MFRONT_VISIBILITY_EXPORT ImplicitDSLBase
      : public BehaviourDSLBase<ImplicitDSLBase> {
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    ImplicitDSLBase(const DSLOptions&);
    /*!
     * \return the solver used to integrate the mechanical behaviour
     * \throw this methods throws is the solver is not defined yet.
     * \note the solver is only defined after the
     * endsInputFileProcessing is called. This defined at the
     * beginning of this method, so it is safe to use this method in
     * the rest of the endsInputFileProcessing method, in the
     * endTreatment method of the behaviour bricks in particular.
     */
    const NonLinearSystemSolver& getSolver() const;

    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;

    void getSymbols(std::map<std::string, std::string>&,
                    const Hypothesis,
                    const std::string&) override;
    //!\brief destructor
    ~ImplicitDSLBase() override;

   protected:
    virtual void predictorAnalyser(const Hypothesis, const std::string&);

    virtual void integratorAnalyser(const Hypothesis, const std::string&);

    virtual std::string integratorVariableModifier(const Hypothesis,
                                                   const std::string&,
                                                   const bool);

    virtual std::string computeThermodynamicForcesVariableModifier1(
        const Hypothesis, const std::string&, const bool);

    virtual std::string computeThermodynamicForcesVariableModifier2(
        const Hypothesis, const std::string&, const bool);

    std::string tangentOperatorVariableModifier(const Hypothesis,
                                                const std::string&,
                                                const bool) override;

    std::unique_ptr<AbstractBehaviourCodeGenerator> getCodeGenerator()
        const override;
    /*!
     * treat an unknown keyword. This method is overriden so the
     * solver may have specific keywords
     */
    void treatUnknownKeyword() override;

    void treatUnknownVariableMethod(const Hypothesis,
                                    const std::string&) override;

    void treatStateVariable() override;
    //! \brief treat the `@ProcessNewCorrection` keyword
    void treatProcessNewCorrection();
    //! \brief treat the `@RejectCurrentCorrection` keyword
    void treatRejectCurrentCorrection();
    //! \brief treat the `@ProcessNewEstimate` keyword
    void treatProcessNewEstimate();
    //! \brief treat the `@IntegrationVariable` keyword
    void treatIntegrationVariable();

    void treatIntegrator() override;
    //! \brief treat the `@ComputeFinalThermodynamicForces` keyword
    virtual void treatComputeFinalThermodynamicForces();

    void readTangentOperatorCodeBlock(const CodeBlockOptions&,
                                      const std::string&) override;

    void completeVariableDeclaration() override;

    void endsInputFileProcessing() override;
    //! \brief treat the `@Theta` keyword
    virtual void treatTheta();
    //! \brief treat the `@IterMax` keyword
    virtual void treatIterMax();
    //! \brief treat the `@Epsilon` keyword
    virtual void treatEpsilon();
    //! \brief treat the `@AdditionalConvergenceChecks` keyword
    virtual void treatAdditionalConvergenceChecks();
    //! \brief treat the `@PerturbationValueForNumericalJacobianComputation`
    //! keyword
    virtual void treatPerturbationValueForNumericalJacobianComputation();
    //! \brief treat the `@Algorithm` keyword
    virtual void treatAlgorithm();
    //! \brief treat the `@Predictor` keyword
    virtual void treatPredictor();
    //! \brief treat the `@ComputeThermodynamicForces` keyword
    virtual void treatComputeThermodynamicForces();
    //! \brief treat the `@CompareToNumericalJacobian` keyword
    virtual void treatCompareToNumericalJacobian();
    //! \brief treat the `@JacobianComparisonCriterion` keyword
    virtual void treatJacobianComparisonCriterion();
    //! \brief treat the `@InitJacobian` keyword
    virtual void treatInitJacobian();
    //! \brief treat the `@InitJacobianInvert` keyword
    virtual void treatInitJacobianInvert();
    //! \brief treat the `@MaximumIncrementValuePerIteration` keyword
    virtual void treatMaximumIncrementValuePerIteration();
    //! \brief treat the `@NumericallyComputedJacobianBlocks` keyword
    virtual void treatNumericallyComputedJacobianBlocks();
    /*!
     * \brief set the non linear solver
     * \param[in] s: non linear solver
     * \param[in] n: name of the non linear solver
     */
    virtual void setNonLinearSolver(std::shared_ptr<NonLinearSystemSolver>,
                                    const std::string&);
    /*!
     * \brief set the non linear solver
     * \param[in] s: non linear solver
     */
    virtual void setNonLinearSolver(const std::string&);
    /*!
     * \return true if the the given variable may have methods
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool isCallableVariable(const Hypothesis,
                            const std::string&) const override;

    // let the BehaviourDSLBase access specific keywords
    friend struct BehaviourDSLBase<ImplicitDSLBase>;

    std::set<std::string> jacobianPartsUsedInIntegrator;

    std::set<std::string> integrationVariablesIncrementsUsedInPredictor;
    //! \brief non linear solver
    std::shared_ptr<NonLinearSystemSolver> solver;
  };  // end of struct ImplicitDSLBase

}  // end of namespace mfront

#endif /* LIB_MFRONTIMPLICITPARSERBASE_HXX */
