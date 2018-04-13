/*!
 * \file   mfront/src/LinearIsotropicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/LinearIsotropicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    void LinearIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                                  AbstractBehaviourDSL& dsl,
                                                  const std::string& fid,
                                                  const std::string& id,
                                                  const DataMap& d) {
      using namespace tfel::glossary;
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      // this shall be captured in gcc 4.7.2
      auto get_mp = [&dsl, &bd, &fid, &id, &d,this](const std::string& n) {
        const auto ni = IsotropicHardeningRule::getVariableId(n, fid, id);
        if (d.count(n) == 0) {
          tfel::raise(
              "LinearIsotropicHardeningRule::initialize: "
              "material property '" +
              n + "' is not defined");
        }
        auto mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
        declareParameterOrLocalVariable(bd, mp, ni);
        return mp;
      };
      mfront::bbrick::check(d, this->getOptions());
      this->R0 = get_mp("R0");
      this->H = get_mp("H");
      const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
      const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
      const auto dR = "d" + R + "_ddp" + fid;
      bd.reserveName(uh,Rel);
      bd.reserveName(uh,R);
      bd.reserveName(uh,dR);
    }  // end of LinearIsotropicHardeningRule::initialize

    std::vector<OptionDescription> LinearIsotropicHardeningRule::getOptions()
        const {
      std::vector<OptionDescription> opts;
      opts.emplace_back("R0", "Yield strength",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("H", "Hardening slope",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of LinearIsotropicHardeningRule::getOptions

    std::string LinearIsotropicHardeningRule::computeElasticPrediction(
        const std::string& fid, const std::string& id) const {
      const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
      const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
      const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
      return "const auto " + Rel + " = this->" + R0n + "+(this->" + Hn +
             ")*(this->p" + fid + ");\n";
    }  // end of LinearIsotropicHardeningRule::computeElasticPrediction

    std::string LinearIsotropicHardeningRule::computeElasticLimit(
        const std::string& fid, const std::string& id) const {
      const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
      const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
      const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
      return "const auto " + R + " = this->" + R0n + "+(this->" + Hn +
             ")*(this->p" + fid + "+(this->theta)*(this->dp" + fid + "));\n";
    }  // end of LinearIsotropicHardeningRule::computeElasticLimit

    std::string LinearIsotropicHardeningRule::computeElasticLimitAndDerivative(
        const std::string& fid, const std::string& id) const {
      const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
      const auto dR = "d" + R + "_ddp" + fid;
      const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
      const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
      return  //
          "const auto " + R + " = this->" + R0n + "+(this->" + Hn +
          ")*(this->p" + fid + "+(this->theta)*(this->dp" + fid +
          "));\n"  //
          "const auto " +
          dR + " = this->" + Hn + ";\n";
    }  // end of LinearIsotropicHardeningRule::computeElasticLimitAndDerivative

    void LinearIsotropicHardeningRule::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& id) const {
      auto mts = getMiddleOfTimeStepModifier(bd);
      // computation of the material properties
      if ((!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) &&
          (!this->H.is<BehaviourDescription::ConstantMaterialProperty>())) {
        constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        CodeBlock i;
        std::ostringstream mps;
        if (!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) {
          const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
          mps << "this->" + R0n + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->R0, mts);
          mps << ";\n";
        }
        if (!this->H.is<BehaviourDescription::ConstantMaterialProperty>()) {
          const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
          mps << "this->" + Hn + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->H, mts);
          mps << ";\n";
        }
        i.code += mps.str();
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of LinearIsotropicHardeningRule::endTreatment

    LinearIsotropicHardeningRule::~LinearIsotropicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
