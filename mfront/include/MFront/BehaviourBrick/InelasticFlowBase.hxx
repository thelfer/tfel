/*!
 * \file   include/MFront/BehaviourBrick/InelasticFlowBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWBASE_HXX

#include <vector>
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"
#include "MFront/BehaviourBrick/PorosityEvolutionAlgorithm.hxx"
#include "MFront/BehaviourBrick/InelasticFlow.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief an helper class which provide common method for all inelastic
     * potentials.
     */
    struct InelasticFlowBase : InelasticFlow {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;

      void setPorosityEvolutionHandled(const bool) override;

      bool isCoupledWithPorosityEvolution() const override;

      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const StressPotential&,
                        const std::string&) const override;

      void completeVariableDeclaration(BehaviourDescription&,
                                       const AbstractBehaviourDSL&,
                                       const std::string&) const override;
      std::vector<OptionDescription> getOptions() const override;
      bool requiresActivationState() const override;
      void computeInitialActivationState(BehaviourDescription&,
                                         const StressPotential&,
                                         const std::string&) const override;
      std::string updateNextEstimateOfThePorosityIncrement(
          const BehaviourDescription&, const std::string&) const override;
      /*!
       * \return the code updating the upper bound of the porosity.
       * If this flow does not affect the porosity growth, the returned
       * value may be empty. By default, this method calls the
       * `updatePorosityUpperBound` of the underlying stress criterion.
       * \param[in] bd: behaviour description
       * \param[in] id: flow id
       */
      std::string updatePorosityUpperBound(const BehaviourDescription&,
                                           const std::string&) const override;
      //! destructor
      ~InelasticFlowBase() override;

     protected:
      //! \brief policy on how the porosity affects the flow rule
      enum PorosityEffectOnFlowRule {
        /*!
         * \brief default choice. The effect of the porosity depends
         * on the stress criterion (see the
         * `StressCriterion::getPorosityEffectOnEquivalentPlasticStrain`
         * method).
         */
        UNDEFINED_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN,
        /*!
         * \brief This value indicate that the flow rule is not affected by the
         * porosity evolution.
         */
        NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN,
        /*!
         * \brief This value indicate that the flow rule must be corrected by
         * the standard \f$(1-f)\f$ factor where \f$f\f$ is the porosity.
         */
        STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN
      };
      //! \return the effect of the porosity on the flow rule.
      virtual PorosityEffectOnFlowRule
      getPorosityEffectOnEquivalentPlasticStrain() const;
      //! \return if this flow contributes to porosity growth
      virtual bool contributesToPorosityGrowth() const;
      /*!
       * \brief add the contribution of this inelastic flow to the implicit
       * equation associated with the porosity evolution.
       * \param[in] ib: integrator code block
       * \param[in] dsl: abstract behaviour dsl
       * \param[in] bd: behaviour description
       * \param[in] sp: stress potential
       * \param[in] id: flow id
       */
      virtual void
      addFlowContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
          CodeBlock&,
          const BehaviourDescription&,
          const AbstractBehaviourDSL&,
          const StressPotential&,
          const std::string&) const;
      /*!
       * \brief compute the effective stress at \f$t+\theta\,dt\f$.
       * \param[in] id: flow id
       */
      virtual std::string computeEffectiveStress(const std::string& id) const;
      /*!
       * \param[in] bd: behaviour description
       * \param[in] sp: stress potential
       * \param[in] id: flow id
       * \param[in] b: tells if an analytical jacobian is required
       */
      virtual std::string buildFlowImplicitEquations(
          const BehaviourDescription&,
          const StressPotential&,
          const std::string&,
          const bool) const = 0;
      //! stress criterion
      std::shared_ptr<StressCriterion> sc;
      //! flow criterion
      std::shared_ptr<StressCriterion> fc;
      //! isotropic hardening rules
      std::vector<std::shared_ptr<IsotropicHardeningRule>> ihrs;
      //! kinematic hardening rules
      std::vector<std::shared_ptr<KinematicHardeningRule>> khrs;
      //! Effect of the porosity on the flow rule.
      PorosityEffectOnFlowRule porosity_effect_on_equivalent_plastic_strain =
          UNDEFINED_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
      //! \brief algorithm used to handle the porosity evolution
      PorosityEvolutionAlgorithm porosity_evolution_algorithm =
          PorosityEvolutionAlgorithm::STAGGERED_SCHEME;
      /*!
       * \brief minimum value of the cosine of the angle between two successive
       * estimates of the flow direction. If the computed angle is lower than
       * this threshold, the Newton step is rejected.
       *
       * \note the value of this parameter must be in the range [-1:1]. If this
       * parameter is not in that range (which is the default), the code
       * checking the value of the cosine is not generated.
       */
      double cosine_threshold = 2;
      /*!
       * \brief a factor \f$alpha\f$ which gives the threshold below which the
       * check on the cosine of the angle between two successive
       * estimates of the flow direction is performed, i.e. the test is
       * performed if the iteration counter is greater than \f$alpha \cdot
       * i_{\max{}}\f$ where \f$i_{\max{}}\f$ is the maximum number of
       * iterations.
       */
      double cosine_check_minimum_iteration_factor = 0;
      /*!
       * \brief a factor \f$alpha\f$ which gives the threshold upper which the
       * check on the cosine of the angle between two successive
       * estimates of the flow direction is performed, i.e. the test is
       * performed if the iteration counter is below \f$alpha \cdot
       * i_{\max{}}\f$ where \f$i_{\max{}}\f$ is the maximum number of
       * iterations.
       */
      double cosine_check_maximum_iteration_factor = 1;
      /*!
       * \brief flag stating that the porosity evolution is handled by the brick
       */
      bool porosity_evolution_explicitely_handled = false;
      /*!
       * \brief flag stating if the contribution to the porosity growth
       * associated with this flow, if any, must be saved in a dedicated
       * auxiliary state variable.
       */
      bool save_porosity_increase = false;
    };  // end of struct InelasticFlowBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWBASE_HXX */
