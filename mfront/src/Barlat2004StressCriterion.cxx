/*!
 * \file   mfront/src/Barlat2004StressCriterion.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/Barlat2004StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> Barlat2004StressCriterion::getOptions()
        const {
      std::vector<OptionDescription> opts;
      opts.emplace_back("l1", "First linear transformation coefficients",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      opts.emplace_back("l2", "Second linear transformation coefficients",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      opts.emplace_back("a", "Barlat exponent",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back(
          "eigen_solver",
          "Choice of the eigen solver. Valid value are 'default' and 'Jacoby'.",
          OptionDescription::STRING);
      return opts;
    }  // end of Barlat2004StressCriterion::getOptions

    std::vector<mfront::BehaviourSymmetryType>
    Barlat2004StressCriterion::getSupportedBehaviourSymmetries() const {
      return {mfront::ORTHOTROPIC};
    }  // end of Barlat2004StressCriterion::getSupportedBehaviourSymmetries

    void Barlat2004StressCriterion::initialize(BehaviourDescription& bd,
                                               AbstractBehaviourDSL& dsl,
                                               const std::string& id,
                                               const DataMap& d,
                                               const Role r) {
      using ConstantMaterialProperty =
          mfront::BehaviourDescription::ConstantMaterialProperty;
      tfel::raise_if(bd.getSymmetryType() != mfront::ORTHOTROPIC,
                     "Barlat2004StressCriterion::initialize: "
                     "the behaviour must be orthotropic");
      // this is captured for gcc 4.7.2
      auto get_l = [&dsl, &bd, &d, &r, &id, this](const std::string& n,
                                                  const char* const en) {
        if (d.count(n) == 0) {
          tfel::raise("Barlat2004StressCriterion::initialize: entry '" +
                      std::string(n) + "' is not defined");
        }
        const auto vn = StressCriterion::getVariableId(n + "v", id, r);
        const auto l = getArrayOfBehaviourDescriptionMaterialProperties<9u>(
            dsl, n, d.at(n));
        if (l[0].is<ConstantMaterialProperty>()) {
          std::vector<double> values(9u);
          for (unsigned short i = 0; i != 9; ++i) {
            tfel::raise_if(!l[i].is<ConstantMaterialProperty>(),
                           "Barlat2004StressCriterion::initialize: "
                           "if one component of '" +
                               n +
                               "' is a constant value, all components must be "
                               "constant values");
            values[i] = l[i].get<ConstantMaterialProperty>().value;
          }
          addParameter(bd, vn, en, 9, values);
        } else {
          addLocalVariable(bd, "real", vn, 9);
        }
        return l;
      };
      StressCriterionBase::initialize(bd, dsl, id, d, r);
      if (r == StressCriterion::STRESSANDFLOWCRITERION) {
        this->l1 = get_l("l1", "FirstLinearTransformationCoefficients");
        this->l2 = get_l("l2", "SecondLinearTransformationCoefficients");
      } else if (r == StressCriterion::STRESSCRITERION) {
        this->l1 =
            get_l("l1", "StressCriterionFirstLinearTransformationCoefficients");
        this->l2 = get_l(
            "l2", "StressCriterionSecondLinearTransformationCoefficients");
      } else {
        this->l1 =
            get_l("l1", "FlowCriterionFirstLinearTransformationCoefficients");
        this->l2 =
            get_l("l2", "FlowCriterionSecondLinearTransformationCoefficients");
      }
      const auto an = StressCriterion::getVariableId("a", id, r);
      tfel::raise_if(d.count("a") == 0,
                     "Barlat2004StressCriterion::initialize: "
                     "material property 'a' is not defined");
      this->a = getBehaviourDescriptionMaterialProperty(dsl, "a", d.at("a"));
      if (d.count("eigen_solver") != 0) {
        this->eigen_solver =
            handleEigenSolverOption(d.at("eigen_solver").get<std::string>());
      }
      declareParameterOrLocalVariable(bd, this->a, "real", an);
      addLocalVariable(bd, "Stensor4",
                       StressCriterion::getVariableId("l1", id, r));
      addLocalVariable(bd, "Stensor4",
                       StressCriterion::getVariableId("l2", id, r));
    }  // end of Barlat2004StressCriterion::initialize

    void Barlat2004StressCriterion::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& id,
        const Role r) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto an = StressCriterion::getVariableId("a", id, r);
      const auto l1n = StressCriterion::getVariableId("l1", id, r);
      const auto l2n = StressCriterion::getVariableId("l2", id, r);
      const auto l1vn = StressCriterion::getVariableId("l1v", id, r);
      const auto l2vn = StressCriterion::getVariableId("l2v", id, r);
      auto c = std::string{};
      c += generateMaterialPropertyInitializationCode(dsl, bd, an, this->a);
      c +=
          generateMaterialPropertiesInitializationCode(dsl, bd, l1vn, this->l1);
      c +=
          generateMaterialPropertiesInitializationCode(dsl, bd, l2vn, this->l2);
      const auto oac = [&bd]() -> std::string {
        if (bd.getOrthotropicAxesConvention() ==
            tfel::material::OrthotropicAxesConvention::PIPE) {
          return "tfel::material::OrthotropicAxesConvention::PIPE";
        } else if (bd.getOrthotropicAxesConvention() ==
                   tfel::material::OrthotropicAxesConvention::PLATE) {
          return "tfel::material::OrthotropicAxesConvention::PLATE";
        } else {
          tfel::raise_if(
              bd.getOrthotropicAxesConvention() !=
                  tfel::material::OrthotropicAxesConvention::DEFAULT,
              "Barlat2004StressCriterion::endTreatment: "
              "internal error, unsupported orthotropic axes convention");
          for (const auto mh : bd.getDistinctModellingHypotheses()) {
            tfel::raise_if(
                mh != ModellingHypothesis::TRIDIMENSIONAL,
                "Barlat2004StressCriterion::endTreatment: "
                "an orthotropic axes convention must be choosen when "
                "defining a stress free expansion in behaviours "
                "which shall be valid in other modelling hypothesis "
                "than 'Tridimensional'.\n"
                "Either restrict the validity of the behaviour to "
                "'Tridimensional' (see @ModellingHypothesis) or "
                "choose and orthotropic axes convention as on option "
                "to the @OrthotropicBehaviour keyword");
          }
        }
        return "tfel::material::OrthotropicAxesConvention::DEFAULT";
      }();
      c += "this->" + l1n + " = ";
      c += "makeBarlatLinearTransformation<hypothesis," + oac +
           ",real>(this->" + l1vn + ");\n";
      c += "this->" + l2n + " = ";
      c += "makeBarlatLinearTransformation<hypothesis," + oac +
           ",real>(this->" + l2vn + ");\n";
      if (!c.empty()) {
        CodeBlock i;
        i.code = c;
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of Barlat2004StressCriterion::endTreatment

    std::string Barlat2004StressCriterion::computeElasticPrediction(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto l1n = StressCriterion::getVariableId(
          "l1", id, StressCriterion::STRESSCRITERION);
      const auto l2n = StressCriterion::getVariableId(
          "l2", id, StressCriterion::STRESSCRITERION);
      return "const auto seqel" + id + " = computeBarlatStress" +
             this->getTemplateParameters() + "(sel" + id + ",this->" + l1n +
             ",this->" + l2n + ",this->" + an + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
    }  // end of Barlat2004StressCriterion::computeElasticPrediction

    std::string Barlat2004StressCriterion::computeCriterion(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto l1n = StressCriterion::getVariableId(
          "l1", id, StressCriterion::STRESSCRITERION);
      const auto l2n = StressCriterion::getVariableId(
          "l2", id, StressCriterion::STRESSCRITERION);
      return "const auto seq" + id + " = computeBarlatStress" +
             this->getTemplateParameters() + "(s" + id + ",this->" + l1n +
             ",this->" + l2n + ",this->" + an + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
    }  // end of Barlat2004StressCriterion::computeNormal

    std::string Barlat2004StressCriterion::computeNormal(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto l1n = StressCriterion::getVariableId(
          "l1", id, StressCriterion::STRESSCRITERION);
      const auto l2n = StressCriterion::getVariableId(
          "l2", id, StressCriterion::STRESSCRITERION);
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + "] = ";
        c += "computeBarlatStressNormal" + this->getTemplateParameters() +
             "(s" + id + ",this->" + l1n + ",this->" + l2n + "," + an + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seq" + id + ";\n";
        c += "Stensor dseq" + id + "_ds" + id + ";\n";
        c += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ") = ";
        c += "computeBarlatStressNormal" + this->getTemplateParameters() +
             "(s" + id + ",this->" + l1n + ",this->" + l2n + ",this->" + an +
             "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        c += "const auto [seqf" + id + ", n" + id + "] = ";
        c += "computeBarlatStressNormal" + this->getTemplateParameters() +
             "(s" + id + ",this->" + l1n + ",this->" + l2n + ",this->" + an +
             "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seqf" + id + ";\n";
        c += "Stensor n" + id + ";\n";
        c += "std::tie(seqf" + id + ",n" + id + ") = ";
        c += "computeBarlatStressNormal" + this->getTemplateParameters() +
             "(s" + id + ",this->" + l1n + ",this->" + l2n + ",this->" + an +
             "," + sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return c;
    }  // end of Barlat2004StressCriterion::computeNormal

    std::string Barlat2004StressCriterion::computeNormalDerivative(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto l1n = StressCriterion::getVariableId(
          "l1", id, StressCriterion::STRESSCRITERION);
      const auto l2n = StressCriterion::getVariableId(
          "l2", id, StressCriterion::STRESSCRITERION);
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" +
             id + "_ds" + id + "ds" + id + "] = ";
        c += "computeBarlatStressSecondDerivative" +
             this->getTemplateParameters() + "(s" + id + ", this->" + l1n +
             ",this->" + l2n + ", this->" + an + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seq" + id + ";\n";
        c += "Stensor dseq" + id + "_ds" + id + ";\n";
        c += "Stensor4 d2seq" + id + "_ds" + id + "ds" + id + ";\n";
        c += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" + id +
             "_ds" + id + "ds" + id + ") = ";
        c += "computeBarlatStressSecondDerivative" +
             this->getTemplateParameters() + "(s" + id + ",this->" + l1n +
             ",this->" + l2n + ",this->" + an + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
        c += "const auto& dn" + id + "_ds" + id + " = ";
        c += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        c += "const auto [seqf" + id + ", n" + id + ", dn" + id + "_ds" + id +
             "] = ";
        c += "computeBarlatStressSecondDerivative" +
             this->getTemplateParameters() + "(s" + id + ",this->" + l1n +
             ",this->" + l2n + ",this->" + an + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seqf" + id + ";\n";
        c += "Stensor n" + id + ";\n";
        c += "Stensor4 dn" + id + "_ds" + id + ";\n";
        c +=
            "std::tie(seqf" + id + ",n" + id + ",dn" + id + "_ds" + id + ") = ";
        c += "computeBarlatStressSecondDerivative<" +
             this->getTemplateParameters() + ">(s" + id + ",this->" + l1n +
             ",this->" + l2n + ",this->" + an + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return c;
    }  // end of Barlat2004StressCriterion::computeNormalDerivative

    bool Barlat2004StressCriterion::isCoupledWithPorosityEvolution() const {
      return false;
    }  // end of Barlat2004StressCriterion::isCoupledWithPorosityEvolution

    bool Barlat2004StressCriterion::isNormalDeviatoric() const {
      return true;
    }  // end of Barlat2004StressCriterion::isNormalDeviatoric

    StressCriterion::PorosityEffectOnFlowRule
    Barlat2004StressCriterion::getPorosityEffectOnEquivalentPlasticStrain()
        const {
      return StressCriterion::NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
    }  // end of
       // Barlat2004StressCriterion::getPorosityEffectOnEquivalentPlasticStrain()

    std::string Barlat2004StressCriterion::getTemplateParameters() const {
      if (this->eigen_solver.empty()) {
        return "";
      }
      return "<StressStensor, real, " + this->eigen_solver + ">";
    }  // end of Barlat2004StressCriterion::getTemplateParameters

    Barlat2004StressCriterion::~Barlat2004StressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
