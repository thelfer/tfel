/*!
 * \file   include/MFront/BehaviourBrick/VoceIsotropicHardeningRule.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_VOCEISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_VOCEISOTROPICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  /*!
   * \brief class describing a Voce isotropic hardening rule
   * \f[
   * R\left(p\right)=R_{0}+\left(R_{\infty}-R_{0}\right)\,\exp\left(-b\,p\right)
   * \f]
   * where:
   * - \f$R_{0}\f$ is the yield strength
   * - \f$R_{inf}\f$ is the maximal yield strength
   * - \f$b\f$ is a parameter
   */
  struct VoceIsotropicHardeningRule final : IsotropicHardeningRule {
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const std::string&,
                    const DataMap&) override;
    std::vector<OptionDescription> getOptions() const override;
    std::string computeElasticPrediction(const BehaviourDescription&,
                                         const std::string&,
                                         const std::string&) const override;
    std::string computeElasticLimit(const BehaviourDescription&,
                                    const std::string&,
                                    const std::string&) const override;
    std::string computeElasticLimitAndDerivative(
        const BehaviourDescription&,
        const std::string&,
        const std::string&) const override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const std::string&) const override;
    //! \brief destructor
    ~VoceIsotropicHardeningRule() override;

   protected:
    BehaviourDescription::MaterialProperty R0;
    BehaviourDescription::MaterialProperty Rinf;
    BehaviourDescription::MaterialProperty b;
  };  // end of struct VoceIsotropicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_VOCEISOTROPICHARDENINGRULE_HXX */
