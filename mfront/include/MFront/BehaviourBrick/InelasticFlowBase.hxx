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
      std::vector<OptionDescription> getOptions() const override;
      //! destructor
      ~InelasticFlowBase() override;

     protected:
      //! stress criterion
      std::shared_ptr<StressCriterion> sc;
      //! isotropic hardening rule
      std::shared_ptr<IsotropicHardeningRule> ihr;
      //! kinematic hardening rules
      std::vector<std::shared_ptr<KinematicHardeningRule>> khrs;
    };  // end of struct InelasticFlowBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWBASE_HXX */
