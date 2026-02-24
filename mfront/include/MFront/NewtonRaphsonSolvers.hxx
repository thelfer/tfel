/*!
 * \file  mfront/include/MFront/NewtonRaphsonSolvers.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTNEWTONRAPHSONSOLVERS_HXX
#define LIB_MFRONT_MFRONTNEWTONRAPHSONSOLVERS_HXX

#include "MFront/NonLinearSystemSolverBase.hxx"
#include "MFront/PowellDogLegAlgorithmBase.hxx"

namespace mfront {

  /*!
   * \brief base class for non linear system solver based on the
   * Newton-Raphson algorithm.
   * The Newton-Raphson algorithm can be coupled with the
   * PowellDogLegAlgorithm for increased robustness.
   */
  struct NewtonRaphsonSolverBase : public NonLinearSystemSolverBase {
    //! a simple alias
    using NonLinearSystemSolverBase::CxxTokenizer;
    //! a simple alias
    using NonLinearSystemSolverBase::tokens_iterator;
    //! a simple alias
    using Hypothesis = NonLinearSystemSolverBase::Hypothesis;
    //! \brief default constructor
    NewtonRaphsonSolverBase();
    bool usesJacobian() const override;
    bool usesJacobianInvert() const override;
    bool allowsJacobianInitialisation() const override;
    bool allowsJacobianInvertInitialisation() const override;
    bool requiresJacobianToBeReinitialisedToIdentityAtEachIterations()
        const override;
    std::pair<bool, tokens_iterator> treatSpecificKeywords(
        BehaviourDescription&,
        const std::string&,
        const tokens_iterator,
        const tokens_iterator) override;
    void completeVariableDeclaration(BehaviourDescription&) const override;
    void writeSpecificInitializeMethodPart(std::ostream&,
                                           const BehaviourDescription&,
                                           const Hypothesis) const override;
    void writeSpecificMembers(std::ostream&,
                              const BehaviourDescription&,
                              const Hypothesis) const override;

   protected:
    //! \brief destructor
    ~NewtonRaphsonSolverBase() override;
  };  // end of struct NewtonRaphsonSolverBase

  //! \brief the standard Newton-Raphson Solver
  struct NewtonRaphsonSolver : public NewtonRaphsonSolverBase {
    std::vector<std::string> getSpecificHeaders() const override;
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    bool requiresNumericalJacobian() const override;
    //! \brief destructor
    ~NewtonRaphsonSolver() override;
  };

  //! \brief the standard Newton-Raphson solver with a numerical jacobian
  struct NewtonRaphsonNumericalJacobianSolver : public NewtonRaphsonSolverBase {
    std::vector<std::string> getSpecificHeaders() const override;
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    bool requiresNumericalJacobian() const override;
    //! destructor
    ~NewtonRaphsonNumericalJacobianSolver() override;
  };

  /*!
   * \brief the standard Newton-Raphson Solver coupled with a Powell' dog leg
   * algorithm.
   */
  struct PowellDogLegNewtonRaphsonSolver : public NewtonRaphsonSolverBase,
                                           protected PowellDogLegAlgorithmBase {
    using Hypothesis = NewtonRaphsonSolverBase::Hypothesis;
    //! a simple alias
    using NonLinearSystemSolverBase::CxxTokenizer;
    //! a simple alias
    using NonLinearSystemSolverBase::tokens_iterator;
    std::vector<std::string> getSpecificHeaders() const override;
    void initializeNumericalParameters(std::ostream&,
                                       const BehaviourDescription&,
                                       const Hypothesis) const override;
    std::pair<bool, tokens_iterator> treatSpecificKeywords(
        BehaviourDescription&,
        const std::string&,
        const tokens_iterator,
        const tokens_iterator) override;
    void completeVariableDeclaration(BehaviourDescription&) const override;
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    bool requiresNumericalJacobian() const override;
    //! \brief destructor
    ~PowellDogLegNewtonRaphsonSolver() override;
  };

  /*!
   * \brief the standard Newton-Raphson Solver with numerical jacobian coupled
   * with a Powell' dog leg algorithm.
   */
  struct PowellDogLegNewtonRaphsonNumericalJacobianSolver
      : public NewtonRaphsonSolverBase,
        protected PowellDogLegAlgorithmBase {
    using Hypothesis = NewtonRaphsonSolverBase::Hypothesis;
    //! a simple alias
    using NonLinearSystemSolverBase::CxxTokenizer;
    //! a simple alias
    using NonLinearSystemSolverBase::tokens_iterator;
    std::vector<std::string> getSpecificHeaders() const override;
    void initializeNumericalParameters(std::ostream&,
                                       const BehaviourDescription&,
                                       const Hypothesis) const override;
    std::pair<bool, tokens_iterator> treatSpecificKeywords(
        BehaviourDescription&,
        const std::string&,
        const tokens_iterator,
        const tokens_iterator) override;
    void completeVariableDeclaration(BehaviourDescription&) const override;
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    bool requiresNumericalJacobian() const override;
    //! destructor
    ~PowellDogLegNewtonRaphsonNumericalJacobianSolver() override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTNEWTONRAPHSONSOLVERS_HXX */
