/*!
 * \file   include/MFront/BehaviourBrick/DataIsotropicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   25/07/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_DATAISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_DATAISOTROPICHARDENINGRULE_HXX

#include <map>
#include "MFront/DataInterpolationUtilities.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  /*!
   * \brief class describing a Data isotropic hardening rule
   * \f[
   * R\left(p\right)=R_{0}+\left(R_{\infty}-R_{0}\right)\,\exp\left(-b\,p\right)
   * \f]
   * where:
   * - \f$R_{0}\f$ is the yield strength
   * - \f$R_{inf}\f$ is the maximal yield strength
   * - \f$b\f$ is a parameter
   */
  struct DataIsotropicHardeningRule final
      : public IsotropicHardeningRule,
        private SingleVariableInterpolatedData {
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const std::string&,
                    const DataMap&) override;
    std::vector<OptionDescription> getOptions() const override;
    std::string computeElasticPrediction(const BehaviourDescription&,
                                         const std::string&,
                                         const std::string&) const override;
    std::string computeElasticLimit(const BehaviourDescription&,
                                    const std::string&,
                                    const std::string&) const override;
    std::string computeElasticLimitAndDerivative(
        const BehaviourDescription&,
        const std::string&,
        const std::string&) const override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const std::string&) const override;
    //! \brief destructor
    ~DataIsotropicHardeningRule() override;

   protected:
    /*!
     * \brief append two member functions beginning with `computeYieldRadius`
     * and `computeYieldRadiusAndDerivative`.
     * \param[in] bd: behaviour description
     * \param[in] fid: flow id
     * \param[in] id: identifier
     */
    virtual void writeLinearInterpolationOfYieldRadius(
        BehaviourDescription&, const std::string&, const std::string&) const;
    /*!
     * \brief append two member functions beginning with `computeYieldRadius`
     * and `computeYieldRadiusAndDerivative`.
     * \param[in] bd: behaviour description
     * \param[in] fid: flow id
     * \param[in] id: identifier
     */
    virtual void writeCubicSplineInterpolationOfYieldRadius(
        BehaviourDescription&, const std::string&, const std::string&) const;
  };  // end of struct DataIsotropicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_DATAISOTROPICHARDENINGRULE_HXX */
