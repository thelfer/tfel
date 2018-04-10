/*!
 * \file   AmstrongFrederickKinematicHardeningRule.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_AMSTRONGFREDERICKKINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_AMSTRONGFREDERICKKINEMATICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing the Amstrong-Frederick kinematic hardening rule
     * defined by the following evolution of the back-strain variable
     * \f$\underline{a}\f$:
     * \f[
     * \underline{\dot{a}}=\underline{\dot{\varepsilon}}^{p}-D\,\underline{a}
     * \f]
     */
    struct AmstrongFrederickKinematicHardeningRule
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
      std::string buildBackStrainImplicitEquations(const BehaviourDescription&,
                                                   const StressPotential&,
                                                   const std::string&,
                                                   const std::string&,
                                                   const bool) const override;
      //! destructor
      ~AmstrongFrederickKinematicHardeningRule() override;

     protected:
      //! \brief call-back
      BehaviourDescription::MaterialProperty D;
      //! \brief kinematic hardening moduli
      BehaviourDescription::MaterialProperty C;

    };  // end of struct KinematicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_AMSTRONGFREDERICKKINEMATICHARDENINGRULE_HXX \
          */
