/*!
 * \file   mtest/include/MTest/PipeTest.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPETEST_HXX
#define LIB_MTEST_PIPETEST_HXX

#include <string>
#include <vector>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/PipeProfileHandler.hxx"
#include "MTest/PipeMesh.hxx"
#include "MTest/SingleStructureScheme.hxx"

namespace tfel {
  namespace utilities {
    // forward declaration
    struct TextData;
  }  // namespace utilities
}  // namespace tfel

namespace mtest {

  // forward declaration
  struct PipeProfileHandler;
  // forward declaration
  struct GasEquationOfState;

  /*!
   * a study describing mechanical tests on pipes
   */
  struct MTEST_VISIBILITY_EXPORT PipeTest : public SingleStructureScheme {
    //! how the pipe is modelled
    enum AxialLoading {
      DEFAULTAXIALLOADING,
      ENDCAPEFFECT,
      IMPOSEDAXIALFORCE,
      IMPOSEDAXIALGROWTH,
      NONE
    };  // end of enum AxialLoading
    //! how the pipe is modelled
    enum RadialLoading {
      DEFAULTLOADINGTYPE,
      TIGHTPIPE,
      IMPOSEDPRESSURE,
      IMPOSEDOUTERRADIUS
    };  // end of enum AxialLoading
    //! a simple alias
    using size_type = tfel::math::vector<real>::size_type;
    /*!
     * \brief base class for tests
     */
    struct UTest {
      /*!
       * \param[in] s  : current state
       * \param[in] t  : time
       * \param[in] dt : time increment
       * \param[in] p  : period
       */
      virtual void check(const StudyCurrentState&,
                         const real,
                         const real,
                         const unsigned int) = 0;
      /*!
       * \return the results of the test
       */
      virtual tfel::tests::TestResult getResults() const = 0;
      //! desctructor
      virtual ~UTest();
    };  // end of struct UnitTest
    //! default constructor
    PipeTest();
    /*!
     * \return the name of the test
     */
    std::string name() const override;
    /*!
     * \return the group of the test
     */
    std::string classname() const override;
    //! return the mesh
    const PipeMesh& getMesh() const;
    /*!
     * \brief integrate the behaviour along the loading path
     * \param[in] bInit: if true, call the completeInitialisationMethod
     */
    virtual tfel::tests::TestResult execute(const bool);
    /*!
     * \brief integrate the behaviour along the loading path
     * \note this is equivalent to `execute(true)`
     */
    tfel::tests::TestResult execute() override;
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
     * \brief set the pipe inner radius
     * \param[in] r: radius value
     */
    virtual void setInnerRadius(const real);
    /*!
     * \brief set the pipe outer radius
     * \param[in] r: radius value
     */
    virtual void setOuterRadius(const real);
    /*!
     * \brief set the number of elements
     * \param[in] n: number of elements
     */
    virtual void setNumberOfElements(const int);
    /*!
     * \brief set the element type
     * \param[in] e: element type
     */
    virtual void setElementType(const PipeMesh::ElementType);
    /*!
     * \brief set the pipe modelling hypothesis
     * \param[in] ph: pipe modelling hypothesis
     */
    virtual void setAxialLoading(const AxialLoading);
    /*!
     * \brief set the inner pressure evolution
     * \param[in] p : pressure evolution
     */
    virtual void setInnerPressureEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the loading type
     * \param[in] t: loading type
     */
    virtual void setRadialLoading(const RadialLoading);
    /*!
     * \brief set the inner radius evolution
     * \param[in] p : radius evolution
     */
    virtual void setOuterRadiusEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the outer pressure evolution
     * \param[in] p : pressure evolution
     */
    virtual void setOuterPressureEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the axial force evolution
     * \param[in] f : axial force evolution
     */
    virtual void setAxialForceEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the axial growth evolution
     * \param[in] f : axial growth evolution
     */
    virtual void setAxialGrowthEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the filling pressure evolution
     * \param[in] p : filling pressure evolution
     */
    virtual void setFillingPressure(const real);
    /*!
     * \brief set the filling temperature evolution
     * \param[in] p : filling temperature evolution
     */
    virtual void setFillingTemperature(const real);
    /*!
     * \brief set the gas equation of state
     * \param[in] e: equation of state
     */
    virtual void setGasEquationOfState(const std::string&);
    /*!
     * \brief set displacement criterion value
     * \param[in] e: criterion value
     */
    virtual void setDisplacementEpsilon(const real);
    /*!
     * \brief set criterium value for the convergence test on the
     * residual
     * \param[in] e : criterion value
     */
    virtual void setResidualEpsilon(const real);
    /*!
     * \return the total number of unknowns
     */
    size_type getNumberOfUnknowns() const override;
    /*!
     * \return the total number of nodes
     */
    virtual size_type getNumberOfNodes() const;
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
     * \brief update current state at the beginning of a new time step:
     * - update the material properties
     * - update the external state variables
     * - compute the thermal expansion if mandatory
     * \param[out] s: current structure state
     * \param[in]  t: current time
     * \param[in] dt: time increment
     */
    void prepare(StudyCurrentState&, const real, const real) const override;
    /*!
     * \brief make a linear prediction of the unknows and state
     * \param[out] s: current structure state
     * \param[in] dt: time increment
     */
    void makeLinearPrediction(StudyCurrentState&, const real) const override;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \return a pair containing:
     * - a boolean syaing if the behaviour integration shall be
     *   performed
     * - a scaling factor that can be used to:
     *     - increase the time step if the integration was successfull
     *     - decrease the time step if the integration failed or if the
     *       results were not reliable (time step too large).
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    std::pair<bool, real> computePredictionStiffnessAndResidual(
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
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    std::pair<bool, real> computeStiffnessMatrixAndResidual(
        StudyCurrentState&,
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        const real,
        const real,
        const StiffnessMatrixType) const override;
    /*!
     * \param[in] : du unknows increment difference between two iterations
     */
    real getErrorNorm(const tfel::math::vector<real>&) const override;
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
    bool checkConvergence(const StudyCurrentState&,
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
    std::vector<std::string> getFailedCriteriaDiagnostic(
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
    void computeLoadingCorrection(StudyCurrentState&,
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
    void postConvergence(StudyCurrentState&,
                         const real,
                         const real,
                         const unsigned int) const override;
    /*!
     * \brief add a new profile postprocessing
     * \param[in] f: file name
     * \param[in] c: components
     */
    virtual void addProfile(const std::string&,
                            const std::vector<std::string>&);
    /*!
     * \param[in] h : modelling hypothesis
     */
    void setModellingHypothesis(const std::string&) override;
    //! \brief set the modelling hypothesis to the default one
    void setDefaultModellingHypothesis() override;
    //! \brief turn hpp to true
    virtual void performSmallStrainAnalysis();
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
     * \brief compute the minium and maximum values of a scalar
     * variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     */
    virtual std::pair<real, real> computeMinimumAndMaximumValues(
        const StudyCurrentState&, const std::string&) const;
    /*!
     * \brief compute the maximum value of a scalar variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     */
    virtual real computeMinimumValue(const StudyCurrentState&,
                                     const std::string&) const;
    /*!
     * \brief compute the maximum value of a scalar variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     */
    virtual real computeMaximumValue(const StudyCurrentState&,
                                     const std::string&) const;
    /*!
     * \brief add a test comparing to results stored in a reference
     * file to the computed ones
     * \param[in] n: named of the variable tested
     * \param[in] d: data
     * \param[in] c: column
     * \param[in] e: criterion value
     */
    virtual void addIntegralTest(const std::string&,
                                 const tfel::utilities::TextData&,
                                 const unsigned short,
                                 const real);
    /*!
     * \brief add a test comparing to results stored in a reference
     * file to the computed ones
     * \param[in] n: named of the variable tested
     * \param[in] d: data
     * \param[in] c: column
     * \param[in] e: criterion value
     */
    virtual void addProfileTest(const std::string&,
                                const tfel::utilities::TextData&,
                                const unsigned short,
                                const real);
    /*!
     * \brief add a output
     * \param[in] t: type of output (minimum_value, maximum_value,
     *               minimum_and_maximum_value)
     * \param[in] n: name of the output
     */
    virtual void addOutput(const std::string&, const std::string&);
    void completeInitialisation() override;
    //! destructor
    ~PipeTest() override;

   protected:
    /*!
     * \brief check that the behaviour is consistent with the
     * modelling hypothesis
     * \param[in] bp : pointer to the behaviour
     */
    void checkBehaviourConsistency(const std::shared_ptr<Behaviour>&) override;

   private:
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    /*!
     * \brief set the position of the Gauss point in the evolution
     * manager
     * \param[in] s: current state
     */
    void setGaussPointPositionForEvolutionsEvaluation(
        const CurrentState&) const override;
    //! description of an additional
    struct AdditionalOutput {
      //! description
      std::string d;
      //! functor
      std::function<void(std::ostream&, const StudyCurrentState&)> f;
    };
    //! additional outputs
    std::vector<AdditionalOutput> aoutputs;
    //! list of tests
    std::vector<std::shared_ptr<UTest>> tests;
    //! registred profile
    std::vector<PipeProfileHandler> profiles;
    //! mesh data
    PipeMesh mesh;
    //! user defined gas equation of state
    std::unique_ptr<GasEquationOfState> gseq;
    //! outer radius evolution
    std::shared_ptr<Evolution> orev;
    //! axial growth evolution
    std::shared_ptr<Evolution> axial_growth;
    //! axial force evolution
    std::shared_ptr<Evolution> axial_force;
    //! inner pressure
    std::shared_ptr<Evolution> inner_pressure;
    //! outer pressure
    std::shared_ptr<Evolution> outer_pressure;
    //! initial number of moles (tight pipe modelling)
    real n0 = -1;
    //! initial pressure (tight pipe modelling)
    real P0 = -1;
    //! initial temperature (tight pipe modelling)
    real T0 = -1;
    //! pipe modelling hypothesis
    RadialLoading rl = DEFAULTLOADINGTYPE;
    //! axial modelling hypothesis
    AxialLoading al = DEFAULTAXIALLOADING;
    //! element type
    //! small strain hypothesis
    bool hpp = false;
  };  // end of struct PipeTest

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPETEST_HXX */
