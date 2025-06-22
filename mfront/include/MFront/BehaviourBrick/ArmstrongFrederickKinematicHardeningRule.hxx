/*!
 * \file   ArmstrongFrederickKinematicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ARMSTRONGFREDERICKKINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ARMSTRONGFREDERICKKINEMATICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing the Armstrong-Frederick kinematic hardening rule
     * defined by the following evolution of the back-strain variable
     * \f$\underline{a}\f$:
     * \f[
     * \underline{\dot{a}}=\underline{\dot{\varepsilon}}^{p}-D\,\underline{a}
     * \f]
     */
    struct ArmstrongFrederickKinematicHardeningRule
        : KinematicHardeningRuleBase {
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
      //! destructor
      ~ArmstrongFrederickKinematicHardeningRule() override;

     protected:
      //! \brief call-back
      BehaviourDescription::MaterialProperty D;

    };  // end of struct KinematicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ARMSTRONGFREDERICKKINEMATICHARDENINGRULE_HXX \
        */
