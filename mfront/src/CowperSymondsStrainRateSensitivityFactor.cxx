/*!
 * \file   mfront/src/CowperSymondsStrainRateSensitivityFactor.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/11/2024
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/CowperSymondsStrainRateSensitivityFactor.hxx"

namespace mfront::bbrick {

  void CowperSymondsStrainRateSensitivityFactor::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id,
      const DataMap& d) {
    using namespace tfel::glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto get_mp = [&dsl, &bd, &fid, &id, &d](const std::string& mpn,
                                             const std::string& t,
                                             const std::string& vn) {
      if (d.count(mpn) == 0) {
        tfel::raise(
            "CowperSymondsStrainRateSensitivityFactor::initialize: "
            "material property '" +
            mpn + "' is not defined");
      }
      const auto ni = StrainRateSensitivityFactor::getVariableId(vn, fid, id);
      auto mp = getBehaviourDescriptionMaterialProperty(dsl, mpn, d.at(mpn));
      declareParameterOrLocalVariable(bd, mp, t, ni);
      return mp;
    };
    mfront::bbrick::check(d, this->getOptions());
    if (d.count("A") != 0) {
      this->A = get_mp("A", "real", "A");
    }
    this->dp0 = get_mp("dp0", "strainrate", "dp0");
    this->n = get_mp("n", "real", "E");
    if (d.count("Rs_eps") != 0) {
      this->Rs_eps = get_mp("Rs_eps", "strain", "Rs_eps");
      if (!this->Rs_eps.is<BehaviourDescription::ConstantMaterialProperty>()) {
        tfel::raise(
            "CowperSymondsStrainRateSensitivityFactor::initialize: "
            "the 'Rs_eps' parameter must be a constant");
      }
      const auto Rs_epsn =
          StrainRateSensitivityFactor::getVariableId("Rs_eps", fid, id);
      const auto nn = StrainRateSensitivityFactor::getVariableId("E", fid, id);
      const auto dp0n =
          StrainRateSensitivityFactor::getVariableId("dp0", fid, id);
      const auto initial_slopen =
          StrainRateSensitivityFactor::getVariableId("initial_slope", fid, id);
      const auto delta_p_epsn =
          StrainRateSensitivityFactor::getVariableId("delta_p_eps", fid, id);
      mfront::bbrick::addLocalVariable(bd, "real", initial_slopen);
      mfront::bbrick::addLocalVariable(bd, "strain", delta_p_epsn);
      CodeBlock init;
      if (!this->A.empty()) {
        const auto An =
            StrainRateSensitivityFactor::getVariableId("A", fid, id);
        init.code += "this->" + delta_p_epsn + " = (" + dp0n +
                     ")*(this->dt)*(std::pow(" + Rs_epsn + "/" + An + ", 1 / " +
                     nn + "));\n";
      } else {
        init.code += "this->" + delta_p_epsn + " = (" + dp0n +
                     ")*(this->dt)*(std::pow(" + Rs_epsn + " , 1 / " + nn +
                     "));\n";
      }
      init.code += "this->" + initial_slopen + " = ";
      init.code += Rs_epsn + " / " + delta_p_epsn + ";\n";
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    }
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    const auto dRs = "d" + Rs + "_ddp" + fid;
    bd.reserveName(uh, Rs);
    bd.reserveName(uh, dRs);
  }  // end of initialize

  std::vector<OptionDescription>
  CowperSymondsStrainRateSensitivityFactor::getOptions() const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("A", "Multiplicative coefficient",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("dp0", "Reference strain rate",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("Rs_eps", "small strain",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("n", "CowperSymonds exponent",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  std::string
  CowperSymondsStrainRateSensitivityFactor::computeStrainRateSensitivityFactor(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    const auto dp0n =
        StrainRateSensitivityFactor::getVariableId("dp0", fid, id);
    const auto nn = StrainRateSensitivityFactor::getVariableId("E", fid, id);
    const auto dpn = "dp" + fid;
    auto c = "const auto " + Rs + " = [&]() -> real {\n";
    c += "if(" + dpn + " < strain{0}){\n";
    c += "  return real{1};\n";
    c += "}\n";
    if (!this->Rs_eps.empty()) {
      const auto delta_p_epsn =
          "this->" +
          StrainRateSensitivityFactor::getVariableId("delta_p_eps", fid, id);
      const auto initial_slopen =
          "this->" +
          StrainRateSensitivityFactor::getVariableId("initial_slope", fid, id);
      c += "  if(" + dpn + " < " + delta_p_epsn + "){\n";
      c += "    return 1 + (" + initial_slopen + ") * (" + dpn + ");\n";
      c += "  }\n";
    }
    if (!this->A.empty()) {
      const auto An =
          "this->" + StrainRateSensitivityFactor::getVariableId("A", fid, id);
      c += "  return 1 + (" + An + ") * std::pow(" + dpn + " / (" + dp0n +
           " * (this->dt)), " + nn + ");\n";
    } else {
      c += "  return 1 + std::pow(" + dpn + " / (" + dp0n + " * (this->dt)), " +
           nn + ");\n";
    }
    c += "}();\n";
    return c;
  }  // end of computeStrainRateSensitivityFactor

  std::string CowperSymondsStrainRateSensitivityFactor::
      computeStrainRateSensitivityFactorAndDerivative(
          const BehaviourDescription& bd,
          const std::string& fid,
          const std::string& id) const {
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    const auto dRs = "d" + Rs + "_ddp" + fid;
    const auto An = StrainRateSensitivityFactor::getVariableId("A", fid, id);
    const auto dp0n =
        StrainRateSensitivityFactor::getVariableId("dp0", fid, id);
    const auto nn = StrainRateSensitivityFactor::getVariableId("E", fid, id);
    const auto dpn = "dp" + fid;
    auto c = this->computeStrainRateSensitivityFactor(bd, fid, id);
    c += "const auto " + dRs + " = [&]() -> real {\n";
    c += "if(" + dpn + " < strain{0}){\n";
    c += "  return real{};\n";
    c += "}\n";
    if (!this->Rs_eps.empty()) {
      const auto Rs_epsn =
          "this->" +
          StrainRateSensitivityFactor::getVariableId("Rs_eps", fid, id);
      const auto delta_p_epsn =
          "this->" +
          StrainRateSensitivityFactor::getVariableId("delta_p_eps", fid, id);
      const auto initial_slope_n =
          "this->" +
          StrainRateSensitivityFactor::getVariableId("initial_slope", fid, id);
      c += "if(" + dpn + " < " + delta_p_epsn + "){\n";
      c += "   return " + initial_slope_n + ";\n";
      c += "}\n";
      c += "return " + nn + " * (" + Rs + " - 1) / std::max(strain(" + dpn +
           "), " + delta_p_epsn + ");\n";
    } else {
      c += "return " + nn + " * (" + Rs + " - 1) / std::max(strain(" + dpn +
           "), static_cast<strain>(this->epsilon));\n";
    }
    c += "}();\n";
    return c;
  }  // end of computeStrainRateSensitivityFactorAndDerivative

  void CowperSymondsStrainRateSensitivityFactor::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id) const {
    auto mts = getMiddleOfTimeStepModifier(bd);
    // computation of the material properties
    std::ostringstream mps;
    if (!this->A.empty()) {
      if (!this->A.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto An =
            StrainRateSensitivityFactor::getVariableId("A", fid, id);
        mps << "this->" + An + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->A, mts);
        mps << ";\n";
      }
    }
    if (!this->dp0.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto dp0n =
          StrainRateSensitivityFactor::getVariableId("dp0", fid, id);
      mps << "this->" + dp0n + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->dp0, mts);
      mps << ";\n";
    }
    if (!this->n.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto nn = StrainRateSensitivityFactor::getVariableId("E", fid, id);
      mps << "this->" + nn + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->n, mts);
      mps << ";\n";
    }
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock i;
    i.code += mps.str();
    if (!i.code.empty()) {
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  CowperSymondsStrainRateSensitivityFactor::
      ~CowperSymondsStrainRateSensitivityFactor() = default;

}  // end of namespace mfront::bbrick