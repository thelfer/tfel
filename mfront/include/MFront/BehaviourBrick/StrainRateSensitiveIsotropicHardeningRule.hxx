/*!
 * \file   MFront/BehaviourBrick/StrainRateSensitiveIsotropicHardeningRule.hxx
 * \brief  This file declares the StrainRateSensitiveIsotropicHardeningRule
 * class \author Thomas Helfer \date   13/11/2024
 */

#ifndef LIB_MFRONT_BBRICKS_STRAINRATESENSITIVEISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BBRICKS_STRAINRATESENSITIVEISOTROPICHARDENINGRULE_HXX

#include <vector>
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/StrainRateSensitivityFactor.hxx"

namespace mfront::bbrick {

  /*!
   * \brief class describing a rate-sensitive isotropic hardening rule
   * \f[
   * R\left(p, \frac{\mathrm{d} p}{\mathrm{d} t}\right)=
   * \f]
   * where:
   * - \f$R_{0}\f$ is the radius is the elastic domain for an infinitely slow
   * loading. \f$R_{0}\f$ can be built by summing an arbitrary number of
   * isotropic hardening rule available in the `IsotropicHardeningRuleFactory`
   * factory.
   * - \f$R_{rs}\f$ is a correction taking into rate sensitivity.
   *   To be consistent with the definition of \f$R_{0}\f$,
   *   this correction is such that:
   *   \f[
   *   R_{rs}\left(0\right) = 1
   *   \f]
   */
  struct StrainRateSensitiveIsotropicHardeningRule final
      : IsotropicHardeningRule {
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
    ~StrainRateSensitiveIsotropicHardeningRule() override;

   protected:
    //! \brief rate independant isotropic hardening rules
    std::vector<std::shared_ptr<IsotropicHardeningRule>> ihrs;
    //! \brief rate sensitivity factor
    std::shared_ptr<StrainRateSensitivityFactor> rsf;
  };  // end of struct StrainRateSensitiveIsotropicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BBRICKS_STRAINRATESENSITIVEISOTROPICHARDENINGRULE_HXX */
