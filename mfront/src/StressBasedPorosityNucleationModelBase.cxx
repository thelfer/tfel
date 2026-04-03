/*!
 * \file   mfront/src/StressBasedPorosityNucleationModelBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   05/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressBasedPorosityNucleationModelBase.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription>
  StressBasedPorosityNucleationModelBase::getOptions() const {
    auto opts = PorosityNucleationModelBase::getOptions();
    opts.emplace_back("fmax", "maximum value of the nucleated porosity",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  bool StressBasedPorosityNucleationModelBase::requiresSavingNucleatedPorosity()
      const {
    return true;
  }  // end of requiresSavingNucleatedPorosity

  void StressBasedPorosityNucleationModelBase::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& id,
      const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    PorosityNucleationModelBase::initialize(bd, dsl, id, d);
    const auto mn = this->getModelName();
    bd.appendToIncludes("#include \"TFEL/Material/" + mn + ".hxx\"");
    //
    const auto parameters =
        PorosityNucleationModel::getVariableId("parameters", id);
    addLocalVariable(bd, "" + mn + "Parameters<StressStensor>", parameters);
    //
    const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
    tfel::raise_if(
        d.count("fmax") == 0,
        "ChuNeedleman1980StressBasedPorosityNucleationModel::initialize: "
        "material property 'fmax' is not defined");
    this->fmax =
        getBehaviourDescriptionMaterialProperty(dsl, "fmax", d.at("fmax"));
    declareParameterOrLocalVariable(bd, this->fmax, "real", fmax_n);
    // porosity factor
    const auto An_n = PorosityNucleationModel::getVariableId("An", id);
    bd.reserveName(uh, An_n);
    bd.reserveName(uh, "d" + An_n + "_dsig");
  }  // end of initialize

  void StressBasedPorosityNucleationModelBase::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const StressPotential& sp,
      const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&
          iflows,
      const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    PorosityNucleationModelBase::endTreatment(bd, dsl, sp, iflows, id);
    const auto mn = this->getModelName();
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    const auto requiresAnalyticalJacobian =
        ((idsl.getSolver().usesJacobian()) &&
         (!idsl.getSolver().requiresNumericalJacobian()));
    // variable initialisation
    const auto parameters =
        PorosityNucleationModel::getVariableId("parameters", id);
    const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
    CodeBlock init;
    for (const auto& mc : this->getMaterialCoefficientDescriptions()) {
      const auto mc_n = PorosityNucleationModel::getVariableId(mc.name, id);
      init.code +=
          "this->" + parameters + "." + mc.name + " = this->" + mc_n + ";\n";
    }
    init.code +=
        generateMaterialPropertyInitializationCode(dsl, bd, fmax_n, this->fmax);
    bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    // porosity evolution
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    // total plastic strain
    const auto p = [&iflows, &bd] {
      const auto theta = bd.getClassName() + "::theta";
      auto first = true;
      auto v = std::string{};
      for (const auto& flow : iflows) {
        if (!first) {
          v += " + ";
        }
        v += "this->p" + flow.first + " + ";
        v += theta + " * std::max(this->dp" + flow.first + ", real(0))";
        first = false;
      }
      return v;
    }();
    // sum of the increments of the equivalent plastic strains
    const auto dp = [&iflows] {
      auto first = true;
      auto v = std::string{};
      for (const auto& flow : iflows) {
        if (!first) {
          v += " + ";
        }
        v += "std::max(this->dp" + flow.first + ", real(0))";
        first = false;
      }
      return v;
    }();
    // implicit equations
    const auto An_n = PorosityNucleationModel::getVariableId("An", id);
    const auto dAn_n_dsig = "d" + An_n + "_dsig";
    CodeBlock i;
    const auto call = "compute" + mn +
                      "PorosityRateFactorAndDerivative<StressStensor>" +  //
                      "(this->" + parameters + ", this->sig, " + p + ")";
    if (this->porosity_evolution_algorithm ==
        PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
      i.code += "if(";
      i.code += StandardElastoViscoPlasticityBrick::
          computeStandardSystemOfImplicitEquations;
      i.code += "){\n";
    }
    i.code += "const auto [" + An_n + ", " + dAn_n_dsig + "]  = ";
    i.code += call + ";\n";
    if (!requiresAnalyticalJacobian) {
      i.code += "static_cast<void>(" + dAn_n_dsig + ");\n";
    }
    i.code += "this->dfn" + id + " = " + An_n + " * (" + dp + ");\n";
    i.code += "if(this->fn" + id + " + this->dfn" + id + " >= this->" + fmax_n +
              "){\n";
    i.code += "this->dfn" + id + " = this->" + fmax_n + " - this->fn;\n";
    i.code += "f" + f.name + " -= this->dfn" + id + ";\n";
    i.code += "} else {\n";
    i.code += "f" + f.name + " -= this->dfn" + id + ";\n";
    if (requiresAnalyticalJacobian) {
      for (const auto& flow : iflows) {
        i.code += "if(this->dp" + flow.first + " > real(0)){\n";
        i.code += "df" + f.name + "_ddp" + flow.first + " -= " + An_n + ";\n";
        i.code += "}\n";
      }
      for (const auto& dsig : sp.getStressDerivatives(bd)) {
        const auto& dsig_ddv = std::get<0>(dsig);
        const auto& v = std::get<1>(dsig);
        const auto& t = std::get<2>(dsig);
        if ((t == SupportedTypes::SCALAR) || (t == SupportedTypes::STENSOR)) {
          i.code += "df" + f.name + "_dd" + v + " -= ";
          i.code +=
              "(" + dp + ") * (" + dAn_n_dsig + " | (" + dsig_ddv + "));\n";
        } else {
          tfel::raise(
              "StressBasedPorosityNucleationModelBase::"
              "endTreatment: stress dependency on variable '" +
              v + "' is not unsupported ");
        }
      }
    }
    i.code += "}\n";
    if (this->porosity_evolution_algorithm ==
        PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
      i.code += "}\n";
    }
    bd.setCode(uh, BehaviourData::Integrator, i, BehaviourData::CREATEORAPPEND,
               BehaviourData::AT_BEGINNING);
  }  // end of
  // StressBasedPorosityNucleationModelBase::endTreatment

  std::string StressBasedPorosityNucleationModelBase::
      updateNextEstimateOfThePorosityIncrement(
          const BehaviourDescription& bd,
          const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&
              iflows,
          const std::string& id) const {
    const auto mn = this->getModelName();
    const auto parameters =
        PorosityNucleationModel::getVariableId("parameters", id);
    const auto An_n = PorosityNucleationModel::getVariableId("An", id);
    const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
    // total plastic strain
    const auto p = [&iflows, &bd] {
      const auto theta = bd.getClassName() + "::theta";
      auto first = true;
      auto v = std::string{};
      for (const auto& flow : iflows) {
        if (!first) {
          v += " + ";
        }
        v += "this->p" + flow.first + " + ";
        v += theta + " * std::max(this->dp" + flow.first + ", real(0))";
        first = false;
      }
      return v;
    }();
    // sum of the increments of the equivalent plastic strains
    const auto dp = [&iflows] {
      auto first = true;
      auto v = std::string{};
      for (const auto& flow : iflows) {
        if (!first) {
          v += " + ";
        }
        v += "std::max(this->dp" + flow.first + ", real(0))";
        first = false;
      }
      return v;
    }();
    //
    auto c = std::string{};
    c += "const auto " + An_n + " = ";
    c += "compute" + mn + "PorosityRateFactor<StressStensor>" +  //
         "(this->" + parameters + ", this->sig, " + p + ");\n";
    c += "this->dfn" + id + " = std::min(" + An_n + " * (" + dp + "), this->" +
         fmax_n + " - this->fn);\n";
    c += mfront::StandardElastoViscoPlasticityBrick::
        nextEstimateOfThePorosityIncrement;
    c += " += this->dfn" + id + ";\n";
    return c;
  }  // end of updateNextEstimateOfThePorosityIncrement

  StressBasedPorosityNucleationModelBase::
      ~StressBasedPorosityNucleationModelBase() = default;

}  // end of namespace mfront::bbrick
