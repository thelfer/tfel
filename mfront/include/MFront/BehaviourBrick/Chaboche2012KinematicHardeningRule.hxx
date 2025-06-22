/*!
 * \file   Chaboche2012KinematicHardeningRule.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CHABOCHE2012KINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CHABOCHE2012KINEMATICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief This class describes one modification of the Amstrong-Frederick
     * kinematic hardening rule proposed by J.L. Chaboche, P. Kanouté and F.
     * Azzouz in 2012 defined by the following evolution of the back-strain
     * variable \f$\underline{a}\f$: \f[ \underline{\dot{a}}
     * =\underline{\dot{\varepsilon}}^{p}-\frac{3\,D}{2\,C}\,\Phi\left(p\right)\,
     * \Psi^{\left(\underline{X}\right)}\left(\underline{X}\right)\underline{X}
     * =\underline{\dot{\varepsilon}}^{p}-
     * D\,\Phi\left(p\right)\,\Psi\left(\underline{a}\right)\underline{a}
     * \f]
     * with:
     * - \f$\underline{X}=\frac{2}{3}\,C\,\underline{a}\f$
     * - \f$
     * \Phi\left(p\right)=\phi_{\infty}+
     * \left(1-\phi_{\infty}\right)\,\exp\left(-b\,p\right)
     * \f$
     * - \f$
     * \Psi^{\left(\underline{X}\right)}\left(\underline{X}\right)=
     * \frac{\left<D\,J\left(\underline{X}\right)-\omega\,C\right>^{m}}{1-\omega}\,
     * \frac{1}{\left(D\,J\left(\underline{X}\right)\right)^{m}}
     * \f$
     * - \f$
     * \Psi\left(\underline{a}\right)=
     * \frac{\left<D\,J\left(\underline{a}\right)-\frac{3}{2}\omega\right>^{m}}{1-\omega}\,
     * \frac{1}{\left(D\,J\left(\underline{a}\right)\right)^{m}}
     * \f$
     */
    struct Chaboche2012KinematicHardeningRule : KinematicHardeningRuleBase {
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
      ~Chaboche2012KinematicHardeningRule() override;

     protected:
      //! \brief call-back
      BehaviourDescription::MaterialProperty D;
      BehaviourDescription::MaterialProperty m;
      BehaviourDescription::MaterialProperty Phi_inf;
      BehaviourDescription::MaterialProperty bp;
      BehaviourDescription::MaterialProperty w;

    };  // end of struct KinematicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CHABOCHE2012KINEMATICHARDENINGRULE_HXX */
