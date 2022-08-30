/*!
 * \file   OrthotropicKinematicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ORTHOTROPICKINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ORTHOTROPICKINEMATICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief 
   */
  struct OrthotropicKinematicHardeningRule : KinematicHardeningRuleBase {
    std::vector<OptionDescription> getOptions() const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const std::string&,
                    const DataMap&) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const std::string&) const override;
    std::string buildBackStrainImplicitEquations(
        const BehaviourDescription&,
        const StressPotential&,
        const StressCriterion&,
        const std::vector<std::shared_ptr<KinematicHardeningRule>>&,
        const std::string&,
        const std::string&,
        const bool) const override;
    //! \brief destructor
    ~OrthotropicKinematicHardeningRule() override;

   protected:
    //! \brief call-back
    BehaviourDescription::MaterialProperty D;
    //! \brief memory coefficient
    BehaviourDescription::MaterialProperty f;
    //! \brief normalisation coefficient of the equivalent back strain
    BehaviourDescription::MaterialProperty a0;
    //! \brief memory exponent
    BehaviourDescription::MaterialProperty m;
    //! \brief coefficients of the first linear transformation
    std::array<MaterialProperty, 6u> Ec;
    //! \brief coefficients of the second linear transformation
    std::array<MaterialProperty, 6u> Rd;
    //! \brief coefficients of the third linear transformation
    std::array<MaterialProperty, 6u> Rs;
  };  // end of struct KinematicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ORTHOTROPICKINEMATICHARDENINGRULE_HXX */
