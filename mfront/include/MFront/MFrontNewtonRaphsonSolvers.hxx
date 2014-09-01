/*! 
 * \file  mfront/include/MFront/MFrontNewtonRaphsonSolvers.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTNEWTONRAPHSONSOLVERS_H_
#define _LIB_MFRONT_MFRONTNEWTONRAPHSONSOLVERS_H_ 

#include "MFront/MFrontNonLinearSystemSolverBase.hxx"
#include "MFront/MFrontPowellDogLegAlgorithmBase.hxx"

namespace mfront{

  /*!
   * base class for non linear system solver based on the
   * Newton-Raphson algorithm.
   * The Newton-Raphson algorithm can be coupled with the
   * PowellDogLegAlgorithm for increased robustness.
   */
  struct MFrontNewtonRaphsonSolverBase
    : public    MFrontNonLinearSystemSolverBase,
      protected MFrontPowellDogLegAlgorithmBase
  {
    /*!
     * \return the reserved names
     */
    virtual std::vector<std::string>
    getReservedNames(void) const;
    /*!
     * \return true if the solver uses the jacobian of the system
     * (Newton-Raphson solvers) or an approximation of it (Broyden
     * method).
     */
    virtual bool
    usesJacobian(void) const;
    /*!
     * \return true if the solver uses the invert of the jacobian of
     * the system or an approximation of it (second Broyden method).
     */
    virtual bool
    usesJacobianInvert(void) const;
    /*!
     * \return true if the solver allows the user to initialize the
     * jacobian
     */
    virtual bool
    allowsJacobianInitialisation(void) const;
    /*!
     * \return true if the solver allows the user to initialize the
     * invert of the jacobian.
     * \note If the solver does not provide such initialisation, the
     * invert of the jacobian could for example be initialised to
     * identity in the behaviour constructor.
     */
    virtual bool
    allowsJacobianInvertInitialisation(void) const;
    /*!
     * \return true if the solver requires the jacobian to be
     * initialized to the identity matrix at the beginning of the
     * computeFdF method.
     */
    virtual bool
    requiresJacobianToBeReinitialisedToIdentityAtEachIterations(void) const;
    /*!
     * \brief write the algorithm specific members
     * \param[in,out] md  : mechanical description
     * \param[in] key     : keyword
     * \param[in] p       : current position in file (after keyword)
     * \param[in] pe      : end of file
     */
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatSpecificKeywords(MechanicalBehaviourDescription&,
			  const std::string&,
			  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
			  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) const;
    /*!
     * \brief method called at the end of the input file processing.
     * Solver can use it to declared additional parameters
     * \param[in,out] md  : mechanical description
     */
    virtual void
    endsInputFileProcessing(MechanicalBehaviourDescription&) const;
    /*!
     * \brief write algorithm specific initialisations
     * This method may be used to initialise the jacobian to identity
     * at the beginning of the computations (if the user did no
     * provide an initialisation of the jacobian).
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void
    writeSpecificInitializeMethodPart(std::ostream&,
				      const MechanicalBehaviourDescription&,
				      const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \brief write the algorithm specific members
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void
    writeSpecificMembers(std::ostream&,
			 const MechanicalBehaviourDescription&,
			 const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \brief write the resolution algorithm
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void
    writeResolutionAlgorithm(std::ostream&,
			     const MechanicalBehaviourDescription&,
			     const tfel::material::ModellingHypothesis::Hypothesis) const;
  protected:
    /*!
     * destructor
     */
    virtual ~MFrontNewtonRaphsonSolverBase();
    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    virtual bool
    usesPowellDogLegAlgorithm(void) const = 0;
  }; // end of struct MFrontNewtonRaphsonSolverBase

  /*!
   * The standard Newton-Raphson Solver
   */
  struct MFrontNewtonRaphsonSolver
    : public MFrontNewtonRaphsonSolverBase
  {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    virtual bool
    requiresNumericalJacobian(void) const;
    /*!
     * destructor
     */
    virtual ~MFrontNewtonRaphsonSolver();
  protected:
    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    virtual bool
    usesPowellDogLegAlgorithm(void) const;
  };

  /*!
   * The standard Newton-Raphson Solver
   */
  struct MFrontNewtonRaphsonNumericalJacobianSolver
    : public MFrontNewtonRaphsonSolverBase
  {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    virtual bool
    requiresNumericalJacobian(void) const;
    /*!
     * destructor
     */
    virtual ~MFrontNewtonRaphsonNumericalJacobianSolver();
  protected:
    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    virtual bool
    usesPowellDogLegAlgorithm(void) const;
  };

  /*!
   * The standard Newton-Raphson Solver
   */
  struct MFrontPowellDogLegNewtonRaphsonSolver
    : public MFrontNewtonRaphsonSolverBase
  {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    virtual bool
    requiresNumericalJacobian(void) const;
    /*!
     * destructor
     */
    virtual ~MFrontPowellDogLegNewtonRaphsonSolver();
  protected:
    /*!
     * The derived class shall return true if the PowellDogLegNewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    virtual bool
    usesPowellDogLegAlgorithm(void) const;
  };

  /*!
   * The standard PowellDogLegNewton-Raphson Solver
   */
  struct MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver
    : public MFrontNewtonRaphsonSolverBase
  {
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    virtual bool
    requiresNumericalJacobian(void) const;
    /*!
     * destructor
     */
    virtual ~MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver();
  protected:
    /*!
     * The derived class shall return true if the PowellDogLegNewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    virtual bool
    usesPowellDogLegAlgorithm(void) const;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTNEWTONRAPHSONSOLVERS_H */

