/*!
 * \file   include/MFront/BehaviourBrick/PowerIsotropicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13/02/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_POWERISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_POWERISOTROPICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  /*!
   * \brief class describing a power isotropic hardening rule
   * \f[
   * R\left(p\right)=R_{0}\left(p+p_{0}\right)^{n}
   * \f]
   * where:
   * - \f$R_{0}\f$ is the yield strength
   * - \f$n\f$ is the exponent
   * - \f$p_{0}\f$ is a parameter which avoids an infinite derivative of the
   *   previous relation for \(p=0\) in \(n\leq 1\).
   */
  struct PowerIsotropicHardeningRule final : IsotropicHardeningRule {
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
    ~PowerIsotropicHardeningRule() override;

   protected:
    //! \brief Coefficent of the power law (required)
    BehaviourDescription::MaterialProperty R0;
    /*!
     * \brief Numerical parameter to avoid infinite derivative if the power
     * exponent is lower than \(1\).
     */
    BehaviourDescription::MaterialProperty p0;
    //! \brief Exponent of the power law (required)
    BehaviourDescription::MaterialProperty n;
  };  // end of struct PowerIsotropicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_POWERISOTROPICHARDENINGRULE_HXX */
