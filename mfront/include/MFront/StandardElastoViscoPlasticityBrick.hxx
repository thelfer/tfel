/*!
 * \file   mfront/includes/MFront/StandardElastoViscoPlasticityBrick.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX
#define LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX

#include <map>
#include <string>
#include <memory>
#include "MFront/CodeBlock.hxx"
#include "MFront/BehaviourBrickBase.hxx"
#include "MFront/BehaviourBrick/PorosityEvolutionAlgorithm.hxx"

namespace mfront {

  namespace bbrick {
    // forward declaration
    struct StressPotential;
    // forward declaration
    struct InelasticFlow;
    // forward declaration
    struct PorosityNucleationModel;
  }  // end of namespace bbrick

  /*!
   * \brief The `StandardElastoViscoPlasticityBrick` is meant to describe a
   * behaviour based on:
   * - a stress potential describing how the stress is computed. The most
   *   used one will probably be the `Hooke` stress potential.
   * - a list of inelastic flows which describe how inelastic strains evolve.
   */
  struct StandardElastoViscoPlasticityBrick : public BehaviourBrickBase {
    /*!
     * \brief if the staggered algorithm is used to treat the porosity
     * evolution, this variable gives the name of a boolean variable which can
     * is false during the iteration of the staggered algorithm and set to true
     * once the staggered algorithm has converged.
     *
     * The rationale behing this is that the exact jacobian of the full system
     * of implicit equations is required to compute the consistent tangent
     * operator. Thus, once the staggered algorithm has converged, this jacobian
     * must be built. This flag allows to switch from a reduced system of
     * equations to the full system of equations.
     */
    static const char* const computeStandardSystemOfImplicitEquations;
    /*!
     * \brief if the staggered approach is used, this is the name of an
     * auxiliary state variable which states if the material point is
     * totally damaged. If this variable is equal to 0, then the material is
     * still able to sustain stress. If this variable is equal to 1, the
     * material is totally damaged.
     */
    static const char* const brokenVariable;
    /*!
     * \brief if the staggered approach is used, this is the name of a
     * local variable which states if the staggered approach succeeded.
     */
    static const char* const fixedPointConverged;
    /*!
     * \brief if the staggered approach is used, this is the name of a
     * local variable which holds the current estimate of the porosity
     * increment.
     */
    static const char* const currentEstimateOfThePorosityIncrement;
    /*!
     * \brief if the staggered approach is used, this is the name of a variable
     * which holds a next estimate of the porosity increment.
     */
    static const char* const nextEstimateOfThePorosityIncrement;
    /*!
     * \brief if the staggered approach is used, this is the name of a variable
     * which holds a next estimate of the porosity at the end of the time step.
     */
    static const char* const nextEstimateOfThePorosityAtTheEndOfTheTimeStep;
    /*!
     * \brief this is the name of a variable which holds the upper bound of the
     * porosity.
     */
    static const char* const porosityUpperBound;
    /*!
     * \brief this is the name of a parameter which holds a safety factor for
     * the porosity upper bound, because most of the stress criteria becomes
     * singular when the associated upper bound is reached.
     */
    static const char* const porosityUpperBoundSafetyFactor;
    /*!
     * \brief this is the name of a parameter which holds a safety factor for
     * the porosity upper bound when trying to detect the fracture of the
     * material.
     */
    static const char* const porosityUpperBoundSafetyFactorForFractureDetection;
    /*!
     * \brief if the staggered approach is used, this is the name of a
     * variable
     * which holds the difference between the new and the current estimates
     * of
     * the porosity increment.
     */
    static const char* const
        differenceBetweenSuccessiveEstimatesOfThePorosityIncrement;
    /*!
     * \brief if the staggered approach is used, this is the name of a parameter
     * which holds the value of criterion on the difference between the new and
     * the current estimates of the porosity increment used to stop the
     * staggered scheme.
     */
    static const char* const staggeredSchemeConvergenceCriterion;
    /*!
     * \brief if the staggered approach is used, this is the name of a local
     * variable which holds the number of iterations of the staggered scheme
     * already performed.
     */
    static const char* const staggeredSchemeIterationCounter;
    /*!
     * \brief if the staggered approach is used, this is the name of a parameter
     * which holds the maximum number of iterations of the staggered scheme.
     */
    static const char* const maximumNumberOfIterationsOfTheStaggeredScheme;
    /*!
     * \brief if the staggered approach is used, this is the name of a
     * instance of the `tfel::math::BissectionAlgorithmBase` class.
     */
    static const char* const staggeredSchemeBissectionAlgorithm;
    /*!
     * \brief if the staggered approach is used and that the acceleration
     * algorithm used is the Aitken one, this is the name of a
     * instance of the `tfel::math::AitkenAccelerationAlgorithm` class.
     */
    static const char* const staggeredSchemeAitkenAccelerationAlgorithm;
    /*!
     * \brief constructor
     * \param[in] dsl_ : calling domain specific language
     * \param[in] bd_  : behaviour description
     */
    StandardElastoViscoPlasticityBrick(AbstractBehaviourDSL&,
                                       BehaviourDescription&);
    std::string getName() const override;
    BehaviourBrickDescription getDescription() const override;
    std::vector<bbrick::OptionDescription> getOptions(
        const bool) const override;
    void initialize(const Parameters&, const DataMap&) override;
    std::vector<Hypothesis> getSupportedModellingHypotheses() const override;
    void completeVariableDeclaration() const override;
    void endTreatment() const override;
    //! destructor
    ~StandardElastoViscoPlasticityBrick() override;

   private:
    /*!
     * \brief method part of the `initialize` method which treats the
     * `porosity_evolution` section.
     * \param[in] e: data declared in the `porosity_evolution` section.
     * \return if the porosity evolution
     */
    bool treatPorosityEvolutionSection(const Data&);
    /*!
     * \brief method part of the `treatPorosityEvolutionAlgorithmSection` method
     * which treats the `algorithm` subsection of the `porosity_evolution`
     * section.
     * \param[in] d: data declared in the algorithm subsection of the
     * `porosity_evolution` section.
     */
    void treatPorosityEvolutionAlgorithmSection(const Data&);
    /*!
     * \brief extract the parameters associated with the staggered scheme
     * \param[in] algorithm_parameters: parameters
     */
    void treatStaggeredPorosityEvolutionAlgorithmParameters(const DataMap&);
    //! \return if a coupling with the porosity evolution is required
    bool isCoupledWithPorosityEvolution() const;
    /*!
     * \brief add the contribution of this inelastic flow to the implicit
     * equation associated with the porosity evolution.
     * \param[in] ib: integrator code block
     */
    void
    addElasticContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
        CodeBlock&) const;
    //! \return a map associating a map and its idea
    std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>
    buildInelasticFlowsMap() const;
    //! \brief stress potential
    std::shared_ptr<bbrick::StressPotential> stress_potential;
    //! \brief inelastic flows
    std::vector<std::shared_ptr<bbrick::InelasticFlow>> flows;
    //! \brief nucleation models
    std::vector<std::shared_ptr<bbrick::PorosityNucleationModel>>
        nucleation_models;
    /*!
     * \brief porosity growth policy
     *
     * By default, no porosity growth is considered if no inelastic flow is
     * coupled with the porosity evolution, even though some inelastic flows
     * are not purely deviatoric.
     *
     * However, if a `porosity_evolution` section is defined, then standard
     * growth of the porosity is considered, i.e. each inelastic flow
     * which is not purely deviatoric contributes to the porosity growth by
     * the standard term:
     * \f[
     * \left(1-f\right)\,\mathop{trace}\left(\underline{\dot{\varepsilon}}^{p}\)
     * \f]
     */
    enum {
      UNDEFINEDPOROSITYGROWTHPOLICY,
      STANDARDVISCOPLASTICPOROSITYGROWTHPOLICY
    } porosity_growth_policy = UNDEFINEDPOROSITYGROWTHPOLICY;
    //! \brief algorithm used to handle the porosity evolution
    mfront::bbrick::PorosityEvolutionAlgorithm porosity_evolution_algorithm =
        mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME;
    //! \brief structure holding the parameters for the staggered scheme
    struct StaggeredSchemeParameters {
      /*!
       * \brief value used to check if the values of the porosity between two
       * iterations of the staggered scheme are almost identical, i.e. that
       * the
       * value of the porosity has become stationnary.
       */
      double convergence_criterion = 1e-10;
      //! \brief maximum number of iterations of the staggered scheme
      unsigned short maximum_number_of_iterations = 100;
      /*!
       * \brief list of possible acceleration algorithm
       */
      enum AccelerationAlgorithm {
        RELAXATION,
        AITKEN
      };  // end of enum AccelerationAlgorithm
          //! \brief selected acceleration algorithm
      AccelerationAlgorithm acceleration_algorithm = AITKEN;
    };  // end of  struct StaggeredSchemeParameters
    //! \brief parameters for the staggered scheme
    StaggeredSchemeParameters staggered_scheme_parameters;
    /*!
     * \brief a boolean stating if the elastic contribution to the porosity
     * growth must be taken into account.
     */
    bool elastic_contribution = false;
  };  // end of StandardElastoViscoPlasticityBrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_STANDARDELASTOVISCOPLASTICITYBRICK_HXX */
