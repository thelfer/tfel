/*!
 * \file   include/MFront/BehaviourBrick/Hosford1972StressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HOSFORD1972STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HOSFORD1972STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief This class describes the Hosford1972 stress criterion defined by:
   * \f[
   * \sigma_{\mathrm{eq}}^{H}=\sqrt[a]{\frac{1}{2}\left(
   *   \left|\sigma_{1}-\sigma_{2}\right|^{a}+
   *   \left|\sigma_{1}-\sigma_{3}\right|^{a}+
   *   \left|\sigma_{2}-\sigma_{3}\right|^{a}\right)}
   * \f]
   * where \f$\sigma_{1}\f$, \f$\sigma_{2}\f$ and \f$\sigma_{3}\f$
   * are the eigenvalues of the stress tensor
   * \f$\underline{\sigma}\f$.
   */
  struct Hosford1972StressCriterion final : StressCriterionBase {
    std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
        const override;
    std::vector<OptionDescription> getOptions() const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&,
                    const Role) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const Role) override;
    std::string computeElasticPrediction(const std::string&,
                                         const BehaviourDescription&,
                                         const StressPotential&) const override;
    std::string computeCriterion(const std::string&,
                                 const BehaviourDescription&,
                                 const StressPotential&) const override;
    std::string computeNormal(const std::string&,
                              const BehaviourDescription&,
                              const StressPotential&,
                              const Role) const override;
    std::string computeNormalDerivative(const std::string&,
                                        const BehaviourDescription&,
                                        const StressPotential&,
                                        const Role) const override;
    bool isCoupledWithPorosityEvolution() const override;
    bool isNormalDeviatoric() const override;
    PorosityEffectOnFlowRule getPorosityEffectOnEquivalentPlasticStrain()
        const override;
    //! destructor
    ~Hosford1972StressCriterion() override;

   private:
    /*!
     * \brief return the template parameter of the functions implemention the
     * computation of the functions computing the Hosford stress criterion,
     * its normal and its second derivative.
     */
    std::string getTemplateParameters() const;
    //! \brief hosford exponent
    BehaviourDescription::MaterialProperty a;
    //! \brief choice of the eigen solver
    std::string eigen_solver;
  };  // end of struct Hosford1972StressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HOSFORD1972STRESSCRITERION_HXX */
