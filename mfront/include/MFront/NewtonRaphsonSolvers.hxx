/*!
 * \file  mfront/include/MFront/NewtonRaphsonSolvers.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
    //! \brief default constructor
    MFrontNewtonRaphsonSolverBase();
    std::vector<std::string> getSpecificHeaders() const override;
    std::vector<std::string> getReservedNames() const override;
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
    void writeResolutionAlgorithm(std::ostream&,
                                  const BehaviourDescription&,
                                  const Hypothesis) const override;

   protected:
    virtual bool usesPowellDogLegAlgorithm() const = 0;
    //! \brief destructor
    ~MFrontNewtonRaphsonSolverBase() override;
  };  // end of struct MFrontNewtonRaphsonSolverBase

  //! \brief the standard Newton-Raphson Solver
  struct MFrontNewtonRaphsonSolver : public MFrontNewtonRaphsonSolverBase {
    bool usesExternalAlgorithm() const override;
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    bool requiresNumericalJacobian() const override;
    //! \brief destructor
    ~MFrontNewtonRaphsonSolver() override;

   protected:
    bool usesPowellDogLegAlgorithm() const override;
  };

  //! \brief the standard Newton-Raphson solver with a numerical jacobian
  struct MFrontNewtonRaphsonNumericalJacobianSolver
      : public MFrontNewtonRaphsonSolverBase {
    bool usesExternalAlgorithm() const override;
    std::string getExternalAlgorithmClassName(const BehaviourDescription&,
                                              const Hypothesis) const override;
    bool requiresNumericalJacobian() const override;
    //! destructor
    ~MFrontNewtonRaphsonNumericalJacobianSolver() override;

   protected:
    bool usesPowellDogLegAlgorithm() const override;
  };

  /*!
   * The standard Newton-Raphson Solver
   */
  struct MFrontPowellDogLegNewtonRaphsonSolver
      : public MFrontNewtonRaphsonSolverBase {
    bool requiresNumericalJacobian() const override;
    //! \brief destructor
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
