/*!
 * \file mfront/include/MFront/BehaviourBrick/UserDefinedViscoplasticFlow.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_USERDEFINEDVISCOPLASTICFLOW_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_USERDEFINEDVISCOPLASTICFLOW_HXX

#include <map>
#include "TFEL/Math/Evaluator.hxx"
#include "MFront/BehaviourBrick/ViscoplasticFlowBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief describe an inelastic flow where the flow rule is defined by user.
   *
   * The viscoplastic strain rate is defined by the following rule:
   * \f[
   * \dot{p}=F\left(f,p\right)
   * \f]
   * where:
   * -
   * \f$f=\phi\left(\underline{\sigma}-\displaystyle\sum_{i}\underline{X}_{i}\right)-R\left(p\right)\f$
   * - \f$\phi\f$ is the stress criterion
   * - \f$\underline{X}_{i}\f$ is the \f$i^{\mathrm{th}}\f$ back stress
   * - \f$R\left(p\right)\f$ is the isotropic hardening rule
   *
   * The user shall define the \(F\) function
   */
  struct UserDefinedViscoplasticFlow : ViscoplasticFlowBase {
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const StressPotential&,
                      const std::string&) const override;
    std::vector<OptionDescription> getOptions() const override;
    //! \brief destructor
    ~UserDefinedViscoplasticFlow() override;

   protected:
    void checkOptions(const DataMap&) const override;
    bool describesStrainHardeningExplicitly() const override;
    std::string computeFlowRate(const BehaviourDescription&,
                                const StressPotential&,
                                const std::string&) const override;
    std::string computeFlowRateAndDerivative(const BehaviourDescription&,
                                             const StressPotential&,
                                             const std::string&) const override;
    //! \brief viscoplastic strain rate
    tfel::math::Evaluator vp;
    //! \brief derivative of the viscoplastic strain rate with respect to f
    std::optional<tfel::math::Evaluator> dvp_df;
    //! \brief derivative of the viscoplastic strain rate with respect to p
    std::optional<tfel::math::Evaluator> dvp_dp;
    //! \brief material properties
    std::map<std::string, BehaviourDescription::MaterialProperty> mps;
  };  // end of struct UserDefinedViscoplasticFlow

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_USERDEFINEDVISCOPLASTICFLOW_HXX */
