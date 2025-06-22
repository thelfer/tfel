/*!
 * \file   NortonHoffViscoplasticFlow.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_NORTONHOFFVISCOPLASTICFLOW_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_NORTONHOFFVISCOPLASTICFLOW_HXX

#include "MFront/BehaviourBrick/ViscoplasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief describe an inelastic flow defined by a Norton law.
     *
     * The Norton law is defined by the following associated flow rule:
     * \f[
     * \underline{\dot{\varepsilon}}^{\mathrm{vp}}=
     * A\,\left(\frac{\left<\phi\left(\underline{\sigma}-\displaystyle\sum_{i}\underline{X}_{i}\right)-R\left(p\right)\right>}{K}\right)^{n}
     * \frac{\partial \phi}{\partial \underline{\sigma}}
     * \f]
     * where:
     * - \f$<.>\f$ are the Macaulay brackets
     * - \f$\phi\f$ is the stress criterion
     * - \f$\underline{X}_{i}\f$ is the \f$i^{\mathrm{th}}\f$ back stress
     * - \f$R\left(p\right)\f$ is the isotropic hardening rule
     * - \f$A\f$ is the Norton coefficient
     * - \f$K\f$ is a stress normalisation coefficient
     * - \f$n\f$ is the Norton exponent
     */
    struct NortonHoffViscoplasticFlow : ViscoplasticFlowBase {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const StressPotential&,
                        const std::string&) const override;
      std::vector<OptionDescription> getOptions() const override;
      //! destructor
      ~NortonHoffViscoplasticFlow() override;

     protected:
      std::string computeFlowRate(const std::string&) const override;
      std::string computeFlowRateAndDerivative(
          const std::string&) const override;
      //! \brief A coefficient
      BehaviourDescription::MaterialProperty A;
      //! \brief K coefficient
      BehaviourDescription::MaterialProperty K;
      //! \brief Norton exponent
      BehaviourDescription::MaterialProperty n;
      /*!
       * \brief stress threshold factor (optional).
       *
       * If defined, this property is used to check if the stress prediction
       * does not diverge during the Newton iterations.
       *
       * In pratice, it means that if the condition
       * \f$\sigma_{\mathrm{eq}}-R>K_{sf}\,K\f$, the Newton step is rejected.
       */
      BehaviourDescription::MaterialProperty Ksf;
    };  // end of struct NortonHoffViscoplasticFlow

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_NORTONHOFFVISCOPLASTICFLOW_HXX */
