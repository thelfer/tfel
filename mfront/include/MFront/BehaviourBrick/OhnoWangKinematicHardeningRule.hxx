/*!
 * \file   OhnoWangKinematicHardeningRule.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_OHNOWANGKINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_OHNOWANGKINEMATICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief This class describes the Ohno-Wang kinematic hardening rule
     * defined by the following evolution of the back-strain variable
     * \f$\underline{a}\f$:
     * \f[
     * \underline{\dot{a}}=\underline{\dot{\varepsilon}}^{p}-D\,\Phi\left(\underline{X}\right)\,\underline{a}
     * \f]
     * with:
     * \f[
     * \Phi\left(\underline{X}\right)=\left<\frac{D\,J\left(\underline{X}\right)-\omega\,C}{1-\omega}\right>^{m_{1}}\,\left(\frac{1}{D\,J\left(\underline{X}\right)}\right)^{m_{2}}
     * \f]
     */
    struct OhnoWangKinematicHardeningRule
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
          const std::vector<std::shared_ptr<KinematicHardeningRule>>&,
          const std::string&,
          const std::string&,
          const bool) const override;
      //! destructor
      ~OhnoWangKinematicHardeningRule() override;

     protected:
      //! \brief call-back
      BehaviourDescription::MaterialProperty D;
      //! \brief Ohno-Wang parameter
      BehaviourDescription::MaterialProperty w;
      //! \brief Ohno-Wang parameter
      BehaviourDescription::MaterialProperty m1;
      //! \brief Ohno-Wang parameter
      BehaviourDescription::MaterialProperty m2;

    };  // end of struct KinematicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_OHNOWANGKINEMATICHARDENINGRULE_HXX */
