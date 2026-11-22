/*!
 * \file   mfront/include/LinearSystemSolverBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date 07/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LINEARSYSTEMSOLVERBASE_HXX
#define LIB_MFRONT_LINEARSYSTEMSOLVERBASE_HXX

#include "MFront/AbstractLinearSystemSolver.hxx"

namespace mfront {

  /*!
   * \brief Base class for linear system solver used by implicit
   * domain specific languages.
   */
  struct MFRONT_VISIBILITY_EXPORT LinearSystemSolverBase
      : AbstractLinearSystemSolver {
    /*!
     * \brief constructor
     */
    LinearSystemSolverBase();
    //
    std::vector<std::string> getSpecificHeaders() const override;
    std::vector<std::string> getReservedNames() const override;
    void writeSpecificMembers(std::ostream&,
                              const BehaviourDescription&,
                              const Hypothesis) const override;
    bool usesSimultaneousSubstitutions() const override;
    /*!
     * \brief write the initialisation of the right hand side to the first
     * columns of the identity matrix, followed by its substitution (see
     * `writeLinearSystemSubstitution`)
     *
     * \param[in] os: output stream
     * \param[in] r: result of the matrix decomposition
     * \param[in] s: description of the substitution variables
     */
    void writeIdentityColumnsSubstitution(
        std::ostream&,
        const MatrixDecompositionResult&,
        const LinearSystemSubstitutionVariables&) const override;
    //! \brief destructor
    ~LinearSystemSolverBase() override;

   protected:
    /*!
     * \brief if true, the linear systems whose right-hand sides are the
     * first columns of the identity matrix are solved by a single
     * substitution
     */
    bool simultaneous_substitutions = false;
  };  // end of struct LinearSystemSolverBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_LINEARSYSTEMSOLVERBASE_HXX */
