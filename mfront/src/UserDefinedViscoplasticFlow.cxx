/*!
 * \file   UserDefinedViscoplasticFlow.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/UserDefinedViscoplasticFlow.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> UserDefinedViscoplasticFlow::getOptions()
      const {
    auto opts = ViscoplasticFlowBase::getOptions();
    opts.emplace_back("vp", "viscoplastic strain rate.",
                      OptionDescription::STRING);
    opts.emplace_back("dvp_df",
                      "derivative of the viscoplastic strain rate with respect "
                      "to the effective equivalent stresss.",
                      OptionDescription::STRING);
    opts.emplace_back(
        "dvp_dp",
        "partial derivative of the viscoplastic strain rate with respect "
        "to the equivalent viscoplastic strain, assuming the effective "
        "equivalent stress constant.",
        OptionDescription::STRING);
    return opts;
  }  // end of getOptions

  void UserDefinedViscoplasticFlow::checkOptions(const DataMap&) const {
  }  // end of checkOptions

  void UserDefinedViscoplasticFlow::initialize(BehaviourDescription& bd,
                                               AbstractBehaviourDSL& dsl,
                                               const std::string& id,
                                               const DataMap& d) {
    //
    ViscoplasticFlowBase::initialize(bd, dsl, id, d);
    //
    if (d.count("vp") == 0) {
      tfel::raise(
          "UserDefinedViscoplasticFlow::initialize: "
          "the strain rate (given by option 'vp') is not defined");
    }
    this->vp.setFunction(d.at("vp").get<std::string>());
    const auto& variables = this->vp.getVariablesNames();
    if (std::find(variables.begin(), variables.end(), "f") == variables.end()) {
      tfel::raise(
          "UserDefinedViscoplasticFlow::initialize: "
          "the strain rate does not depend on 'f'");
    }
    for (const auto& rv : {"vp", "seps"}) {
      if (std::find(variables.begin(), variables.end(), rv) !=
          variables.end()) {
        tfel::raise(
            "UserDefinedViscoplasticFlow::initialize: "
            "the strain rate shall not depend on a variable called '" +
            std::string{rv} + "'");
      }
    }
    const auto& opts = this->getOptions();
    for (const auto& opt : d) {
      if ((opt.first == "f") || (opt.first == "p")) {
        tfel::raise("UserDefinedViscoplasticFlow::initialize: '" + opt.first +
                    "' is not a valid option");
      }
      if (std::any_of(opts.begin(), opts.end(),
                      [&opt](const OptionDescription& o) {
                        return o.name == opt.first;
                      })) {
        continue;
      }
      if (std::find(variables.begin(), variables.end(), opt.first) ==
          variables.end()) {
        tfel::raise(
            "UserDefinedViscoplasticFlow::initialize: "
            "unexpected option '" +
            opt.first + "'");
      }
      auto mp =
          getBehaviourDescriptionMaterialProperty(dsl, opt.first, opt.second);
      declareParameterOrLocalVariable(bd, mp, "real", opt.first + id);
      this->mps.insert({opt.first, mp});
    }
    for (const auto& v : variables) {
      if ((v == "f") || (v == "p") || (this->mps.count(v) != 0)) {
        continue;
      }
      // check that the variable is valid
      bd.getMaterialPropertyInput(v, false);
    }
    //
    auto add_derivative_if =
        [&variables,
         &d](const std::string& n) -> std::optional<tfel::math::Evaluator> {
      if (d.count(n) == 0) {
        return {};
      }
      const auto df = tfel::math::Evaluator(d.at(n).get<std::string>());
      const auto df_variables = df.getVariablesNames();
      for (const auto& v : df_variables) {
        if ((v == "vp") || (v == "seps")) {
          continue;
        }
        if (std::find(variables.begin(), variables.end(), v) ==
            variables.end()) {
          auto msg = std::string{};
          msg += "UserDefinedViscoplasticFlow::initialize: ";
          msg += "error while defining derivative '" + n + "'. ";
          msg += "The derivative depends on a variable which is ";
          msg += "not a variable of the viscoplastic strain rate.\n";
          msg += "List of variables of the viscoplastic strain rate:";
          for (const auto& vn : variables) {
            msg += " " + vn;
          }
          msg += "\nList of variables of the derivative '" + n + "':";
          for (const auto& vn : df_variables) {
            msg += " " + vn;
          }
          tfel::raise(msg);
        }
      }
      return df;
    };
    //
    this->dvp_df = add_derivative_if("dvp_df");
    //
    if (!this->describesStrainHardeningExplicitly()) {
      if (d.count("dvp_dp") != 0) {
        tfel::raise(
            "UserDefinedViscoplasticFlow::initialize: "
            "the viscoplastic strain rate does not depend explicitely "
            "on the equivalent viscoplastic strain, so defining the derivative "
            "`dvp_dp` is meaningless");
      }
    }
    this->dvp_dp = add_derivative_if("dvp_dp");
  }  // end of initialize

  void UserDefinedViscoplasticFlow::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const StressPotential& sp,
      const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    ViscoplasticFlowBase::endTreatment(bd, dsl, sp, id);
    auto mts = getMiddleOfTimeStepModifier(bd);
    CodeBlock i;
    for (const auto& mp : mps) {
      i.code += generateMaterialPropertyInitializationCode(
          dsl, bd, mp.first + id, mp.second);
    }
    if (!i.code.empty()) {
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  bool UserDefinedViscoplasticFlow::describesStrainHardeningExplicitly() const {
    const auto& variables = this->vp.getVariablesNames();
    return std::find(variables.begin(), variables.end(), "p") !=
           variables.end();
  }  // end of describesStrainHardeningExplicitly

  static std::map<std::string, std::string> getVariablesMap(
      const tfel::math::Evaluator& vp,
      const std::map<std::string, BehaviourDescription::MaterialProperty>& mps,
      const BehaviourDescription& bd,
      const std::string& id) {
    const auto mts = getMiddleOfTimeStepModifier(bd);
    auto m = std::map<std::string, std::string>{};
    m.insert({"f", "mfront_udvf_f" + id});
    for (const auto& mp : mps) {
      m.insert({mp.first, "this->" + mp.first + id});
    }
    const auto& variables = vp.getVariablesNames();
    for (const auto& v : variables) {
      if ((v == "f") || (mps.count(v) != 0)) {
        continue;
      }
      if (v == "p") {
        m.insert({"p", "this->p" + id + " + " +  //
                           bd.getClassName() + "::theta * (this->dp" + id +
                           ")"});
      } else {
        const auto i = bd.getMaterialPropertyInput(v, false);
        m.insert({v, mts(i)});
      }
    }
    return m;
  }  // end of getVariablesMap

  std::string UserDefinedViscoplasticFlow::computeFlowRate(
      const BehaviourDescription& bd,
      const StressPotential&,
      const std::string& id) const {
    const auto m = getVariablesMap(this->vp, this->mps, bd, id);
    auto c = std::string{};
    c += "const auto mfront_udvf_f" + id + " = ";
    if (this->ihrs.empty()) {
      c += "seq" + id + ";\n";
    } else {
      c += "seq" + id + " - R" + id + ";\n";
    }
    c += "const auto vp" + id + " = [this, mfront_udvf_f" + id + "] {\n";
    c += "  if(mfront_udvf_f" + id + " >= stress{0}){\n";
    c += "   return strainrate{" + this->vp.getCxxFormula(m) + "};\n";
    c += "  }\n";
    c += "  return strainrate{0};\n";
    c += "}();\n";
    return c;
  }  // end of computeFlowRate

  std::string UserDefinedViscoplasticFlow::computeFlowRateAndDerivative(
      const BehaviourDescription& bd,
      const StressPotential& sp,
      const std::string& id) const {
    const auto m = getVariablesMap(this->vp, this->mps, bd, id);
    const auto buildDerivativesVariablesMap =
        [&m, &id, &sp, &bd](const tfel::math::Evaluator& df) {
          auto r = std::map<std::string, std::string>{};
          for (const auto& v : df.getVariablesNames()) {
            if (v == "vp") {
              r.insert({"vp", "vp" + id});
              continue;
            }
            if (v == "seps") {
              r.insert({"seps", sp.getEquivalentStressLowerBound(bd)});
              continue;
            }
            if (m.count(v) == 0) {
              tfel::raise(
                  "UserDefinedViscoplasticFlow::computeFlowRateAndDerivative: "
                  "internal error (unexpected variable '" +
                  v + "' in derivative)");
            }
            r.insert({v, m.at(v)});
          }
          return r;
        };
    auto c = std::string{};
    c += "const auto mfront_udvf_f" + id + " = ";
    if (this->ihrs.empty()) {
      c += "seq" + id + ";\n";
    } else {
      c += "seq" + id + " - R" + id + ";\n";
    }
    c += "const auto vp" + id + " = [this, mfront_udvf_f" + id + "] {\n";
    c += "  if(mfront_udvf_f" + id + " >= stress{0}){\n";
    c += "   return strainrate{" + this->vp.getCxxFormula(m) + "};\n";
    c += "  }\n";
    c += "  return strainrate{0};\n";
    c += "}();\n";
    //
    const auto cl = [id](const auto& dv) {
      auto r = "this, mfront_udvf_f" + id;
      if (dv.has_value()) {
        const auto df_variables = dv->getVariablesNames();
        if (std::find(df_variables.begin(), df_variables.end(), "vp") !=
            df_variables.end()) {
          r += ", vp" + id;
        }
      }
      return r;
    };
    c += "const auto dvp" + id + "_dseqe" + id + " = ";
    c += "[" + cl(this->dvp_df) + "] {\n";
    c += "  if(mfront_udvf_f" + id + " >= stress{0}){\n";
    c += "return derivative_type<strainrate, stress>{";
    if (this->dvp_df.has_value()) {
      c += this->dvp_df->getCxxFormula(
               buildDerivativesVariablesMap(*(this->dvp_df))) +
           "};\n";
    } else {
      const auto dvp = std::dynamic_pointer_cast<tfel::math::Evaluator>(
          this->vp.differentiate("f"));
      c += dvp->getCxxFormula(m) + "};\n";
    }
    c += "  }\n";
    c += "  return derivative_type<strainrate, stress>{0};\n";
    c += "}();\n";
    if (this->describesStrainHardeningExplicitly()) {
      c += "const auto dvp" + id + "_dp" + id + " = ";
      c += "[" + cl(this->dvp_dp) + "] {\n";
      c += "  if(mfront_udvf_f" + id + " >= stress{0}){\n";
      c += "return derivative_type<strainrate, strain>{";
      if (this->dvp_dp.has_value()) {
        c += this->dvp_dp->getCxxFormula(
                 buildDerivativesVariablesMap(*(this->dvp_dp))) +
             "};\n";
      } else {
        const auto dvp = std::dynamic_pointer_cast<tfel::math::Evaluator>(
            this->vp.differentiate("p"));
        c += dvp->getCxxFormula(m) + "};\n";
      }
      c += "  }\n";
      c += "  return derivative_type<strainrate, strain>{0};\n";
      c += "}();\n";
    }
    return c;
  }  // end of computeFlowRateAndDerivative

  UserDefinedViscoplasticFlow::~UserDefinedViscoplasticFlow() = default;

}  // end of namespace mfront::bbrick
