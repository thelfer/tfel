/*!
 * \file   include/MFront/BehaviourBrick/InelasticFlow.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOW_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOW_HXX

#include <memory>
#include "MFront/BehaviourDescription.hxx"

namespace tfel {
  namespace utilities {
    // forward declaration
    struct Data;
  }  // end of namespace utilities
}  // end of namespace tfel

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  
  namespace bbrick {

    // forward declaration
    struct IsotropicHardeningRule;
    // forward declaration
    struct KinematicHardeningRule;
    // forward declaration
    struct StressCriterion;
    // forward declaration
    struct OptionDescription;

    /*!
     * \brief class describing an inelastic flow.
     */
    struct InelasticFlow {
      //! a simple alias
      using DataMap = std::map<std::string, tfel::utilities::Data>;
      /*!
       * \param[in] dsl: abstract behaviour dsl
       * \param[in] bd: behaviour description
       * \param[in] d: options
       */
      virtual void initialize(AbstractBehaviourDSL&,
                              BehaviourDescription&,
                              const DataMap&) = 0;
      //! \return the flow options
      virtual std::vector<OptionDescription> getOptions() const = 0;
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
      virtual ~InelasticFlow();

    };  // end of struct InelasticFlow

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOW_HXX */
