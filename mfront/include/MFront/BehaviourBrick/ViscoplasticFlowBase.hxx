/*!
 * \file   ViscoplasticFlowBase.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_VISCOPLASTICFLOWBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_VISCOPLASTICFLOWBASE_HXX

#include "MFront/BehaviourBrick/InelasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief describe an general viscoplastic flow.
     * \f[
     * \underline{\dot{\varepsilon}}^{\mathrm{vp}}=
     * f\left(\phi\left(\underline{\sigma}-\displaystyle\sum_{i}\underline{X}_{i}\right)-R\left(p\right)\right)^{n}
     * \frac{\partial \phi_{f}}{\partial \underline{\sigma}}
     * \f]
     * where:
     * - \f$f\f$ is a function describing the flow intensity
     * - \f$<.>\f$ are the Macaulay brackets
     * - \f$\phi\f$ is the stress criterion
     * - \f$\phi_{f}\f$ is the flow criterion
     * - \f$\underline{X}_{i}\f$ is the \f$i^{\mathrm{th}}\f$ back stress
     * - \f$R\left(p\right)\f$ is the isotropic hardening rule
     */
    struct ViscoplasticFlowBase : InelasticFlowBase {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      //! destructor
      ~ViscoplasticFlowBase() override;

     protected:
      std::string buildFlowImplicitEquations(const BehaviourDescription&,
                                             const StressPotential&,
                                             const std::string&,
                                             const bool) const override;
      /*!
       * \brief compute the flow rate "vp"+id
       * \param[in] id: flow id
       */
      virtual std::string computeFlowRate(const std::string&) const = 0;
      /*!
       * \brief compute the flow rate "vp"+id and its derivative "dvp_dseqe"
       * with respect to the effective equivalent stress defined by:
       * \f[
       * \phi\left(\underline{\sigma}-\displaystyle\sum_{i}\underline{X}_{i}\right)-R\left(p\right)
       * \f]
       * \param[in] id: flow id
       */
      virtual std::string computeFlowRateAndDerivative(
          const std::string&) const = 0;
    };  // end of struct ViscoplasticFlowBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_VISCOPLASTICFLOWBASE_HXX */
