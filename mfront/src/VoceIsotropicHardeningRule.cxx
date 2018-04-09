/*!
 * \file   mfront/src/VoceIsotropicHardeningRule.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/VoceIsotropicHardeningRule.hxx"

namespace mfront{

  namespace bbrick {

    void VoceIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                                  AbstractBehaviourDSL& dsl,
                                                  const std::string& id,
                                                  const DataMap& d) {
      using namespace tfel::glossary;
      auto get_mp = [&dsl, &bd, &id, &d](const std::string& n) {
        if (d.count(n) == 0) {
          tfel::raise(
              "VoceIsotropicHardeningRule::initialize: "
              "material property '" +
              n + "' is not defined");
        }
        auto mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
        declareParameterOrLocalVariable(bd, mp, n + id);
        return mp;
      };
      mfront::bbrick::check(d, this->getOptions());
      this->R0 = get_mp("R0");
      this->Rinf = get_mp("Rinf");
      this->b = get_mp("b");
    }  // end of VoceIsotropicHardeningRule::initialize

    std::vector<OptionDescription> VoceIsotropicHardeningRule::getOptions() const {
      std::vector<OptionDescription> opts;
      opts.emplace_back("R0", "Yield strength",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("Rinf", "Ultimate Yield strength",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("b", "Parameter describing the hardening evolution",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of VoceIsotropicHardeningRule::getOptions

    std::string VoceIsotropicHardeningRule::computeElasticPrediction(
        const std::string& id) const {
      return "const auto Rel" + id + " = this->Rinf" + id + "+(this->R0" + id +
             "-Rinf" + id + ")*exp(-(this->b)*(this->p" + id + "));\n";
    }  // end of VoceIsotropicHardeningRule::computeElasticPrediction

    std::string VoceIsotropicHardeningRule::computeElasticLimit(
        const std::string& id) const {
      return "const auto R" + id + " = this->Rinf" + id + "+(this->R0" + id +
             "-Rinf" + id + ")*exp(-(this->b)*(this->p" + id +
             "+(this->theta)*(this->dp" + id + ")));\n";
    }  // end of VoceIsotropicHardeningRule::computeElasticLimit

    std::string VoceIsotropicHardeningRule::computeElasticLimitAndDerivative(
        const std::string& id) const {
      return "const auto R" + id + " = this->Rinf" + id + "+(this->R0" + id +
             "-Rinf" + id + ")*exp(-(this->b)*(this->p" + id +
             "+(this->theta)*(this->dp" + id +
             ")));\n"  //
             "const auto dR" +
             id + "_ddp" + id +
             " = "
             "-b*(R" +
             id + "-(this->Rinf" + id + "));\n";
    }  // end of VoceIsotropicHardeningRule::computeElasticLimitAndDerivative

    void VoceIsotropicHardeningRule::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& id) const {
      auto mts = getMiddleOfTimeStepModifier(bd);
      // computation of the material properties
      if ((!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) &&
          (!this->Rinf.is<BehaviourDescription::ConstantMaterialProperty>()) &&
          (!this->b.is<BehaviourDescription::ConstantMaterialProperty>())) {
        constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        CodeBlock i;
        std::ostringstream mps;
        if (!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) {
          mps << "this->R0" + id + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->R0, mts);
          mps << ";\n";
        }
        if (!this->Rinf.is<BehaviourDescription::ConstantMaterialProperty>()) {
          mps << "this->Rinf" + id + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->Rinf, mts);
          mps << ";\n";
        }
        if (!this->b.is<BehaviourDescription::ConstantMaterialProperty>()) {
          mps << "this->b" + id + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->b, mts);
          mps << ";\n";
        }
        i.code += mps.str();
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of VoceIsotropicHardeningRule::endTreatment

    VoceIsotropicHardeningRule::~VoceIsotropicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront