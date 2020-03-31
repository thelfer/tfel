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
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;

      /*!
       * \return if the the flow is coupled with the porosity evolution.
       * \note the flow is considered coupled with the porosity evolution if the
       * stress criterion of the flow criterion is.
       */
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
