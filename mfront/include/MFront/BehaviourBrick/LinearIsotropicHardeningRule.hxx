/*!
 * \file   include/MFront/BehaviourBrick/LinearIsotropicHardeningRule.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_LINEARISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_LINEARISOTROPICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing a linear isotropic hardening rule
     * \f[
     * R\left(p\right)=R_{0}+H\,p
     * \f]
     * where:
     * - \f$R_{0}\f$ is the yield strength
     * - \f$H\f$ is the hardening slope
     */
    struct LinearIsotropicHardeningRule final : IsotropicHardeningRule {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const std::string&,
                      const DataMap&) override;
      std::vector<OptionDescription> getOptions() const override;
      std::string computeElasticPrediction(const std::string&,
                                           const std::string&) const override;
      std::string computeElasticLimit(const std::string&,
                                      const std::string&) const override;
      std::string computeElasticLimitAndDerivative(
          const std::string&, const std::string&) const override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const std::string&,
                        const std::string&) const override;
      //! destructor
      ~LinearIsotropicHardeningRule() override;

     protected:
      BehaviourDescription::MaterialProperty R0;
      BehaviourDescription::MaterialProperty H;
    };  // end of struct LinearIsotropicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_LINEARISOTROPICHARDENINGRULE_HXX */
