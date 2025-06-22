/*!
 * \file   mfront/src/JohnsonCookStrainRateSensitivityFactor.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/11/2024
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/JohnsonCookStrainRateSensitivityFactor.hxx"

namespace mfront::bbrick {

  void JohnsonCookStrainRateSensitivityFactor::initialize(
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
            "JohnsonCookStrainRateSensitivityFactor::initialize: "
            "material property '" +
            mpn + "' is not defined");
      }
      const auto ni = StrainRateSensitivityFactor::getVariableId(vn, fid, id);
      auto mp = getBehaviourDescriptionMaterialProperty(dsl, mpn, d.at(mpn));
      declareParameterOrLocalVariable(bd, mp, t, ni);
      return mp;
    };
    mfront::bbrick::check(d, this->getOptions());
    this->A = get_mp("A", "real", "A");
    this->dp0 = get_mp("dp0", "strainrate", "dp0");
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    const auto dRs = "d" + Rs + "_ddp" + fid;
    bd.reserveName(uh, Rs);
    bd.reserveName(uh, dRs);
  }  // end of initialize

  std::vector<OptionDescription>
  JohnsonCookStrainRateSensitivityFactor::getOptions() const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("A", "Multiplicative coefficient",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("dp0", "Reference strain rate",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  std::string
  JohnsonCookStrainRateSensitivityFactor::computeStrainRateSensitivityFactor(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    const auto An = StrainRateSensitivityFactor::getVariableId("A", fid, id);
    const auto dp0n =
        StrainRateSensitivityFactor::getVariableId("dp0", fid, id);
    const auto dpn = "dp" + fid;
    auto c = "const auto " + Rs + " = [&](){\n";
    c += "if(" + dpn + " < (" + dp0n + " * (this->dt))){\n";
    c += "  return real{1};\n";
    c += "}\n";
    c += "return 1 + (" + An + ") * std::log(" + dpn + " / (" + dp0n +
         " * (this->dt)));\n";
    c += "}();\n";
    return c;
  }  // end of computeStrainRateSensitivityFactor

  std::string JohnsonCookStrainRateSensitivityFactor::
      computeStrainRateSensitivityFactorAndDerivative(
          const BehaviourDescription& bd,
          const std::string& fid,
          const std::string& id) const {
    const auto Rs = id.empty() ? "Rs" + fid : "Rs" + fid + "_" + id;
    const auto dRs = "d" + Rs + "_ddp" + fid;
    const auto An = StrainRateSensitivityFactor::getVariableId("A", fid, id);
    const auto dp0n =
        StrainRateSensitivityFactor::getVariableId("dp0", fid, id);
    const auto dpn = "dp" + fid;
    auto c = this->computeStrainRateSensitivityFactor(bd, fid, id);
    c += "const auto " + dRs + " = [&](){\n";
    c += "if(" + dpn + " < (" + dp0n + " * (this->dt))){\n";
    c += "  return real{};\n";
    c += "}\n";
    c += "return (" + An + ") / (" + dpn + ");\n";
    c += "}();\n";
    return c;
  }  // end of computeStrainRateSensitivityFactorAndDerivative

  void JohnsonCookStrainRateSensitivityFactor::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id) const {
    auto mts = getMiddleOfTimeStepModifier(bd);
    // computation of the material properties
    std::ostringstream mps;
    if (!this->A.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto An = StrainRateSensitivityFactor::getVariableId("A", fid, id);
      mps << "this->" + An + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->A, mts);
      mps << ";\n";
    }
    if (!this->dp0.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto dp0n =
          StrainRateSensitivityFactor::getVariableId("dp0", fid, id);
      mps << "this->" + dp0n + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->dp0, mts);
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

  JohnsonCookStrainRateSensitivityFactor::
      ~JohnsonCookStrainRateSensitivityFactor() = default;

}  // end of namespace mfront::bbrick
