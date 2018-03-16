/*!
 * \file   include/MFront/BehaviourBrick/InelasticPotentialBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIALBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIALBASE_HXX

#include <vector>
#include "MFront/BehaviourBrick/InelasticPotential.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief an helper class which provide common method for all inelastic
     * potentials.
     */
    struct InelasticPotentialBase : InelasticPotential {
      void setStressCriterion(const std::shared_ptr<StressCriterion>&) override;
      void setIsotropicHardeningRule(
          const std::shared_ptr<IsotropicHardeningRule>&) override;
      void addKinematicHardeningRule(
          const std::shared_ptr<KinematicHardeningRule>&) override;
      //! destructor
      ~InelasticPotentialBase() override;

     private:
      //! stress criterion
      std::shared_ptr<StressCriterion> sc;
      //! isotropic hardening rule
      std::shared_ptr<IsotropicHardeningRule> ihr;
      //! kinematic hardening rules
      std::vector<std::shared_ptr<KinematicHardeningRule>> khrs;
    };  // end of struct InelasticPotentialBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIALBASE_HXX */
