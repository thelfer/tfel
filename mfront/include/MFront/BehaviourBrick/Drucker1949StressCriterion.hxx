/*!
 * \file   include/MFront/BehaviourBrick/Drucker1949StressCriterion.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_DRUCKER1949STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_DRUCKER1949STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief This class describes the Drucker1949 stress criterion
   * defined by:
   * \f[
   * \sigma_{\mathrm{eq}}^{d}=
   * \sqrt{3}\,\sqrt[6]{J_{2}^{3} - c\,J_{3}^{2}}
   * \f]
   * where \(J_{2}\) and \(J_{3}\) are the first and second invariants
   * of the deviatoric part of the stress tensor
   * \f$\underline{\sigma}\f$.
   */
  struct Drucker1949StressCriterion final : StressCriterionBase {
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&,
                    const Role) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const Role) override;
    [[nodiscard]] std::vector<OptionDescription> getOptions() const override;
    [[nodiscard]] std::vector<BehaviourSymmetry>
    getSupportedBehaviourSymmetries() const override;
    [[nodiscard]] std::string computeElasticPrediction(
        const std::string&,
        const BehaviourDescription&,
        const StressPotential&) const override;
    [[nodiscard]] std::string computeCriterion(
        const std::string&,
        const BehaviourDescription&,
        const StressPotential&) const override;
    [[nodiscard]] std::string computeNormal(const std::string&,
                                            const BehaviourDescription&,
                                            const StressPotential&,
                                            const Role) const override;
    [[nodiscard]] std::string computeNormalDerivative(
        const std::string&,
        const BehaviourDescription&,
        const StressPotential&,
        const Role) const override;
    [[nodiscard]] bool isCoupledWithPorosityEvolution() const override;
    [[nodiscard]] bool isNormalDeviatoric() const override;
    [[nodiscard]] PorosityEffectOnFlowRule
    getPorosityEffectOnEquivalentPlasticStrain() const override;
    //! \brief destructor
    ~Drucker1949StressCriterion() override;

   protected:
    //! \brief c parameter
    BehaviourDescription::MaterialProperty cp;
  };  // end of struct Drucker1949StressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_DRUCKER1949STRESSCRITERION_HXX */
