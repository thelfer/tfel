/*!
 * \file   mtest/include/MTest/PipeTest.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 nov. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_PIPETEST_HXX_
#define _LIB_MTEST_PIPETEST_HXX_

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"
#include"MTest/Study.hxx"

namespace mtest{

  /*!
   * a study describing mechanical tests on pipes
   */
  struct MTEST_VISIBILITY_EXPORT PipeTest
    : public Study
  {
    //! a simple alias
    using size_type = tfel::math::vector<real>::size_type;
    /*!
     * \return the total number of unknowns (including the Lagrange
     * multipliers)
     */
    virtual size_type
    getNumberOfUnknowns(void) const override;
    /*!
     * \brief update current state at the beginning of a new time step:
     * - update the material properties
     * - update the external state variables
     * - compute the thermal expansion if mandatory
     * \param[out] s: current structure state
     * \param[in]  t: current time
     * \param[in] dt: time increment
     */
    virtual void
    prepare(StudyCurrentState&,
	    const real,
	    const real) const override;
    /*!
     * \brief make a linear prediction of the unknows and state
     * \param[out] s: current structure state
     * \param[in] dt: time increment
     */
    virtual void makeLinearPrediction(StudyCurrentState&,
				      const real) const override;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    virtual bool
    computePredictionStiffnessAndResidual(StudyCurrentState&,
					  tfel::math::matrix<real>&,
					  tfel::math::vector<real>&,
					  const real&,
					  const real&,
					  const StiffnessMatrixType) const override;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    virtual bool
    computeStiffnessMatrixAndResidual(StudyCurrentState&,
				      tfel::math::matrix<real>&,
				      tfel::math::vector<real>&,
				      const real,
				      const real,
				      const StiffnessMatrixType) const override;
    /*!
     * \param[in] : du unknows increment difference between two iterations
     */
    virtual real
    getErrorNorm(const tfel::math::vector<real>&) const override;
    /*!
     * \param[in]  s: current structure state
     * \param[in] du: unknows increment estimation
     * \param[in] r:  residual
     * \param[in] o:  solver options
     * \param[in] i:  iteration number
     * \param[in] t:  current time
     * \param[in] dt: time increment
     * \return a boolean saying if all convergence criteria are met
     */
    virtual bool
    checkConvergence(const StudyCurrentState&,
		     const tfel::math::vector<real>&,
		     const tfel::math::vector<real>&,
		     const SolverOptions&,
		     const unsigned int,
		     const real,
		     const real) const override;
    /*!
     * \param[in]  s: current structure state
     * \param[in] du: unknows increment estimation
     * \param[in] r:  residual
     * \param[in] o:  solver options
     * \param[in] t:  current time
     * \param[in] dt: time increment
     * \return a description of all the criteria that were not met.
     */
    virtual std::vector<std::string>
    getFailedCriteriaDiagnostic(const StudyCurrentState&,
				const tfel::math::vector<real>&,
				const tfel::math::vector<real>&,
				const SolverOptions&,
				const real,
				const real) const override;
    /*!
     * \param[out] s: current structure state
     * \param[in]  t:  current time
     * \param[in]  dt: time increment
     * \param[in]  p:  period
     */
    virtual void
    postConvergence(StudyCurrentState&,
		    const real,
		    const real,
		    const unsigned int) const override;
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual void
    setModellingHypothesis(const std::string&) override;
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultHypothesis(void) override;
    //! destructor
    virtual ~PipeTest();
  }; // end of struct PipeTest
  
} // end of namespace mtest

#endif /* _LIB_MTEST_PIPETEST_HXX_ */
