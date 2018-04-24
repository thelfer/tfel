/*!
 * \file   include/MFront/BehaviourBrick/InelasticFlowBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWBASE_HXX

#include <vector>
#include "MFront/BehaviourBrick/InelasticFlow.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief an helper class which provide common method for all inelastic
     * potentials.
     */
    struct InelasticFlowBase : InelasticFlow {
      /*!
       * \brief if an isotropic hardening rule is defined, a boolean local
       * variable named `bpl`+id is defined.
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] id: flow id
       * \param[in] d: options
       */
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      /*!
       * \brief if an isotropic hardening rule is defined, additional
       * convergence checks are added to check that, if the convergence of the
       * Newton algorithm is reached:
       * - if the flow is active (`bpl`+id is `true`), the equivalent plastic
       *   strain multiplier must be positive. If negative, the convergence flag
       *   is set to false and the flow is desactivated (`bpl`+id is set to
       *   `false`).
       * - if the flow is not active (`bpl`+id is `false`), the stress criterion
       *   must be lower than the elastic limit given by the isotropic hardening
       *   rule. If this is not the case, the convergence flag is set to false
       *   and the flow is activated (`bpl`+id is set to `true`).
       *
       * \note if the stress criterion is the same as the flow criterion, the
       * flow is associative.
       *
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] sp: stress potential
       * \param[in] id: flow id
       */
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
      //! destructor
      ~InelasticFlowBase() override;

     protected:
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
    };  // end of struct InelasticFlowBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWBASE_HXX */
