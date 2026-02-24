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
    std::vector<std::string> getSpecificHeaders() const override;
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    bool usesJacobian() const override;
    bool usesJacobianInvert() const override;
    bool requiresNumericalJacobian() const override;
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
    //! \brief destructor
    ~SecondBroydenSolver() override;
  };  // end of struct SecondBroydenSolver

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTSECONDBROYDENSOLVER_HXX */
