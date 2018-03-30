/*!
 * \file   NortonInelasticFlow.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_NORTONINELASTICFLOW_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_NORTONINELASTICFLOW_HXX

#include "MFront/BehaviourBrick/InelasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief describe an inelastic flow defined by a Norton law.
     *
     * The Norton law is defined by the following associated flow rule:
     * \f[
     * \underline{\dot{\varepsilon}}^{\mathrm{vp}}=
     * \left(\frac{\left<\phi\left(\underline{\sigma}-\displaystyle\sum_{i}\underline{X}_{i}\right)-R\left(p\right)\right>}{K}\right)^{n}
     * \frac{\partial \phi}{\partial \underline{\sigma}}
     * \f]
     * where:
     * - \f$<.>\f$ are the Macaulay brackets
     * - \f$\phi\f$ is the stress criterion
     * - \f$\underline{X}_{i}\f$ is the \f$i^{\mathrm{th}}\f$ back stress
     * - \f$R\left(p\right)\f$ is the isotropic hardening rule
     * - \f$K\f$ is the Norton coefficient
     * - \f$n\f$ is the Norton exponent
     */
    struct NortonInelasticFlow : InelasticFlowBase {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      std::vector<OptionDescription> getOptions() const override;
      void completeVariableDeclaration(BehaviourDescription&,
                                       const AbstractBehaviourDSL&,
                                       const std::string&) const override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const StressPotential&,
                        const std::string&) const override;
      //! destructor
      ~NortonInelasticFlow() override;

     protected:
      //! \brief K coefficient
      BehaviourDescription::MaterialProperty K;
      //! \brief Norton exponent
      BehaviourDescription::MaterialProperty n;
    };  // end of struct NortonInelasticFlow

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_NORTONINELASTICFLOW_HXX */
