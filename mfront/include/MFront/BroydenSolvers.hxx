/*!
 * \file  mfront/include/MFront/BroydenSolvers.hxx
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

#ifndef LIB_MFRONT_MFRONTBROYDENSOLVERS_HXX
#define LIB_MFRONT_MFRONTBROYDENSOLVERS_HXX

#include "MFront/NonLinearSystemSolverBase.hxx"
#include "MFront/PowellDogLegAlgorithmBase.hxx"

namespace mfront {

  /*!
   * \brief Base class for non linear system solver based on the
   * Broyden algorithm.
   */
  struct BroydenSolverBase : public NonLinearSystemSolverBase {
    //! a simple alias
    using NonLinearSystemSolverBase::tokens_iterator;
    //! a simple alias
    using Hypothesis = NonLinearSystemSolverBase::Hypothesis;
    //
    [[nodiscard]] bool requiresNumericalJacobian() const override;
    [[nodiscard]] bool allowsJacobianInitialisation() const override;
    [[nodiscard]] bool allowsJacobianInvertInitialisation() const override;
    [[nodiscard]] bool
    requiresJacobianToBeReinitialisedToIdentityAtEachIterations()
        const override;
    void writeSpecificInitializeMethodPart(std::ostream&,
                                           const BehaviourDescription&,
                                           const Hypothesis) const override;
    void writeSpecificMembers(std::ostream&,
                              const BehaviourDescription&,
                              const Hypothesis) const override;
    //! \brief destructor
    ~BroydenSolverBase() override;
  };  // end of struct BroydenSolverBase

  //! \brief the standard Broyden Solver
  struct BroydenSolver : public BroydenSolverBase {
    [[nodiscard]] std::vector<std::string> getSpecificHeaders() const override;
    [[nodiscard]] std::string getExternalAlgorithmClassName(
        const BehaviourDescription&, const Hypothesis) const override;
    [[nodiscard]] bool usesJacobian() const override;
    [[nodiscard]] bool usesJacobianInvert() const override;
    [[nodiscard]] std::pair<bool, tokens_iterator> treatSpecificKeywords(
        BehaviourDescription&,
        const std::string&,
        const tokens_iterator,
        const tokens_iterator) override;
    void completeVariableDeclaration(BehaviourDescription&) const override;
    //! \brief destructor
    ~BroydenSolver() override;
  };

  /*!
   * \brief The standard Broyden Solver coupled with Powell' dog leg
   * algorithm.
   */
  struct PowellDogLegBroydenSolver : public BroydenSolverBase,
                                     protected PowellDogLegAlgorithmBase {
    //
    using BroydenSolverBase::Hypothesis;
    using BroydenSolverBase::tokens_iterator;
    //
    [[nodiscard]] std::vector<std::string> getReservedNames() const override;
    [[nodiscard]] std::vector<std::string> getSpecificHeaders() const override;
    [[nodiscard]] std::string getExternalAlgorithmClassName(
        const BehaviourDescription&, const Hypothesis) const override;
    [[nodiscard]] bool usesJacobian() const override;
    [[nodiscard]] bool usesJacobianInvert() const override;
    [[nodiscard]] std::pair<bool, tokens_iterator> treatSpecificKeywords(
        BehaviourDescription&,
        const std::string&,
        const tokens_iterator,
        const tokens_iterator) override;
    void completeVariableDeclaration(BehaviourDescription&) const override;
    void initializeNumericalParameters(std::ostream&,
                                       const BehaviourDescription&,
                                       const Hypothesis) const override;
    //! \brief destructor
    ~PowellDogLegBroydenSolver() override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBROYDENSOLVERS_HXX */
