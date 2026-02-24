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

#ifndef LIB_MTEST_MTEST_HXX
#define LIB_MTEST_MTEST_HXX

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

  //! forward declaration
  struct UserDefinedPostProcessing;

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
    std::string name() const override;
    /*!
     * \return the group of the test
     */
    std::string classname() const override;
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
    void completeInitialisation() override;
    /*!
     * \brief initialize the current state
     * \param[in] s : current state
     */
    void initializeCurrentState(StudyCurrentState&) const override;
    /*!
     * \brief initialize the workspace
     * \param[in] wk : workspace
     */
    void initializeWorkSpace(SolverWorkSpace&) const override;
    /*!
     * \brief integrate the behaviour along the loading path
     * \param[in] bInit: if true, call the completeInitialisationMethod
     */
    virtual tfel::tests::TestResult execute(const bool);
    //
    tfel::tests::TestResult execute() override;
    [[nodiscard]] std::pair<bool, real> prepare(StudyCurrentState&,
                                                const real,
                                                const real) const override;
    void makeLinearPrediction(StudyCurrentState&, const real) const override;
    [[nodiscard]] std::pair<bool, real> computePredictionStiffnessAndResidual(
        StudyCurrentState&,
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        const real&,
        const real&,
        const StiffnessMatrixType) const override;
    [[nodiscard]] std::pair<bool, real> computeStiffnessMatrixAndResidual(
        StudyCurrentState&,
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        const real,
        const real,
        const StiffnessMatrixType) const override;
    [[nodiscard]] real getErrorNorm(
        const tfel::math::vector<real>&) const override;
    [[nodiscard]] bool checkConvergence(StudyCurrentState&,
                                        const tfel::math::vector<real>&,
                                        const tfel::math::vector<real>&,
                                        const SolverOptions&,
                                        const unsigned int,
                                        const real,
                                        const real) const override;
    [[nodiscard]] std::vector<std::string> getFailedCriteriaDiagnostic(
        const StudyCurrentState&,
        const tfel::math::vector<real>&,
        const tfel::math::vector<real>&,
        const SolverOptions&,
        const real,
        const real) const override;
    void computeLoadingCorrection(StudyCurrentState&,
                                  SolverWorkSpace&,
                                  const SolverOptions&,
                                  const real,
                                  const real) const override;
    [[nodiscard]] bool postConvergence(StudyCurrentState&,
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
                         const real);
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
    virtual void setTangentOperatorComparisonCriterion(const real);
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
    virtual void setGradientEpsilon(const real);
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
    virtual void setGradientsInitialValues(const std::vector<real>&);
    /*!
     * \brief set the inital values of the thermodynamic forces
     * \param[in] v : values
     */
    virtual void setThermodynamicForcesInitialValues(const std::vector<real>&);
    /*!
     * \brief set if the lagrange multipliers must be printed in the output file
     * \param[in] b : boolean
     */
    virtual void printLagrangeMultipliers(const bool);
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
    void printOutput(const real,
                     const StudyCurrentState&,
                     const bool) const override;
    /*!
     * \brief add a new event
     * \param[in] e: event name
     * \param[in] evtimes: times
     */
    void addEvent(const std::string&, const std::vector<double>&);
    /*!
     * \brief add a user defined PostProcessing
     * \param[in] f: filename
     * \param[in] p: list of PostProcessings
     */
    void addUserDefinedPostProcessing(const std::string&,
                                      const std::vector<std::string>&);
    //! destructor
    ~MTest() override;

   protected:
    //! \brief set the modelling hypothesis to the default one
    void setDefaultModellingHypothesis() override;
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
    //! \brief list of events
    std::map<double, std::vector<std::string>> events;
    //! \brief list of tests
    std::vector<std::shared_ptr<UTest>> tests;
    //! \brief constraints
    std::vector<std::shared_ptr<Constraint>> constraints;
    //! \brief user defined post-processings
    std::vector<std::shared_ptr<UserDefinedPostProcessing>> upostprocessings;
    //! \brief rotation matrix
    tfel::math::tmatrix<3u, 3u, real> rm;
    //! \brief boolean, true if the rotation matrix has been defined by the user
    bool isRmDefined = false;
    // inital values of the driving variables
    std::vector<real> e_t0;
    // inital values of the thermodynamic forces
    std::vector<real> s_t0;
    //! \brief tangent operator comparison criterium
    real toeps = -1;
    //! \brief perturbation value
    real pv = -1;
    //! \brief compare to numerical jacobian
    bool cto = false;
    //! \brief print lagrange multpliers
    bool shallPrintLagrangeMultipliers = false;
  };  // end of struct MTest

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_HXX */
