/*!
 * \file  mfront/include/MFront/SecondBroydenSolver.hxx
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

#ifndef LIB_MFRONT_MFRONTSECONDBROYDENSOLVER_HXX
#define LIB_MFRONT_MFRONTSECONDBROYDENSOLVER_HXX

#include "MFront/NonLinearSystemSolverBase.hxx"

namespace mfront {

  /*!
   * \brief Non linear system solver based on the
   * second Broyden algorithm.
   */
  struct SecondBroydenSolver : public NonLinearSystemSolverBase {
    [[nodiscard]] std::vector<std::string> getSpecificHeaders() const override;
    [[nodiscard]] std::string getExternalAlgorithmClassName(
        const BehaviourDescription&, const Hypothesis) const override;
    [[nodiscard]] bool usesJacobian() const override;
    [[nodiscard]] bool usesJacobianInvert() const override;
    [[nodiscard]] bool requiresNumericalJacobian() const override;
    [[nodiscard]] bool allowsJacobianInitialisation() const override;
    [[nodiscard]] bool allowsJacobianInvertInitialisation() const override;
    [[nodiscard]] bool
    requiresJacobianToBeReinitialisedToIdentityAtEachIterations()
        const override;
    [[nodiscard]] std::pair<bool, tokens_iterator> treatSpecificKeywords(
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
    //! \brief destructor
    ~SecondBroydenSolver() override;
  };  // end of struct SecondBroydenSolver

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTSECONDBROYDENSOLVER_HXX */
