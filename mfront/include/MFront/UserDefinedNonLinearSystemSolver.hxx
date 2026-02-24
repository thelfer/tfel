/*!
 * \file  mfront/include/MFront/UserDefinedNonLinearSystem.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 09/09/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_USERDEFINEDNONLINEARSYSTEMSOLVER_HXX
#define LIB_MFRONT_USERDEFINEDNONLINEARSYSTEMSOLVER_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MFront/NonLinearSystemSolverBase.hxx"

namespace mfront {

  //! \brief class for user defined non linear system solvers
  struct UserDefinedNonLinearSystemSolver : public NonLinearSystemSolverBase {
    //! \brief
    using DataMap = tfel::utilities::DataMap;
    /*!
     * \brief constructor
     */
    UserDefinedNonLinearSystemSolver(const DataMap&);
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
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    std::vector<std::string> getSpecificHeaders() const override;
    //! \brief destructor
    ~UserDefinedNonLinearSystemSolver() override;

   public:
    //! \brief class implementing the non linear solver
    std::string class_name;
    /*!
     * \brief path to the header file in which the non linear solver is
     * implemented.
     */
    std::string header_file;
    //! \brief
    bool use_jacobian = true;
    bool use_jacobian_invert = false;
    bool requires_numerical_jacobian = false;
    bool allows_jacobian_initialisation = false;
    bool allows_jacobian_invert_initialisation = false;
    bool requires_jacobian_reinitialisation_to_identity_at_each_iterations =
        true;
  };  // end of struct UserDefinedNonLinearSystemSolver

}  // end of namespace mfront

#endif /* LIB_MFRONT_USERDEFINEDNONLINEARSYSTEMSOLVER_HXX */
