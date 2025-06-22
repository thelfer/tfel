/*!
 * \file   mtest/include/MTest/PipeTest.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24/11/2015
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

namespace tfel::utilities {
  // forward declaration
  struct TextData;
}  // namespace tfel::utilities

namespace mtest {

  // forward declarations
  struct PipeProfileHandler;
  struct GasEquationOfState;
  struct PipeFailureCriterion;
  struct OxidationStatusEvolution;

  //! \brief a study describing mechanical tests on pipes
  struct MTEST_VISIBILITY_EXPORT PipeTest : public SingleStructureScheme {
    //! \brief a simple alias
    using size_type = tfel::math::vector<real>::size_type;
    //! \brief type of axial loading
    enum AxialLoading {
      DEFAULTAXIALLOADING,
      ENDCAPEFFECT,
      IMPOSEDAXIALFORCE,
      IMPOSEDAXIALGROWTH,
      NONE
    };  // end of enum AxialLoading
    //! \brief type of radial loading
    enum RadialLoading {
      DEFAULTLOADINGTYPE,
      TIGHTPIPE,
      IMPOSEDPRESSURE,
      IMPOSEDINNERRADIUS,
      IMPOSEDOUTERRADIUS
    };  // end of enum AxialLoading
    //! \brief policy related to failure
    enum FailurePolicy {
      STOPCOMPUTATION,                 /*!<
                                        * \brief if a failure is detected, computations are
                                        * stopped.
                                        */
      REPORTONLY,                      /*!<
                                        * \brief failure detection is reported in the output file,
                                        * but computations are performed as usual.
                                        */
      FREEZESTATEUNTILENDOFCOMPUTATION /*!<
                                        * \brief if a failure is detected, the
                                        * state of the structure is freezed and
                                        * do not evolve. No equilibrium is
                                        * performed, the behaviour is no more
                                        * called and PipeTest will output the
                                        * same results again and again until the
                                        * end of computation.
                                        * This option may be useful when
                                        * optimizing material parameters.
                                        */
    };                                 // end of FailurePolicy
    //! \brief base class for tests
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
      //! \brief desctructor
      virtual ~UTest();
    };  // end of struct UnitTest
    //! \brief structure describing an oxidation model
    struct OxidationModel {
      //! \brief oxidation model
      std::shared_ptr<Behaviour> model;
      //! \brief default material properties
      std::shared_ptr<EvolutionManager> default_material_properties;
    };  // end of OxidationModel
    //! \brief default constructor
    PipeTest();
    //! \return the name of the test
    std::string name() const override;
    //! \return the group of the test
    std::string classname() const override;
    //! \brief return the mesh
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
     * \brief set the pipe axial loading
     * \param[in] al: axial loading
     */
    virtual void setAxialLoading(const AxialLoading);
    //! \brief \return the axial loading
    virtual AxialLoading getAxialLoading() const;
    /*!
     * \brief set the evolution of the radius of the mandrel
     * \param[in] r : evolution of the mandrel' radius
     */
    virtual void setMandrelRadiusEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the evolution of the axial growth of the mandrel
     * \param[in] ag : evolution of the axial growth
     */
    virtual void setMandrelAxialGrowthEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the inner pressure evolution
     * \param[in] p : pressure evolution
     */
    virtual void setInnerPressureEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the radial loading type
     * \param[in] t: radial loading type
     */
    virtual void setRadialLoading(const RadialLoading);
    //! \brief \return the radial loading type
    virtual RadialLoading getRadialLoading() const;
    /*!
     * \brief set the inner radius evolution
     * \param[in] p : radius evolution
     */
    virtual void setInnerRadiusEvolution(std::shared_ptr<Evolution>);
    /*!
     * \brief set the outer radius evolution
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
    //! \return the total number of unknowns
    size_type getNumberOfUnknowns() const override;
    //! \return the total number of nodes
    virtual size_type getNumberOfNodes() const;
    //
    void initializeCurrentState(StudyCurrentState&) const override;
    void initializeWorkSpace(SolverWorkSpace&) const override;
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
    void setModellingHypothesis(const std::string&) override;
    void setDefaultModellingHypothesis() override;
    /*!
     * \brief add a new profile postprocessing
     * \param[in] f: file name
     * \param[in] c: components
     */
    virtual void addProfile(const std::string&,
                            const std::vector<std::string>&);
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
     * \brief compute the minium and maximum values of a scalar
     * variable
     * \param[in] s: structure state
     * \param[in] e: function returning value at integration point
     */
    virtual std::pair<real, real> computeMinimumAndMaximumValues(
        const StudyCurrentState&,
        const std::function<real(const mtest::CurrentState&)>&) const;
    /*!
     * \brief compute the minimum value of a scalar variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     */
    virtual real computeMinimumValue(const StudyCurrentState&,
                                     const std::string&) const;
    /*!
     * \brief compute the minimum value of a scalar variable
     * \param[in] s: structure state
     * \param[in] e: function returning value at integration point
     */
    virtual real computeMinimumValue(
        const StudyCurrentState&,
        const std::function<real(const mtest::CurrentState&)>&) const;
    /*!
     * \brief compute the maximum value of a scalar variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     */
    virtual real computeMaximumValue(const StudyCurrentState&,
                                     const std::string&) const;
    /*!
     * \brief compute the maximum value of a scalar variable
     * \param[in] s: structure state
     * \param[in] e: function returning value at integration point
     */
    virtual real computeMaximumValue(
        const StudyCurrentState&,
        const std::function<real(const mtest::CurrentState&)>&) const;

    /*!
     *
     */
    enum struct Configuration { INTIAL_CONFIGURATION, CURRENT_CONFIGURATION };

    /*!
     * \brief compute the integral value of a scalar variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     * \param[in] c: enum element to choose the initial or final configuration
     */
    virtual real computeIntegralValue(
        const StudyCurrentState&,
        const std::string&,
        const Configuration = Configuration::INTIAL_CONFIGURATION) const;

    /*!
     * \brief compute the integral value of a scalar variable
     * \param[in] s: structure state
     * \param[in] e: function returning value at integration point
     * \param[in] c: enum element to choose the initial or final configuration
     */
    virtual real computeIntegralValue(
        const StudyCurrentState&,
        const std::function<real(const mtest::CurrentState&)>&,
        const Configuration = Configuration::INTIAL_CONFIGURATION) const;
    /*!
     * \brief compute the mean value of a scalar variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     * \param[in] c: enum element to choose the initial or final configuration
     */
    virtual real computeMeanValue(
        const StudyCurrentState&,
        const std::string&,
        const Configuration = Configuration::INTIAL_CONFIGURATION) const;
    /*!
     * \brief compute the mean value of a scalar variable
     * \param[in] s: structure state
     * \param[in] n: variable name
     * \param[in] c: enum element to choose the initial or final configuration
     */
    virtual real computeMeanValue(
        const StudyCurrentState&,
        const std::function<real(const mtest::CurrentState&)>&,
        const Configuration = Configuration::INTIAL_CONFIGURATION) const;
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
    /*!
     * \brief add a failure criterion
     * \param[in] n: name of the failure criterion
     * \param[in] opts: options used to initialize the failure criterion
     */
    void addFailureCriterion(const std::string&,
                             const tfel::utilities::DataMap&);
    /*!
     * \brief change the way a failure is treated
     * \param[in] p: failure policy
     */
    void setFailurePolicy(const FailurePolicy);
    /*!
     * \brief change the way a failure is treated
     * \param[in] library: library path
     * \param[in] model: name of the oxidation model
     * \param[in] boundary: boundary name. Must be `inner_boundary` or
     * `outer_boundary`
     */
    void addOxidationModel(const std::string&,
                           const std::string&,
                           const std::string&);
    //
    void completeInitialisation() override;
    //! \brief destructor
    ~PipeTest() override;

   protected:
    /*!
     * \brief check that the behaviour is consistent with the
     * modelling hypothesis
     * \param[in] bp : pointer to the behaviour
     */
    void checkBehaviourConsistency(const std::shared_ptr<Behaviour>&) override;

   private:
    //! \brief a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    /*!
     * \brief set the position of the Gauss point in the evolution
     * manager
     * \param[in] s: current state
     */
    void setGaussPointPositionForEvolutionsEvaluation(
        const CurrentState&) const override;
    //! \brief description of an additional
    struct AdditionalOutput {
      //! \brief description
      std::string d;
      //! \brief functor
      std::function<void(std::ostream&, const StudyCurrentState&)> f;
    };
    //! \brief additional outputs
    std::vector<AdditionalOutput> aoutputs;
    //! \brief failure criteria
    std::vector<std::unique_ptr<PipeFailureCriterion>> failure_criteria;
    //! \brief list of tests
    std::vector<std::shared_ptr<UTest>> tests;
    //! \brief registred profile
    std::vector<PipeProfileHandler> profiles;
    //! \brief mesh data
    PipeMesh mesh;
    //! \brief oxidation model at the inner boundary
    OxidationModel inner_boundary_oxidation_model;
    //! \brief oxidation model at the outer boundary
    OxidationModel outer_boundary_oxidation_model;
    //! \brief evolution defining the oxidation status
    std::shared_ptr<OxidationStatusEvolution> oxidation_status_evolution;
    //! \brief user defined gas equation of state
    std::unique_ptr<GasEquationOfState> gseq;
    //! \brief inner radius evolution
    std::shared_ptr<Evolution> inner_radius_evolution;
    //! \brief outer radius evolution
    std::shared_ptr<Evolution> outer_radius_evolution;
    //! \brief axial growth evolution
    std::shared_ptr<Evolution> axial_growth_evolution;
    //! \brief evolution of the radius of the mandrel
    std::shared_ptr<Evolution> mandrel_radius_evolution;
    //! \brief mandrel axial growth evolution
    std::shared_ptr<Evolution> mandrel_axial_growth_evolution;
    //! \brief axial force evolution
    std::shared_ptr<Evolution> axial_force_evolution;
    //! \brief inner pressure
    std::shared_ptr<Evolution> inner_pressure_evolution;
    //! \brief outer pressure
    std::shared_ptr<Evolution> outer_pressure_evolution;
    //! \brief initial number of moles (tight pipe modelling)
    real n0 = -1;
    //! \brief initial pressure (tight pipe modelling)
    real P0 = -1;
    //! \brief initial temperature (tight pipe modelling)
    real T0 = -1;
    //! \brief pipe modelling hypothesis
    RadialLoading rl = DEFAULTLOADINGTYPE;
    //! \brief axial modelling hypothesis
    AxialLoading al = DEFAULTAXIALLOADING;
    //! \brief failure policy
    FailurePolicy failure_policy = REPORTONLY;
    //! \brief element type
    //! \brief small strain hypothesis
    bool hpp = false;
  };  // end of struct PipeTest

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPETEST_HXX */
