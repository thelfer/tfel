/*!
 * \file   DelobelleRobinetSchafflerKinematicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_DRSKINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_DRSKINEMATICHARDENINGRULE_HXX

#include "MFront/BehaviourBrick/KinematicHardeningRuleBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief kinematic hardening rule derived from the work of Delobelle, Robinet
   * and Schaffler on Zircaloy4 alloys. This orthotropic hardening kinematic
   * rule describes static and dynamic recovery.
   *
   * Thermomechanical Behavior and Modeling Between 350°C and 400°C of
   * Zircaloy-4 Cladding Tubes From an Unirradiated State to High Fluence (0 to
   * 85s˙1024 nm−2,E>1MeV
   * Schäffler, I. and Geyer, P. and Bouffioux, P. and Delobelle, P.
   * Journal of Engineering Materials and Technology. 1999
   *
   * A model to describe the anisotropic viscoplastic behaviour of Zircaloy-4
   * tubes.
   * Delobelle, P. and Robinet, P. and Geyer, P. and Bouffioux, P.
   * Journal of Nuclear Materials. 1996
   */
  struct DelobelleRobinetSchafflerKinematicHardeningRule
      : KinematicHardeningRuleBase {
    std::vector<OptionDescription> getOptions() const override;
    void initialize(BehaviourDescription&,
                    AbstractBehaviourDSL&,
                    const std::string&,
                    const std::string&,
                    const DataMap&) override;
    void endTreatment(BehaviourDescription&,
                      const AbstractBehaviourDSL&,
                      const std::string&,
                      const std::string&) const override;
    std::string buildBackStrainImplicitEquations(
        const BehaviourDescription&,
        const StressPotential&,
        const StressCriterion&,
        const std::vector<std::shared_ptr<KinematicHardeningRule>>&,
        const std::string&,
        const std::string&,
        const bool) const override;
    //! \brief destructor
    ~DelobelleRobinetSchafflerKinematicHardeningRule() override;

   protected:
    //! \brief call-back
    BehaviourDescription::MaterialProperty D;
    //! \brief memory coefficient
    BehaviourDescription::MaterialProperty f;
    //! \brief normalisation coefficient of the equivalent back strain
    BehaviourDescription::MaterialProperty a0;
    //! \brief memory exponent
    BehaviourDescription::MaterialProperty m;
    //! \brief coefficients of the first linear transformation
    std::array<MaterialProperty, 6u> Ec;
    //! \brief coefficients of the second linear transformation
    std::array<MaterialProperty, 6u> Rd;
    //! \brief coefficients of the third linear transformation
    std::array<MaterialProperty, 6u> Rs;
  };  // end of struct KinematicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_DRSKINEMATICHARDENINGRULE_HXX */
