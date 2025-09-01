/*!
 * \file   include/MFront/BehaviourBrick/Cazacu2001StressCriterion.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CAZACU2001STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CAZACU2001STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterionBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief This class describes the Cazacu2001 stress criterion
   * defined by:
   * \f[
   * \sigma_{\mathrm{eq}}=
   * \sqrt{3}\,\sqrt[6]{\left(J_{2}^{O}\right)^{3}
   * - c\,\left(J_{3}^{O}\right)^{2}}
   * \f]
   * where \(J^{O}_{2}\) and \(J^{O}_{3}\) are the orthotropic
   * generalizations of the second and third invariants
   * of the deviatoric part of the stress tensor
   * \f$\underline{\sigma}\f$.
   */
  struct Cazacu2001StressCriterion final : StressCriterionBase {
    std::vector<OptionDescription> getOptions() const override;
    std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
        const override;
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
    ~Cazacu2001StressCriterion() override;

   protected:
    //! coefficients of \f$J_{2}^{O}\f$
    std::array<MaterialProperty, 6u> a;
    //! coefficients of \f$J_{3}^{O}\f$
    std::array<MaterialProperty, 11u> b;
    //! \brief c parameter
    BehaviourDescription::MaterialProperty cp;
  };  // end of struct Cazacu2001StressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CAZACU2001STRESSCRITERION_HXX */
