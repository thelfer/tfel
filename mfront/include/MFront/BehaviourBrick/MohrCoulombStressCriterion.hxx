/*!
 * \file   include/MFront/BehaviourBrick/MohrCoulombStressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date 10/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_MOHRCOULOMBSTRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_MOHRCOULOMBSTRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief This class describes the MohrCoulomb stress criterion
   */
  struct MohrCoulombStressCriterion final : StressCriterionBase {
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
    ~MohrCoulombStressCriterion() override;

   protected:
    //! \brief cohesion
    BehaviourDescription::MaterialProperty c;
    //! \brief friction angle or dilatancy angle
    BehaviourDescription::MaterialProperty phi;
    //! \brief transition angle as defined by Abbo and Sloan
    BehaviourDescription::MaterialProperty lodeT;
    //! \brief tension cuff-off parameter
    BehaviourDescription::MaterialProperty a;
  };  // end of struct MohrCoulombStressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_MOHRCOULOMBSTRESSCRITERION_HXX */
