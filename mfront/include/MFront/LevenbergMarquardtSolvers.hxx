/*!
 * \file  mfront/include/MFront/LevenbergMarquardtSolvers.hxx
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

#ifndef LIB_MFRONT_LEVENBERGMARQUARTSOLVERS_HXX
#define LIB_MFRONT_LEVENBERGMARQUARTSOLVERS_HXX

#include "MFront/NonLinearSystemSolverBase.hxx"

namespace mfront {

  /*!
   * base class for non linear system solver based on the
   * Newton-Raphson algorithm coupled with the Levenberg-Marquart
   * algorithm for increased robustness.
   */
  struct LevenbergMarquardtSolverBase : public NonLinearSystemSolverBase {
    /*!
     * \return the reserved names
     */
    virtual std::vector<std::string> getReservedNames() const override;
    /*!
     * \return true if the solver uses the jacobian of the system
     * (Newton-Raphson solvers) or an approximation of it (Broyden
     * method).
     */
    virtual bool usesJacobian() const override;
    /*!
     * \return true if the solver uses the invert of the jacobian of
     * the system or an approximation of it (second Broyden method).
     */
    virtual bool usesJacobianInvert() const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * jacobian
     */
    virtual bool allowsJacobianInitialisation() const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * invert of the jacobian.
     * \note If the solver does not provide such initialisation, the
     * invert of the jacobian could for example be initialised to
     * identity in the behaviour constructor.
     */
    virtual bool allowsJacobianInvertInitialisation() const override;
    /*!
     * \return true if the solver requires the jacobian to be
     * initialized to the identity matrix at the beginning of the
     * computeFdF method.
     */
    virtual bool requiresJacobianToBeReinitialisedToIdentityAtEachIterations()
        const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in,out] md  : mechanical description
     * \param[in] key     : keyword
     * \param[in] p       : current position in file (after keyword)
     * \param[in] pe      : end of file
     */
    virtual std::pair<bool, tokens_iterator> treatSpecificKeywords(
        BehaviourDescription&,
        const std::string&,
        const tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \brief method called when all the user defined variables have been set.
     * \param[in,out] md  : mechanical description
     */
    virtual void completeVariableDeclaration(
        BehaviourDescription&) const override;
    /*!
     * \brief write algorithm specific initialisations
     * This method may be used to initialise the jacobian to identity
     * at the beginning of the computations (if the user did no
     * provide an initialisation of the jacobian).
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void writeSpecificInitializeMethodPart(
        std::ostream&,
        const BehaviourDescription&,
        const Hypothesis) const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void writeSpecificMembers(std::ostream&,
                                      const BehaviourDescription&,
                                      const Hypothesis) const override;
    /*!
     * \brief write the resolution algorithm
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void writeResolutionAlgorithm(std::ostream&,
                                          const BehaviourDescription&,
                                          const Hypothesis) const override;

   protected:
    /*!
     * destructor
     */
    virtual ~LevenbergMarquardtSolverBase();
  };  // end of struct LevenbergMarquardtSolverBase

  /*!
   * the standard Newton-Raphson solver coupled with the
   * Levenberg-Marquart algorithm for increased robustness
   */
  struct LevenbergMarquardtSolver : public LevenbergMarquardtSolverBase {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    virtual bool requiresNumericalJacobian() const override;
    /*!
     * destructor
     */
    virtual ~LevenbergMarquardtSolver();
  };

  /*!
   * the standard Newton-Raphson solver based on a numerical jacobian
   * evaluation coupled with the Levenberg-Marquart algorithm for
   * increased robustness
   */
  struct LevenbergMarquardtNumericalJacobianSolver
      : public LevenbergMarquardtSolverBase {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    virtual bool requiresNumericalJacobian() const override;
    /*!
     * destructor
     */
    virtual ~LevenbergMarquardtNumericalJacobianSolver();
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_LEVENBERGMARQUARTSOLVERS_HXX */
