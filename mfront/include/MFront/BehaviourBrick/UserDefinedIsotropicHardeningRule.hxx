/*!
 * \file   include/MFront/BehaviourBrick/UserDefinedIsotropicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_USERDEFINEDISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_USERDEFINEDISOTROPICHARDENINGRULE_HXX

#include "TFEL/Math/Evaluator.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  //! \brief class describing an hardening rule defined by analytical expression
  struct UserDefinedIsotropicHardeningRule final : IsotropicHardeningRule {
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
    ~UserDefinedIsotropicHardeningRule() override;

   protected:
    //! \brief yield surface radius
    tfel::math::Evaluator R;
    /*!
     * \brief derivative of the yield surface radius with respect to the
     * equivalent plastic strain
     */
    std::optional<tfel::math::Evaluator> dR_dp;
    //! \brief material properties
    std::map<std::string, BehaviourDescription::MaterialProperty> mps;
  };  // end of struct UserDefinedIsotropicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_USERDEFINEDISOTROPICHARDENINGRULE_HXX */
