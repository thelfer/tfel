/*!
 * \file   KinematicHardeningRuleBase.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEBASE_HXX

#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing the Prager kinematic hardening rule
     */
    struct KinematicHardeningRuleBase : KinematicHardeningRule {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const std::string&,
                      const DataMap&) override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const std::string&,
                        const std::string&) const override;
      std::vector<OptionDescription> getOptions() const override;
      std::vector<std::string> getKinematicHardeningsVariables(
          const std::string&, const std::string&) const override;
      std::string computeKinematicHardeningsInitialValues(
          const std::string&, const std::string&) const override;
      std::string computeKinematicHardenings(const std::string&,
                                             const std::string&) const override;
      std::string getBackStrainVariable(const std::string&,
                                        const std::string&) const override;
      std::string getBackStressDerivative(const std::string&,
                                          const std::string&) const override;
      std::string generateImplicitEquationDerivatives(
          const std::string&,
          const std::string&,
          const std::string&,
          const std::string&) const override;
      //! destructor
      ~KinematicHardeningRuleBase() override;

     protected:
      //! \brief kinematic hardening moduli
      BehaviourDescription::MaterialProperty C;

    };  // end of struct KinematicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEBASE_HXX */
