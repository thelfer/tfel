/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/GeneralisedRousselierTanguyBessonStressCriterion.hxx
 * \brief  This file declares the
 * GeneralisedRousselierTanguyBessonStressCriterion class
 * \author Tristan Chenaille
 * \date   14/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/PorousStressCriterionEquivalentStress.hxx"
#include "MFront/BehaviourBrick/RousselierTanguyBesson2002StressCriterion.hxx"

namespace mfront::bbrick {

  /*!
   * \brief implementation of the GeneralisedRousselierTanguyBesson stress
   * criterion, which is the RousselierTanguyBesson2002 stress criterion in
   * which the von Mises stress is replaced by an equivalent stress chosen by
   * the user.
   */
  struct GeneralisedRousselierTanguyBessonStressCriterion final
      : RousselierTanguyBesson2002StressCriterion {
    //! \brief constructor
    GeneralisedRousselierTanguyBessonStressCriterion();
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
    ~GeneralisedRousselierTanguyBessonStressCriterion() override;

   private:
    //! \brief equivalent stress replacing the von Mises stress
    PorousStressCriterionEquivalentStress equivalent_stress;
  };  // end of struct GeneralisedRousselierTanguyBessonStressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESSCRITERION_HXX \
        */
