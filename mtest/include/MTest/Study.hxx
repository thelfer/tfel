/*!
 * \file   Study.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   03 nov. 2015
 */

#ifndef _LIB_MTEST_STUDY_HXX_
#define _LIB_MTEST_STUDY_HXX_

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"
#include"MTest/SolverOptions.hxx"

namespace mtest{

  // forward declaration
  struct StudyCurrentState;

  // forward declaration
  struct Structure;
  
  /*!
   * \brief abstract class describing a physical system
   */
  struct MTEST_VISIBILITY_EXPORT Study
  {
    //! a simple alias
    using size_type = tfel::math::vector<real>::size_type;
    /*!
     * \return the total number of unknowns (including the Lagrange
     * multipliers)
     */
    virtual size_type
    getNumberOfUnknowns(void) const = 0;
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
	    const real) const = 0;
    /*!
     * \brief make a linear prediction of the unknows and state
     * \param[out] s: current structure state
     * \param[in] dt: time increment
     */
    virtual void makeLinearPrediction(StudyCurrentState&,
				      const real) const = 0;
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
					  const StiffnessMatrixType) const = 0;
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
				      const StiffnessMatrixType) const = 0;
    /*!
     * \param[in]  s: current structure state
     * \param[in] du: unknows increment estimation
     * \param[in] r:  residual
     * \param[in] o:  solver options
     * \param[in] i:  iteration number
     * \param[in] t:  current time
     * \param[in] dt: time increment
     * \return a pair containing:
     * - a boolean saying if all convergence criteria are met
     * - one of the convergence estimator used to compute the order of
     *   convergence.
     */
    virtual std::pair<bool,real>
    checkConvergence(const StudyCurrentState&,
		     const tfel::math::vector<real>&,
		     const tfel::math::vector<real>&,
		     const SolverOptions&,
		     const unsigned int,
		     const real,
		     const real) const = 0;
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
				const real) const = 0;
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
		    const unsigned int) const = 0;
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual void
    setModellingHypothesis(const std::string&) = 0;
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultHypothesis(void) = 0;
    //! destructor
    virtual ~Study();
  }; // end of struct Study
  
} // end of namespace mtest

#endif /* _LIB_MTEST_STUDY_HXX_ */

