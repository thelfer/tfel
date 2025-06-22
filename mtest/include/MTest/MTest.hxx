/*!
 * \file  mtest/include/MTest/MTest.hxx
 * \brief
 * \author Thomas Helfer
 * \date 09 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_H
#define LIB_MTEST_MTEST_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/LUSolve.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/SolverWorkSpace.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/AccelerationAlgorithm.hxx"
#include "MTest/SingleStructureScheme.hxx"

namespace mtest {

  /*!
   * \brief MTest is a simple class to test mfront behaviours.
   *
   * The unknowns are made of the driving variables stensor (for small
   * strain behaviours) followed by a number of lagrange multiplier.
   */
  struct MTEST_VISIBILITY_EXPORT MTest : public SingleStructureScheme {
    /*!
     * \brief base class for testing the behaviour outputs
     */
    struct UTest {
      /*!
       * \param[in] s:  state
       * \param[in] t:  time
       * \param[in] dt: time increment
       * \param[in] p:  period
       */
      virtual void check(const CurrentState&,
                         const real,
                         const real,
                         const unsigned int) = 0;
      /*!
       * \return the results of the test
       */
      virtual tfel::tests::TestResult getResults() const = 0;
      //! desctructor
      virtual ~UTest();
    };
    /*!
     * default constructor
     */
    MTest();
    /*!
     * \return the name of the test
     */
    virtual std::string name() const override;
    /*!
     * \return the group of the test
     */
    virtual std::string classname() const override;
    /*!
     * \param[in] f : file to be read
     * \param[in] ecmds: external commands
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    virtual void readInputFile(const std::string&,
                               const std::vector<std::string>&,
                               const std::map<std::string, std::string>&);
    /*!
     * complete the initialisation. This method must be called once.
     * \note this method is called automatically by the execute method.
     */
    virtual void completeInitialisation() override;
    /*!
     * \brief initialize the current state
     * \param[in] s : current state
     */
    virtual void initializeCurrentState(StudyCurrentState&) const override;
    /*!
     * \brief initialize the workspace
     * \param[in] wk : workspace
     */
    virtual void initializeWorkSpace(SolverWorkSpace&) const override;
    /*!
     * integrate the behaviour
     * along the loading path
     */
    virtual tfel::tests::TestResult execute() override;
    /*!
     * \brief update current state at the beginning of a new time step:
     * - update the material properties
     * - update the external state variables
     * - compute the thermal expansion if mandatory
     * \param[out] state: current structure state
     * \param[in]  t: current time
     * \param[in] dt: time increment
     */
    virtual void prepare(StudyCurrentState&,
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
     * \return a pair containing:
     * - a boolean syaing if the behaviour integration shall be
     *   performed
     * - a scaling factor that can be used to:
     *     - increase the time step if the integration was successfull
     *     - decrease the time step if the integration failed or if the
     *       results were not reliable (time step too large).
     * \param[out] state: current structure state
     * \param[out] k:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    virtual std::pair<bool, real> computePredictionStiffnessAndResidual(
        StudyCurrentState&,
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        const real&,
        const real&,
        const StiffnessMatrixType) const override;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \return a pair containing:
     * - a boolean syaing if the behaviour integration shall be
     *   performed
     * - a scaling factor that can be used to:
     *     - increase the time step if the integration was successfull
     *     - decrease the time step if the integration failed or if the
     *       results were not reliable (time step too large).
     * \param[out] s:   current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    virtual std::pair<bool, real> computeStiffnessMatrixAndResidual(
        StudyCurrentState&,
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        const real,
        const real,
        const StiffnessMatrixType) const override;
    /*!
     * \param[in] : du unknows increment difference between two iterations
     */
    virtual real getErrorNorm(const tfel::math::vector<real>&) const override;
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
    virtual bool checkConvergence(const StudyCurrentState&,
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
    virtual std::vector<std::string> getFailedCriteriaDiagnostic(
        const StudyCurrentState&,
        const tfel::math::vector<real>&,
        const tfel::math::vector<real>&,
        const SolverOptions&,
        const real,
        const real) const override;
    /*!
     * \param[in,out]  s: current structure state
     * \param[in,out] wk: solver workspace
     * \param[in] o:  solver options
     * \param[in] t:  current time
     * \param[in] dt: time increment
     */
    virtual void computeLoadingCorrection(StudyCurrentState&,
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
    virtual void postConvergence(StudyCurrentState&,
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
    virtual void execute(StudyCurrentState&,
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
    virtual void setRotationMatrix(const tfel::math::tmatrix<3u, 3u, real>&,
                                   const bool = false);
    /*!
     * \brief set criterium value for the convergence test on the on
     * the driving variable
     * \param[in] e : criterium
     */
    virtual void setDrivingVariableEpsilon(const real);
    /*!
     * \brief set criterium value for the convergence test on the
     * thermodynamic forces
     * \param[in] e : criterium
     */
    virtual void setThermodynamicForceEpsilon(const real);
    /*!
     * \brief add a new test
     * \param[in] t : test to be added
     */
    virtual void addTest(std::shared_ptr<UTest>);
    /*!
     * \brief set the inital values of the driving variable
     * \param[in] v : values
     */
    virtual void setDrivingVariablesInitialValues(const std::vector<real>&);
    /*!
     * \brief set the inital values of the thermodynamic forces
     * \param[in] v : values
     */
    virtual void setThermodynamicForcesInitialValues(const std::vector<real>&);
    /*!
     * \brief add a new constraint
     * \param[in] c     : constraint
     */
    virtual void addConstraint(const std::shared_ptr<Constraint>);
    /*!
     * \brief print usefull information in the output file
     * \param[in] t: time
     * \param[in] s: current state
     * \param[in] o: if true, this time has been specified by the
     * user. Otherwise, it has been reached due to sub-stepping.
     */
    virtual void printOutput(const real,
                             const StudyCurrentState&,
                             const bool) const override;
    //! destructor
    ~MTest();

   protected:
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultModellingHypothesis() override;
    /*!
     * \brief set the position of the Gauss point in the evolution
     * manager
     * \param[in] s: current state
     */
    virtual void setGaussPointPositionForEvolutionsEvaluation(
        const CurrentState&) const override;
    /*!
     * \return the number of unknowns (size of driving variables plus
     * the number of lagrangian multipliers)
     */
    virtual size_t getNumberOfUnknowns() const override;
    //! list of tests
    std::vector<std::shared_ptr<UTest>> tests;
    //! constraints
    std::vector<std::shared_ptr<Constraint>> constraints;
    //! rotation matrix
    tfel::math::tmatrix<3u, 3u, real> rm;
    //! boolean, true if the rotation matrix has been defined by the user
    bool isRmDefined = false;
    // inital values of the driving variables
    std::vector<real> e_t0;
    // inital values of the thermodynamic forces
    std::vector<real> s_t0;
    //! tangent operator comparison criterium
    real toeps = -1;
    //! perturbation value
    real pv = -1;
    //! compare to numerical jacobian
    bool cto = false;
  };  // end of struct MTest

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_H */
