/*!
 * \file   include/MFront/BehaviourBrick/InelasticFlowBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
      //! destructor
      ~InelasticFlowBase() override;

     protected:
      //! \brief policy on how the porosity affects the flow rule
      enum PorosityEffectOnFlowRule {
        /*!
         * \brief default choice. The effect of the porosity depends
         * on the stress criterion (see the
         * `StressCriterion::getPorosityEffectOnFlowRule` method).
         */
        UNDEFINED_POROSITY_EFFECT_ON_FLOW_RULE,
        /*!
         * \brief This value indicate that the flow rule is not affected by the
         * porosity evolution.
         */
        NO_POROSITY_EFFECT_ON_FLOW_RULE,
        /*!
         * \brief This value indicate that the flow rule must be corrected by
         * the standard \f$(1-f)\f$ factor where \f$f\f$ is the porosity.
         */
        STANDARD_POROSITY_CORRECTION_ON_FLOW_RULE,
      };
      //! \return the effect of the porosity on the flow rule.
      virtual PorosityEffectOnFlowRule getPorosityEffectOnFlowRule() const;
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
      PorosityEffectOnFlowRule porosity_effect_on_flow_rule =
          UNDEFINED_POROSITY_EFFECT_ON_FLOW_RULE;
      //! \brief algorithm used to handle the porosity evolution
      PorosityEvolutionAlgorithm porosity_evolution_algorithm =
          PorosityEvolutionAlgorithm::STAGGERED_SCHEME;
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
