/*!
 * \file   HarmonicSumOfNortonHoffViscoplasticFlows.cxx
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
#include "MFront/BehaviourBrick/HarmonicSumOfNortonHoffViscoplasticFlows.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription>
    HarmonicSumOfNortonHoffViscoplasticFlows::getOptions() const {
      auto opts = ViscoplasticFlowBase::getOptions();
      opts.emplace_back("A", "Norton coefficients (optional)",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      opts.emplace_back("K", "Stress normalisation factors",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      opts.emplace_back("n", "Norton exponents",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      opts.emplace_back("Ksf",
                        "stress thresold factor. If the seq-R is greater "
                        "than Ksf*K, the newton step is rejected",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      return opts;
    }  // end of HarmonicSumOfNortonHoffViscoplasticFlows::getOptions

    void HarmonicSumOfNortonHoffViscoplasticFlows::initialize(
        BehaviourDescription& bd,
        AbstractBehaviourDSL& dsl,
        const std::string& id,
        const DataMap& d) {
      auto get_mps = [&dsl, &bd, &id, &d](const std::string& mpn) {
        if (d.count(mpn) == 0) {
          tfel::raise(
              "HarmonicSumOfNortonHoffViscoplasticFlows::initialize: "
              "material property '" +
              mpn + "' is not defined");
        }
        return getArrayOfBehaviourDescriptionMaterialProperties(dsl, mpn,
                                                                d.at(mpn));
      };
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // the base class
      ViscoplasticFlowBase::initialize(bd, dsl, id, d);
      //
      bd.appendToIncludes(
          "#include\"TFEL/Material/"
          "HarmonicSumOfNortonHoffViscoplasticFlows.hxx\"\n");
      // Norton flow options
      this->K = get_mps("K");
      this->n = get_mps("n");
      if (this->K.size() != this->n.size()) {
        tfel::raise(
            "HarmonicSumOfNortonHoffViscoplasticFlows::initialize: "
            "the sizes of the array of material properties 'K' and 'n' do not "
            "match");
      }
      declareParameterOrLocalVariable(bd, this->K, "stress", "K" + id);
      declareParameterOrLocalVariable(bd, this->n, "real", "E" + id);
      if (d.count("A") != 0) {
        this->A = get_mps("A");
        if (this->A.size() != this->K.size()) {
          tfel::raise(
              "HarmonicSumOfNortonHoffViscoplasticFlows::initialize: "
              "the sizes of the array of material properties 'K' and 'A' do "
              "not match");
        }
        declareParameterOrLocalVariable(bd, this->A, "strainrate", "A" + id);
      } else {
        BehaviourDescription::ConstantMaterialProperty cmp;
        cmp.value = 1;
        this->A = std::vector<BehaviourDescription::MaterialProperty>(
            this->K.size(), cmp);
        declareParameterOrLocalVariable(bd, this->A, "strainrate", "A" + id);
      }
      if (d.count("Ksf") != 0) {
        this->Ksf =
            getBehaviourDescriptionMaterialProperty(dsl, "Ksf", d.at("Ksf"));
        declareParameterOrLocalVariable(bd, this->Ksf, "real", "Ksf" + id);
      }
    }  // end of HarmonicSumOfNortonHoffViscoplasticFlows::initialize

    void HarmonicSumOfNortonHoffViscoplasticFlows::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const StressPotential& sp,
        const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      ViscoplasticFlowBase::endTreatment(bd, dsl, sp, id);
      CodeBlock i;
      i.code += generateMaterialPropertiesInitializationCode(dsl, bd, "A" + id,
                                                             this->A);
      i.code += generateMaterialPropertiesInitializationCode(dsl, bd, "K" + id,
                                                             this->K);
      i.code += generateMaterialPropertiesInitializationCode(dsl, bd, "E" + id,
                                                             this->n);
      if (!this->Ksf.empty()) {
        i.code += generateMaterialPropertyInitializationCode(
            dsl, bd, "Ksf" + id, this->Ksf);
      }
      if (!i.code.empty()) {
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of HarmonicSumOfNortonHoffViscoplasticFlows::endTreatment

    std::string HarmonicSumOfNortonHoffViscoplasticFlows::computeFlowRate(
        const std::string& id) const {
      const auto seq = [this, &id] {
        if (this->ihrs.empty()) {
          return "seq" + id;
        }
        return "seq" + id + "- R" + id;
      }();
      auto c = std::string{};
      //         if (!this->Ksf.empty()) {
      //           c += "if(" + seq + " > (this->Ksf" + id + ") * (this->K"
      //           + id + ")){\n";
      //           c += "return false;\n";
      //           c += "}\n";
      //         }
      c += "const auto vp" + id + " = ";
      c += "tfel::material::computeEquivalentStrainRateOf"
           "HarmonicSumOfNortonHoffViscoplasticFlows";
      c += "(this->A, this->K, this->E, " + seq + ");\n";
      return c;
    }  // end of HarmonicSumOfNortonHoffViscoplasticFlows::computeFlowRate

    std::string
    HarmonicSumOfNortonHoffViscoplasticFlows::computeFlowRateAndDerivative(
        const std::string& id) const {
      const auto seq = [this, &id] {
        if (this->ihrs.empty()) {
          return "seq" + id;
        }
        return "seq" + id + "- R" + id;
      }();
      auto c = std::string{};
      //       if (this->ihrs.empty()) {
      //         if (!this->Ksf.empty()) {
      //           c += "if(seq" + id + ">(this->Ksf" + id + ")*this->K" + id +
      //           "){\n";
      //           c += "return false;\n";
      //           c += "}\n";
      //         }
#if __cplusplus >= 201703L
      c += "const auto [vp" + id + ", dvp" + id + "_dseqe" + id + "] = ";
#else  /* __cplusplus >= 201703L */
      c += "auto vp" + id + " = strainrate{};\n";
      c += "auto dvp" + id + "_dseqe" + id +
           " = derivative_type<strainrate,stress>{};\n";
      c += "std::tie(vp" + id + ", dvp" + id + "_dseqe" + id + ") = ";
#endif /* __cplusplus >= 201703L */
      c += "tfel::material::computeEquivalentStrainRateAndDerivativeOf"
           "HarmonicSumOfNortonHoffViscoplasticFlows";
      c += "(this->A, this->K, this->E, " + seq + ");\n";
      return c;
    }  // end of
       // HarmonicSumOfNortonHoffViscoplasticFlows::computeFlowRateAndDerivative

    HarmonicSumOfNortonHoffViscoplasticFlows::
        ~HarmonicSumOfNortonHoffViscoplasticFlows() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
