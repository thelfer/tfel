/*!
 * \file   mfront/src/MultipleIsotropicMisesFlowsDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   31 jan 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MultipleIsotropicMisesFlowsCodeGenerator.hxx"
#include "MFront/MultipleIsotropicMisesFlowsDSL.hxx"

namespace mfront {

  MultipleIsotropicMisesFlowsDSL::FlowHandler::~FlowHandler() noexcept =
      default;

  std::string MultipleIsotropicMisesFlowsDSL::getName() {
    return "MultipleIsotropicMisesFlows";
  }

  std::string MultipleIsotropicMisesFlowsDSL::getDescription() {
    return "this parser is used to define behaviours combining several "
           "isotropic flows. Supported flow type are 'Creep' (dp/dt=f(s)) "
           "'StrainHardeningCreep' (dp/dt=f(s,p)) and 'Plasticity' (f(p,s)=0) "
           "where p is the equivalent plastic strain and s the equivalent "
           "mises stress";
  }  // end of getDescription

  MultipleIsotropicMisesFlowsDSL::MultipleIsotropicMisesFlowsDSL(
      const DSLOptions& opts)
      : IsotropicBehaviourDSLBase(opts) {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("MultipleIsotropicMisesFlows");
    // Default state vars
    this->mb.addStateVariable(
        h, VariableDescription("StrainStensor", "εᵉˡ", "eel", 1u, 0u));
    this->mb.addStateVariable(h, VariableDescription("strain", "p", 1u, 0u));
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    this->mb.setGlossaryName(h, "p", "EquivalentStrain");
    // default local vars
    this->reserveName("mu_3_theta");
    this->reserveName("surf");
    this->reserveName("jacobian_inversion_succeeded");
    this->mb.addLocalVariable(
        h, VariableDescription("StressStensor", "se", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("stress", "\u03C3\u2091", "seq", 1u, 0u));
    this->mb.addLocalVariable(h,
                              VariableDescription("stress", "seq_e", 1u, 0u));
    this->mb.addLocalVariable(
        h, VariableDescription("StrainStensor", "n", 1u, 0u));
    this->mb.addLocalVariable(h, VariableDescription("strain", "p_", 1u, 0u));
  }

  std::unique_ptr<AbstractBehaviourCodeGenerator>
  MultipleIsotropicMisesFlowsDSL::getCodeGenerator() const {
    return std::make_unique<MultipleIsotropicMisesFlowsCodeGenerator>(
        this->fd, this->mb, this->interfaces, this->flows);
  }  // end of getCodeGenerator

  BehaviourDSLDescription
  MultipleIsotropicMisesFlowsDSL::getBehaviourDSLDescription() const {
    auto d = IsotropicBehaviourDSLBase::getBehaviourDSLDescription();
    d.minimalMFrontFileBody = "@FlowRule Plasticity {}\n\n";
    return d;
  }  // end of getBehaviourDSLDescription

  void MultipleIsotropicMisesFlowsDSL::getSymbols(
      std::map<std::string, std::string>& symbols,
      const Hypothesis h,
      const std::string& n) {
    IsotropicBehaviourDSLBase::getSymbols(symbols, h, n);
    mfront::addSymbol(symbols, "\u2202f\u2215\u2202\u03C3\u2091", "df_dseq");
    mfront::addSymbol(symbols, "\u2202f\u2215\u2202p", "df_dp");
  }  // end of getSymbols

  std::string MultipleIsotropicMisesFlowsDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::FlowRule) {
      if (o.useUnicodeSymbols) {
        return "@FlowRule Plasticity {\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "\u2202f\u2215\u2202p            = ;\n"
               "}\n\n"
               "@FlowRule StrainHardeningCreep{\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "\u2202f\u2215\u2202p            = ;\n"
               "}\n\n"
               "@FlowRule Creep {\n"
               "// \u03C3\u2091 is the current estimate of the von Mises "
               "stress at "
               "t+\u03B8\u22C5\u0394t\n"
               "f       = ;\n"
               "\u2202f\u2215\u2202\u03C3\u2091 = ;\n"
               "}";
      } else {
        return "@FlowRule Plasticity{\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "df_dp   = ;\n"
               "}\n\n"
               "@FlowRule StrainHardeningCreep{\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "df_dp   = ;\n"
               "}\n\n"
               "@FlowRule Creep {\n"
               "// seq is the current estimate of the von Mises stress at "
               "t+theta*dt\n"
               "f       = ;\n"
               "df_dseq = ;\n"
               "}";
      }
    }
    return "";
  }  // end of getCodeBlockTemplate

  void MultipleIsotropicMisesFlowsDSL::treatFlowRule() {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    FlowHandler flow;
    this->checkNotEndOfFile("MultipleIsotropicMisesFlowsDSL::treatFlowRule",
                            "Expected flow rule name.");
    if (this->current->value == "Plasticity") {
      std::ostringstream p;
      std::ostringstream f;
      std::ostringstream df_dseq;
      std::ostringstream df_dp;
      p << "p" << this->flows.size();
      f << "f" << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      df_dp << "df_dp" << this->flows.size();
      this->mb.addStateVariable(uh,
                                VariableDescription("strain", p.str(), 1u, 0u),
                                BehaviourData::FORCEREGISTRATION);
      this->mb.addLocalVariable(uh,
                                VariableDescription("stress", f.str(), 1u, 0u));
      this->mb.addLocalVariable(
          uh, VariableDescription("real", df_dseq.str(), 1u, 0u));
      this->mb.addLocalVariable(
          uh, VariableDescription("stress", df_dp.str(), 1u, 0u));
      flow.flow = FlowHandler::PlasticFlow;
    } else if (this->current->value == "Creep") {
      std::ostringstream p;
      std::ostringstream f;
      std::ostringstream df_dseq;
      p << "p" << this->flows.size();
      f << "f" << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      this->mb.addStateVariable(uh,
                                VariableDescription("strain", p.str(), 1u, 0u),
                                BehaviourData::FORCEREGISTRATION);
      this->mb.addLocalVariable(
          uh, VariableDescription("strainrate", f.str(), 1u, 0u));
      this->mb.addLocalVariable(
          uh,
          VariableDescription("tfel::math::derivative_type<strainrate,stress>",
                              df_dseq.str(), 1u, 0u));
      flow.flow = FlowHandler::CreepFlow;
    } else if (this->current->value == "StrainHardeningCreep") {
      std::ostringstream p;
      std::ostringstream f;
      std::ostringstream df_dseq;
      std::ostringstream df_dp;
      p << "p" << this->flows.size();
      f << "f" << this->flows.size();
      df_dseq << "df_dseq" << this->flows.size();
      df_dp << "df_dp" << this->flows.size();
      this->mb.addStateVariable(uh,
                                VariableDescription("strain", p.str(), 1u, 0u),
                                BehaviourData::FORCEREGISTRATION);
      this->mb.addLocalVariable(
          uh, VariableDescription("strainrate", f.str(), 1u, 0u));
      this->mb.addLocalVariable(
          uh,
          VariableDescription("tfel::math::derivative_type<strainrate,stress>",
                              df_dseq.str(), 1u, 0u));
      this->mb.addLocalVariable(
          uh, VariableDescription("strainrate", df_dp.str(), 1u, 0u));
      flow.flow = FlowHandler::StrainHardeningCreepFlow;
    } else {
      this->throwRuntimeError("MultipleIsotropicMisesFlowsDSL::treatFlowRule",
                              "Unknown flow rule (read '" +
                                  this->current->value +
                                  "').Valid flow rules are 'Plasticity', "
                                  "'Creep' and 'StrainHardeningCreep'.");
    }
    ++(this->current);
    this->checkNotEndOfFile(
        "MultipleIsotropicMisesFlowsDSL::treatFlowRule",
        "Expected the beginning of a block or a specific theta value.");
    if (this->current->value != "{") {
      std::istringstream converter(this->current->value);
      std::ostringstream otheta;
      std::ostringstream ose;
      std::ostringstream oseq_e;
      flow.hasSpecificTheta = true;
      converter >> flow.theta;
      if (!converter || (!converter.eof())) {
        this->throwRuntimeError("MultipleIsotropicMisesFlowsDSL::treatFlowRule",
                                "Could not read theta value (read '" +
                                    this->current->value + "').");
      }
      otheta << "mu_3_theta" << this->flows.size();
      ose << "se" << this->flows.size();
      oseq_e << "seq_e" << this->flows.size();
      this->reserveName(ose.str());
      this->mb.addLocalVariable(
          uh, VariableDescription("stress", oseq_e.str(), 1u, 0u));
      ++(this->current);
    } else {
      flow.hasSpecificTheta = false;
    }
    std::ostringstream cname;
    cname << BehaviourData::FlowRule << flows.size() << '\n';
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->flowRuleVariableModifier(h, sv, b);
        };
    this->treatCodeBlock(cname.str(), m, true, false);
    flow.flowRule =
        this->mb.getCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS, cname.str());
    this->flows.push_back(flow);
  }  // end of treatFlowRule

  MultipleIsotropicMisesFlowsDSL::~MultipleIsotropicMisesFlowsDSL() = default;

}  // end of namespace mfront
