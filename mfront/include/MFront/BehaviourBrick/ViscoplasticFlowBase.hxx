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

namespace mfront::bbrick {

  /*!
   * \brief describe an general viscoplastic flow.
   * \f[
   * \underline{\dot{\varepsilon}}^{\mathrm{vp}}=
   * f\left(\phi\left(\underline{\sigma}
   * -\displaystyle\sum_{i}\underline{X}_{i}\right)
   * -R\left(p\right)\right, p)^{n}
   * \frac{\partial \phi_{f}}{\partial \underline{\sigma}}
   * \f]
   * where:
   * - \f$f\f$ is a function describing the viscoplastic strain rate
   * - \f$<.>\f$ are the Macaulay brackets
   * - \f$\phi\f$ is the stress criterion
   * - \f$\phi_{f}\f$ is the flow criterion
   * - \f$\underline{X}_{i}\f$ is the \f$i^{\mathrm{th}}\f$ back stress
   * - \f$R\left(p\right)\f$ is the isotropic hardening rule
   * - \f$p\f$ is the equivalent viscoplastic strain
   *
   * \note If \f$f\f$ depends explicitly on the equivalent viscoplastic strain,
   * the method `describesStrainHardeningExplicitly` must return `true`.
   */
  struct ViscoplasticFlowBase : InelasticFlowBase {
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&) override;
    //! \brief destructor
    ~ViscoplasticFlowBase() override;

   protected:
    std::string buildFlowImplicitEquations(const BehaviourDescription&,
                                           const StressPotential&,
                                           const std::string&,
                                           const bool) const override;
    /*!
     * \return if the viscoplastic strain rate \f$f\f$ depends explicitly on the
     * equivalent viscoplastic strain.
     */
    virtual bool describesStrainHardeningExplicitly() const = 0;
    /*!
     * \brief compute the strain rate "vp"+id
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     * \param[in] id: flow id
     */
    virtual std::string computeFlowRate(const BehaviourDescription&,
                                        const StressPotential&,
                                        const std::string&) const = 0;
    /*!
     * \brief compute the strain rate "vp"+id and its derivative
     * "dvp"+id+"_dseqe"+id with respect to the effective
     * equivalent stress defined by:
     * \f[
     * \phi\left(\underline{\sigma} -
     * \displaystyle\sum_{i}\underline{X}_{i}\right) - R\left(p\right)
     * \f]
     *
     * If `describesStrainHardeningExplicitly` returns true, this
     * method shall also compute "dvp"+id+"_dseqe"+id.`
     *
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     * \param[in] id: flow id
     */
    virtual std::string computeFlowRateAndDerivative(
        const BehaviourDescription&,
        const StressPotential&,
        const std::string&) const = 0;
  };  // end of struct ViscoplasticFlowBase

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_VISCOPLASTICFLOWBASE_HXX */
