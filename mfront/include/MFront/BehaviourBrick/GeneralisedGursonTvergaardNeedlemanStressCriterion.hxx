/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/GeneralisedGursonTvergaardNeedlemanStressCriterion.hxx
 * \brief  This file declares the
 * GeneralisedGursonTvergaardNeedlemanStressCriterion class
 * \author Tristan Chenaille
 * \date   14/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_GENERALISED_GURSON_TVERGAARD_NEEDLEMAN_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_GENERALISED_GURSON_TVERGAARD_NEEDLEMAN_STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/PorousStressCriterionEquivalentStress.hxx"
#include "MFront/BehaviourBrick/GursonTvergaardNeedleman1982StressCriterion.hxx"

namespace mfront::bbrick {

  /*!
   * \brief implementation of the GeneralisedGursonTvergaardNeedleman stress
   * criterion, which is the GursonTvergaardNeedleman1982 stress criterion in
   * which the von Mises stress is replaced by an equivalent stress chosen by
   * the user.
   */
  struct GeneralisedGursonTvergaardNeedlemanStressCriterion final
      : GursonTvergaardNeedleman1982StressCriterion {
    //! \brief constructor
    GeneralisedGursonTvergaardNeedlemanStressCriterion();
    //
    [[nodiscard]] std::vector<OptionDescription> getOptions() const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const DataMap&,
                    const Role) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const Role) override;
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
    //! \brief destructor
    ~GeneralisedGursonTvergaardNeedlemanStressCriterion() override;

   private:
    //! \brief equivalent stress replacing the von Mises stress
    PorousStressCriterionEquivalentStress equivalent_stress;
  };  // end of struct GeneralisedGursonTvergaardNeedlemanStressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_GENERALISED_GURSON_TVERGAARD_NEEDLEMAN_STRESSCRITERION_HXX \
        */
