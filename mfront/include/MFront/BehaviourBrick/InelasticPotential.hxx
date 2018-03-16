/*!
 * \file   include/MFront/BehaviourBrick/InelasticPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIAL_HXX

#include<memory>

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct IsotropicHardeningRule;
    // forward declaration
    struct KinematicHardeningRule;
    // forward declaration
    struct StressCriterion;

    /*!
     * \brief class describing an inelastic potential.
     */
    struct InelasticPotential {
      /*!
       * \brief set the stress criterion
       * \param[in] s: stress criterion
       */
      virtual void setStressCriterion(
          const std::shared_ptr<StressCriterion>&) = 0;
      /*!
       * \brief set the isotropic hardening rule
       * \param[in] r: isotropic hardening rule
       */
      virtual void setIsotropicHardeningRule(
          const std::shared_ptr<IsotropicHardeningRule>&) = 0;
      /*!
       * \brief add a kinematic hardening rule
       * \param[in] r: kinematic hardening rule
       */
      virtual void addKinematicHardeningRule(
          const std::shared_ptr<KinematicHardeningRule>&) = 0;
      //! destructor
      virtual ~InelasticPotential();

    };  // end of struct InelasticPotential

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIAL_HXX */
