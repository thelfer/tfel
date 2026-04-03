
/*!
 * \file   mfront/src/IsotropicMisesCreepDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/IsotropicMisesCreepCodeGenerator.hxx"
#include "MFront/IsotropicMisesCreepDSL.hxx"

namespace mfront {

  std::string IsotropicMisesCreepDSL::getDescription() {
    return "this parser is used for standard creep behaviours of the form  "
           " dp/dt=f(s) where p is the equivalent creep strain and s the "
           "equivalent mises stress";
  }  // end of getDescription

  std::string IsotropicMisesCreepDSL::getName() {
    return "IsotropicMisesCreep";
  }

  IsotropicMisesCreepDSL::IsotropicMisesCreepDSL(const DSLOptions& opts)
      : IsotropicBehaviourDSLBase(opts) {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("IsotropicMisesCreepDSL");
    //
    this->disableCallBack("@IsotropicHardeningRule");
    this->disableCallBack("@IsotropicHardeningRules");
    // Default state vars
    this->reserveName("mfront_internal_ccto_tmp_1");
    this->reserveName("mfront_internal_3_mu_theta");
    this->mb.addStateVariable(h, VariableDescription("strain", "p", 1u, 0u));
    this->mb.setGlossaryName(h, "p", "EquivalentViscoplasticStrain");
    // default local vars
    this->mb.addLocalVariable(h,
                              VariableDescription("strainrate", "f", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription(
               "tfel::math::derivative_type<strainrate, stress>",
               "\u2202f\u2215\u2202\u03C3\u2091", "df_dseq", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StressStensor", "se", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("stress", "\u03C3\u2091", "seq", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "seq_e", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StrainStensor", "n", 1u, 0u));
    this->mb.setAttribute(h, BehaviourData::hasConsistentTangentOperator, true);
    this->mb.setAttribute(
        h, BehaviourData::isConsistentTangentOperatorSymmetric, true);
  }

  bool IsotropicMisesCreepDSL::handleStrainHardening() const { return false; }

  std::unique_ptr<AbstractBehaviourCodeGenerator>
  IsotropicMisesCreepDSL::getCodeGenerator() const {
    return std::make_unique<IsotropicMisesCreepCodeGenerator>(
        this->fd, this->mb, this->interfaces);
  }  // end of getCodeGenerator

  std::string IsotropicMisesCreepDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::FlowRule) {
      if (o.useUnicodeSymbols) {
        return "@FlowRule{\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "}";
      } else {
        return "@FlowRule{\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "}";
      }
    }
    return "";
  }  // end of getCodeBlockTemplate

  void IsotropicMisesCreepDSL::endsInputFileProcessing() {
    IsotropicBehaviourDSLBase::endsInputFileProcessing();
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      if (!this->mb.hasCode(h, BehaviourData::FlowRule)) {
        this->throwRuntimeError(
            "IsotropicMisesCreepDSL::endsInputFileProcessing",
            "no flow rule defined");
      }
    }
  }  // end of endsInputFileProcessing

  IsotropicMisesCreepDSL::~IsotropicMisesCreepDSL() = default;

}  // end of namespace mfront
