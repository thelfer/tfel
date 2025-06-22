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
  struct MFrontNewtonRaphsonSolverBase : public NonLinearSystemSolverBase,
                                         protected PowellDogLegAlgorithmBase {
    //! a simple alias
    using NonLinearSystemSolverBase::CxxTokenizer;
    //! a simple alias
    using NonLinearSystemSolverBase::tokens_iterator;
    //! a simple alias
    using Hypothesis = NonLinearSystemSolverBase::Hypothesis;
    //! Constructors
    MFrontNewtonRaphsonSolverBase();
    //! \return the reserved names
    std::vector<std::string> getReservedNames() const override;
    /*!
     * \return true if the solver uses the jacobian of the system
     * (Newton-Raphson solvers) or an approximation of it (Broyden
     * method).
     */
    bool usesJacobian() const override;
    /*!
     * \return true if the solver uses the invert of the jacobian of
     * the system or an approximation of it (second Broyden method).
     */
    bool usesJacobianInvert() const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * jacobian
     */
    bool allowsJacobianInitialisation() const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * invert of the jacobian.
     * \note If the solver does not provide such initialisation, the
     * invert of the jacobian could for example be initialised to
     * identity in the behaviour constructor.
     */
    bool allowsJacobianInvertInitialisation() const override;
    /*!
     * \return true if the solver requires the jacobian to be
     * initialized to the identity matrix at the beginning of the
     * computeFdF method.
     */
    bool requiresJacobianToBeReinitialisedToIdentityAtEachIterations()
        const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in,out] md  : mechanical description
     * \param[in] key     : keyword
     * \param[in] p       : current position in file (after keyword)
     * \param[in] pe      : end of file
     */
    std::pair<bool, tokens_iterator> treatSpecificKeywords(
        BehaviourDescription&,
        const std::string&,
        const tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \brief method called when all the user defined variables have been set.
     * \param[in,out] md  : mechanical description
     */
    void completeVariableDeclaration(BehaviourDescription&) const override;
    /*!
     * \brief write algorithm specific initialisations
     * This method may be used to initialise the jacobian to identity
     * at the beginning of the computations (if the user did no
     * provide an initialisation of the jacobian).
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    void writeSpecificInitializeMethodPart(std::ostream&,
                                           const BehaviourDescription&,
                                           const Hypothesis) const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    void writeSpecificMembers(std::ostream&,
                              const BehaviourDescription&,
                              const Hypothesis) const override;
    /*!
     * \brief write the resolution algorithm
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    void writeResolutionAlgorithm(std::ostream&,
                                  const BehaviourDescription&,
                                  const Hypothesis) const override;

   protected:
    /*!
     * destructor
     */
    ~MFrontNewtonRaphsonSolverBase() override;
    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    virtual bool usesPowellDogLegAlgorithm() const = 0;
    //! update jacobian period
    int jacobianUpdatePeriod;
  };  // end of struct MFrontNewtonRaphsonSolverBase

  /*!
   * The standard Newton-Raphson Solver
   */
  struct MFrontNewtonRaphsonSolver : public MFrontNewtonRaphsonSolverBase {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    bool requiresNumericalJacobian() const override;
    //! destructor
    ~MFrontNewtonRaphsonSolver() override;

   protected:
    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    bool usesPowellDogLegAlgorithm() const override;
  };

  /*!
   * The standard Newton-Raphson Solver
   */
  struct MFrontNewtonRaphsonNumericalJacobianSolver
      : public MFrontNewtonRaphsonSolverBase {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    bool requiresNumericalJacobian() const override;
    //! destructor
    ~MFrontNewtonRaphsonNumericalJacobianSolver() override;

   protected:
    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    bool usesPowellDogLegAlgorithm() const override;
  };

  /*!
   * The standard Newton-Raphson Solver
   */
  struct MFrontPowellDogLegNewtonRaphsonSolver
      : public MFrontNewtonRaphsonSolverBase {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    bool requiresNumericalJacobian() const override;
    /*!
     * destructor
     */
    ~MFrontPowellDogLegNewtonRaphsonSolver() override;

   protected:
    /*!
     * The derived class shall return true if the PowellDogLegNewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    bool usesPowellDogLegAlgorithm() const override;
  };

  /*!
   * The standard PowellDogLegNewton-Raphson Solver
   */
  struct MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver
      : public MFrontNewtonRaphsonSolverBase {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    bool requiresNumericalJacobian() const override;
    //! destructor
    ~MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver() override;

   protected:
    /*!
     * The derived class shall return true if the PowellDogLegNewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    bool usesPowellDogLegAlgorithm() const override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTNEWTONRAPHSONSOLVERS_HXX */
