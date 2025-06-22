/*!
 * \file   mfront/src/IsotropicStrainHardeningMisesCreepDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02 jui 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/IsotropicStrainHardeningMisesCreepCodeGenerator.hxx"
#include "MFront/IsotropicStrainHardeningMisesCreepDSL.hxx"

namespace mfront {

  std::string IsotropicStrainHardeningMisesCreepDSL::getName() {
    return "IsotropicStrainHardeningMisesCreep";
  }

  std::string IsotropicStrainHardeningMisesCreepDSL::getDescription() {
    return "this parser is used for standard strain hardening creep behaviours "
           "of the form dp/dt=f(s,p) where p is the equivalent creep strain "
           "and s the equivalent mises stress";
  }  // end of getDescription

  IsotropicStrainHardeningMisesCreepDSL::IsotropicStrainHardeningMisesCreepDSL(
      const DSLOptions& opts)
      : IsotropicBehaviourDSLBase(opts) {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("IsotropicStrainHardeningMisesCreep");
    // Default state vars
    this->mb.addStateVariable(
        h, VariableDescription("StrainStensor", "εᵉˡ", "eel", 1u, 0u));
    this->mb.addStateVariable(h, VariableDescription("strain", "p", 1u, 0u));
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    this->mb.setGlossaryName(h, "p", "EquivalentViscoplasticStrain");
    // default local vars
    this->reserveName("mu_3");
    this->mb.addLocalVariable(h,
                              VariableDescription("strainrate", "f", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("tfel::math::derivative_type<strainrate,stress>",
                               "\u2202f\u2215\u2202\u03C3\u2091", "df_dseq", 1u,
                               0u));
    this->mb.addLocalVariable(
        h, VariableDescription("strainrate", "\u2202f\u2215\u2202p", "df_dp",
                               1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StressStensor", "se", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("stress", "\u03C3\u2091", "seq", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "seq_e", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StrainStensor", "n", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("strain", "p_", 1u, 0u));
    this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator, true);
    this->mb.setAttribute(
        h, BehaviourData::isConsistentTangentOperatorSymmetric, true);
  }

  std::unique_ptr<AbstractBehaviourCodeGenerator>
  IsotropicStrainHardeningMisesCreepDSL::getCodeGenerator() const {
    return std::make_unique<IsotropicStrainHardeningMisesCreepCodeGenerator>(
        this->fd, this->mb, this->interfaces);
  }  // end of getCodeGenerator

  std::string IsotropicStrainHardeningMisesCreepDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::FlowRule) {
      if (o.useUnicodeSymbols) {
        return "@FlowRule{\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "\u2202f\u2215\u2202p            = ;\n"
               "}";
      } else {
        return "@FlowRule{\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "df_dp   = ;\n"
               "}";
      }
    }
    return "";
  }  // end of getCodeBlockTemplate

  void IsotropicStrainHardeningMisesCreepDSL::endsInputFileProcessing() {
    IsotropicBehaviourDSLBase::endsInputFileProcessing();
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    tfel::raise_if(!this->mb.hasCode(h, BehaviourData::FlowRule),
                   "IsotropicMisesCreepDSL::endsInputFileProcessing: "
                   "no flow rule defined");
  }  // end of endsInputFileProcessing

  IsotropicStrainHardeningMisesCreepDSL::
      ~IsotropicStrainHardeningMisesCreepDSL() = default;

}  // end of namespace mfront
