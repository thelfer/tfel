/*! 
 * \file  mtest/include/MTest/MTest.hxx
 * \brief
 * \author Helfer Thomas
 * \date 09 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_MTEST_H_
#define LIB_MTEST_MTEST_H_ 

#include<map>
#include<string>
#include<vector>
#include<memory>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/LUSolve.hxx"

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"
#include"MTest/Constraint.hxx"
#include"MTest/SolverWorkSpace.hxx"
#include"MTest/StudyCurrentState.hxx"
#include"MTest/AccelerationAlgorithm.hxx"
#include"MTest/SingleStructureScheme.hxx"

namespace mtest
{
  
  /*!
   * \brief MTest is a simple class to test mfront behaviours.
   *
   * The unknowns are made of the driving variables stensor (for small
   * strain behaviours) followed by a number of lagrange multiplier.
   */
  struct MTEST_VISIBILITY_EXPORT MTest
    : public SingleStructureScheme
  {
    /*!
     * \brief base class for testing the behaviour outputs
     */
    struct UTest
    {
      /*!
       * Type of variable tested
       */
      enum TestedVariable{
	INTERNALSTATEVARIABLE,
	DRIVINGVARIABLE,
	THERMODYNAMICFORCE
      };
      /*!
       * \param[in] e  : driving variabless
       * \param[in] s  : thermodynamic forces
       * \param[in] iv : internal state variables
       * \param[in] t  : time
       * \param[in] dt : time increment
       * \param[in] p  : period
       */
      virtual void
      check(const tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const real,
	    const real,
	    const unsigned int) = 0;
      /*!
       * \return the results of the test
       */
      virtual tfel::tests::TestResult
      getResults() const = 0;
      //! desctructor
      virtual ~UTest();
    };
    /*!
     * default constructor
     */
    MTest(void);
    /*! 
     * \return the name of the test
     */
    virtual std::string
    name(void) const override;
    /*! 
     * \return the group of the test
     */
    virtual std::string
    classname(void) const override;
    /*!
     * \param[in] f : file to be read
     */
    virtual void
    readInputFile(const std::string&);
    /*!
     * complete the initialisation. This method must be called once.
     * \note this method is called automatically by the execute method.
     */ 
    virtual void
    completeInitialisation(void) override;
    /*!
     * \brief initialize the current state
     * \param[in] s : current state
     */
    virtual void
    initializeCurrentState(StudyCurrentState&) const override;
    /*!
     * \brief initialize the workspace
     * \param[in] wk : workspace
     */
    virtual void
    initializeWorkSpace(SolverWorkSpace&) const override;
    /*!
     * integrate the behaviour
     * along the loading path
     */ 
    virtual tfel::tests::TestResult
    execute(void) override;
    /*!
     * \brief update current state at the beginning of a new time step:
     * - update the material properties
     * - update the external state variables
     * - compute the thermal expansion if mandatory
     * \param[out] state: current structure state
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
    virtual void
    makeLinearPrediction(StudyCurrentState&,
			 const real) const override;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \param[out] state: current structure state
     * \param[out] k:   tangent operator
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
     * \param[out] s:   current structure state
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
     * \param[in/out]  s: current structure state
     * \param[in/out] wk: solver workspace
     * \param[in] o:  solver options
     * \param[in] t:  current time
     * \param[in] dt: time increment
     */
    virtual void
    computeLoadingCorrection(StudyCurrentState&,
			     SolverWorkSpace&,
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
     * integrate the behaviour over one step
     * \param[out] s: current structure state
     * \param[out] wk: workspace
     * \param[in]  t:  current time
     * \param[in]  dt: time increment
     */ 
    virtual void
    execute(StudyCurrentState&,
	    SolverWorkSpace&,
	    const real,
	    const real) const;
    /*!
     * \brief ask the comparison to the numerical tangent operator
     * \param[in] bo : boolean
     */
    virtual void setCompareToNumericalTangentOperator(const bool);
    /*!
     * \brief set the value used to compare the tangent operator given
     * by the behaviour to the numerical tangent operator
     * \param[in] v : value
     */
    virtual void setTangentOperatorComparisonCriterium(const real);
    /*!
     * \brief set the value used to build the numerical tangent operator
     * \param[in] v : perturbation value
     */
    virtual void setNumericalTangentOperatorPerturbationValue(const real);
    /*!
     * \brief set the rotation matrix
     * \param[in] r : rotation matrix
     * \param[in] b : allow redefining the rotation matrix 
     */
    virtual void setRotationMatrix(const tfel::math::tmatrix<3u,3u,real>&,
				   const bool = false);
    /*!
     * \brief set criterium value for the convergence test on the on
     * the driving variable
     * \param[in] e : criterium
     */
    virtual void
    setDrivingVariableEpsilon(const real);
    /*!
     * \brief set criterium value for the convergence test on the
     * thermodynamic forces
     * \param[in] e : criterium
     */
    virtual void
    setThermodynamicForceEpsilon(const real);
    /*!
     * \return the type and the position of the given variable
     * \param[out] type : type of the variable
     * \param[out] pos  : position of the variable
     * \param[in]  n    : name of the variable
     */
    void
    getVariableTypeAndPosition(UTest::TestedVariable&,
			       unsigned short&,
			       const std::string&);
    /*!
     * \brief add a new test
     * \param[in] t : test to be added
     */
    virtual void
    addTest(std::shared_ptr<UTest>);
    /*!
     * \brief set the inital values of the driving variable
     * \param[in] v : values
     */
    virtual void
    setDrivingVariablesInitialValues(const std::vector<real>&);
    /*!
     * \brief set the inital values of the thermodynamic forces
     * \param[in] v : values
     */
    virtual void
    setThermodynamicForcesInitialValues(const std::vector<real>&);
    /*!
     * \brief add a new constraint
     * \param[in] c     : constraint
     */
    virtual void addConstraint(const std::shared_ptr<Constraint>);
    /*!
     * \brief print usefull information in the output file 
     * \param[in] t  : time
     * \param[in] state  : current state
     */
    virtual void printOutput(const real,const StudyCurrentState&) override;
    /*!
     * destructor
     */
    ~MTest();
  protected:
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultModellingHypothesis(void) override;
    /*!
     * \brief set the position of the Gauss point in the evolution
     * manager
     * \param[in] s: current state
     */
    virtual void
    setGaussPointPositionForEvolutionsEvaluation(const CurrentState&) const override;
    /*!
     * \return the number of unknowns (size of driving variables plus
     * the number of lagrangian multipliers)
     */
    virtual size_t getNumberOfUnknowns(void) const override;
    //! list of tests
    std::vector<std::shared_ptr<UTest>> tests;
    //! constraints
    std::vector<std::shared_ptr<Constraint>> constraints;
    //! rotation matrix
    tfel::math::tmatrix<3u,3u,real> rm;
    //! boolean, true if the rotation matrix has been defined by the user
    bool isRmDefined;
    // inital values of the driving variables
    std::vector<real> e_t0;
    // inital values of the thermodynamic forces
    std::vector<real> s_t0;
    //! tangent operator comparison criterium
    real toeps;
    //! perturbation value
    real pv;
    //! compare to numerical jacobian
    bool cto;
  }; // end of struct MTest

} // end of namespace mtest

#endif /* LIB_MTEST_MTEST_H_ */

