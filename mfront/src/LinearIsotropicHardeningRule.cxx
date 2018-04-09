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

namespace mfront{

  namespace bbrick {

    void LinearIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                                  AbstractBehaviourDSL& dsl,
                                                  const std::string& id,
                                                  const DataMap& d) {
      using namespace tfel::glossary;
      auto get_mp = [&dsl, &bd, &id, &d](const std::string& n) {
        if (d.count(n) == 0) {
          tfel::raise(
              "LinearIsotropicHardeningRule::initialize: "
              "material property '" +
              n + "' is not defined");
        }
        auto mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
        declareParameterOrLocalVariable(bd, mp, n + id);
        return mp;
      };
      mfront::bbrick::check(d, this->getOptions());
      this->R0 = get_mp("R0");
      this->H = get_mp("H");
    }  // end of LinearIsotropicHardeningRule::initialize

    std::vector<OptionDescription> LinearIsotropicHardeningRule::getOptions() const {
      std::vector<OptionDescription> opts;
      opts.emplace_back("R0", "Yield strength",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("H", "Hardening slope",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of LinearIsotropicHardeningRule::getOptions

    std::string LinearIsotropicHardeningRule::computeElasticPrediction(
        const std::string& id) const {
      return "const auto Rel" + id + " = this->R0" + id + "+(this->H" + id +
          ")*(this->p" + id + ");\n";
    }  // end of LinearIsotropicHardeningRule::computeElasticPrediction

    std::string LinearIsotropicHardeningRule::computeElasticLimit(
        const std::string& id) const {
      return "const auto R" + id + " = this->R0" + id + "+(this->H" + id +
             ")*(this->p" + id + "+(this->theta)*(this->dp" + id + "));\n";
    }  // end of LinearIsotropicHardeningRule::computeElasticLimit

    std::string LinearIsotropicHardeningRule::computeElasticLimitAndDerivative(
        const std::string& id) const {
      return  //
          "const auto R" + id + " = this->R0" + id + "+(this->H" + id +
          ")*(this->p" + id + "+(this->theta)*(this->dp" + id +
          "));\n"  //
          "const auto dR" +
          id + "_ddp" + id + " = this->H" + id + ";\n";
    }  // end of LinearIsotropicHardeningRule::computeElasticLimitAndDerivative

    void LinearIsotropicHardeningRule::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& id) const {
      auto mts = getMiddleOfTimeStepModifier(bd);
      // computation of the material properties
      if ((!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) &&
          (!this->H.is<BehaviourDescription::ConstantMaterialProperty>())) {
        constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        CodeBlock i;
        std::ostringstream mps;
        if (!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) {
          mps << "this->R0" + id + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->R0, mts);
          mps << ";\n";
        }
        if (!this->H.is<BehaviourDescription::ConstantMaterialProperty>()) {
          mps << "this->H" + id + " = ";
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