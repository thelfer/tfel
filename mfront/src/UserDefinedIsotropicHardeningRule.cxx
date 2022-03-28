/*!
 * \file   mfront/src/UserDefinedIsotropicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2022
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/UserDefinedIsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> UserDefinedIsotropicHardeningRule::getOptions()
      const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("R", "Yield radius", OptionDescription::STRING);
    opts.emplace_back("dR_dp",
                      "Derivative of the yield radius with respect to the "
                      "equivalent plastic strain",
                      OptionDescription::STRING);
    return opts;
  }  // end of getOptions

  void UserDefinedIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                                     AbstractBehaviourDSL& dsl,
                                                     const std::string& fid,
                                                     const std::string& id,
                                                     const DataMap& d) {
    using namespace tfel::glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (d.count("R") == 0) {
      tfel::raise(
          "UserDefinedIsotropicHardeningRule::initialize: "
          "The yield radius shall be defined by an option named 'R'");
    }
    const auto R_opt = d.at("R");
    if (!R_opt.is<std::string>()) {
      tfel::raise(
          "UserDefinedIsotropicHardeningRule::initialize: "
          "option 'R' is not a string");
    }
    this->R.setFunction(R_opt);
    const auto& variables = this->R.getVariablesNames();
    if (std::find(variables.begin(), variables.end(), "p") == variables.end()) {
      tfel::raise(
          "UserDefinedIsotropicHardeningRule::initialize: "
          "the strain rate does not depend on "
          "the equivalent plastic strain 'p'");
    }
    const auto& opts = this->getOptions();
    for (const auto& opt : d) {
      if ((opt.first == "p")) {
        tfel::raise("UserDefinedIsotropicHardeningRule::initialize: '" + opt.first +
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
            "UserDefinedIsotropicHardeningRule::initialize: "
            "unexpected option '" +
            opt.first + "'");
      }
      const auto opt_id = IsotropicHardeningRule::getVariableId(opt.first, fid, id);
      auto mp =
          getBehaviourDescriptionMaterialProperty(dsl, opt.first, opt.second);
      declareParameterOrLocalVariable(bd, mp, "real", opt_id);
      this->mps.insert({opt.first + id, mp});
    }
    //
    if (d.count("dR_dp") != 0) {
      this->dR_dp = tfel::math::Evaluator(d.at("dR_dp").get<std::string>());
      for (const auto& v : this->dR_dp->getVariablesNames()) {
        if (v == "R") {
          continue;
        }
        if (std::find(variables.begin(), variables.end(), v) ==
            variables.end()) {
          auto msg = std::string{};
          msg += "UserDefinedIsotropicHardeningRule::initialize: ";
          msg += "error while defining derivative 'dR_dp'. ";
          msg += "The derivative depends on a variable which is ";
          msg += "not a variable of the yield radius 'R'.\n";
          msg += "List of variables of the yield radius:";
          for (const auto& vn : variables) {
            msg += " " + vn;
          }
          msg += "\nList of variables of the derivative 'dR_dp':";
          for (const auto& vn : this->dR_dp->getVariablesNames()) {
            msg += " " + vn;
          }
          tfel::raise(msg);
        }
      }
    }
    //
    const auto Rel_id = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R_id = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR_id = "d" + R_id + "_ddp" + fid;
    bd.reserveName(uh, Rel_id);
    bd.reserveName(uh, R_id);
    bd.reserveName(uh, dR_id);
  }  // end of initialize

  void UserDefinedIsotropicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto mts = getMiddleOfTimeStepModifier(bd);
    CodeBlock i;
    for (const auto mp : mps) {
      const auto mp_id =
          IsotropicHardeningRule::getVariableId(mp.first, fid, id);
      i.code +=
          generateMaterialPropertyInitializationCode(dsl, bd, mp_id, mp.second);
    }
    if (!i.code.empty()) {
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  static std::map<std::string, std::string> getVariablesMap(
      const tfel::math::Evaluator& R,
      const std::map<std::string, BehaviourDescription::MaterialProperty>& mps,
      const BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id,
      const std::string& p) {
    auto m = std::map<std::string, std::string>{};
    m.insert({"p", p});
    for (const auto mp : mps) {
      const auto mp_id =
          IsotropicHardeningRule::getVariableId(mp.first, fid, id);
      m.insert({mp.first, "this->" + mp_id});
    }
    const auto& variables = R.getVariablesNames();
    for (const auto v : variables) {
      if ((v == "p") || (mps.count(v) != 0)) {
        continue;
      }
      const auto i = bd.getMaterialPropertyInput(v, false);
      m.insert({v, mts(i)});
    }
    return m;
  }  // end of getVariablesMap

  std::string UserDefinedIsotropicHardeningRule::computeElasticPrediction(
      const BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto m =
        getVariablesMap(this->R, this->mps, bd, fid, id, "this->p" + fid);
    return "const auto " + Rel + " = " + this->R.getCxxFormula(m) + ";\n";
  }  // end of computeElasticPrediction

  std::string UserDefinedIsotropicHardeningRule::computeElasticLimit(
      const BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto m = getVariablesMap(
        this->R, this->mps, bd, fid, id,
        "(this->p" + fid + "+(this->theta) * (this->dp" + fid + "))");
    return "const auto " + R + " = " + this->R.getCxxFormula(m) + ";\n";
  }  // end of computeElasticLimit

  std::string
  UserDefinedIsotropicHardeningRule::computeElasticLimitAndDerivative(
      const BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    //     const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    //     const auto dR = "d" + R + "_ddp" + fid;
    //     const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid,
    //     id); const auto Rin = IsotropicHardeningRule::getVariableId("Rinf",
    //     fid, id); const auto bn = IsotropicHardeningRule::getVariableId("b",
    //     fid, id); return "const auto " + R + " = this->" + Rin + "+(this->" +
    //     R0n +
    //            "-this->" + Rin + ")*exp(-(this->" + bn + ")*(this->p" + fid +
    //            "+(this->theta)*(this->dp" + fid +
    //            ")));\n"  //
    //            "const auto " +
    //            dR +
    //            " = "
    //            "-(this->theta)*(this->" +
    //            bn + ")*(" + R + "-(this->" + Rin + "));\n";
  }  // end of computeElasticLimitAndDerivative

  UserDefinedIsotropicHardeningRule::~UserDefinedIsotropicHardeningRule() =
      default;

}  // end of namespace mfront::bbrick
