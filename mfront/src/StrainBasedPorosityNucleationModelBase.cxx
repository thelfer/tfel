/*!
 * \file   mfront/src/StrainBasedPorosityNucleationModelBase.cxx
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
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StrainBasedPorosityNucleationModelBase.hxx"

namespace mfront::bbrick {

  bool StrainBasedPorosityNucleationModelBase::requiresSavingNucleatedPorosity()
      const {
    return this->requiresLimitOnNucleationPorosity();
  }  // end of
     // StrainBasedPorosityNucleationModelBase::requiresSavingNucleatedPorosity()

  std::vector<OptionDescription>
  StrainBasedPorosityNucleationModelBase::getOptions() const {
    auto opts = PorosityNucleationModelBase::getOptions();
    if (this->requiresLimitOnNucleationPorosity()) {
      opts.emplace_back("fmax", "maximum value of the nucleated porosity",
                        OptionDescription::MATERIALPROPERTY);
    }
    return opts;
  }  // end of StrainBasedPorosityNucleationModelBase::getOptions()

  void StrainBasedPorosityNucleationModelBase::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& id,
      const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto mn = this->getModelName();
    PorosityNucleationModelBase::initialize(bd, dsl, id, d);
    bd.appendToIncludes("#include \"TFEL/Material/" + mn + ".hxx\"");
    const auto parameters =
        PorosityNucleationModel::getVariableId("parameters", id);
    addLocalVariable(bd, mn + "Parameters<real>", parameters);
    if (this->requiresLimitOnNucleationPorosity()) {
      const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
      tfel::raise_if(
          d.count("fmax") == 0,
          "ChuNeedleman1980StressBasedPorosityNucleationModel::initialize: "
          "material property 'fmax' is not defined");
      this->fmax =
          getBehaviourDescriptionMaterialProperty(dsl, "fmax", d.at("fmax"));
      declareParameterOrLocalVariable(bd, this->fmax, "real", fmax_n);
    }
    // porosity factor
    const auto fn_n = "fn" + id;
    bd.reserveName(uh, "d" + fn_n + "_ddp");
  }  // end of StrainBasedPorosityNucleationModelBase::initialize

  void StrainBasedPorosityNucleationModelBase::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const StressPotential& sp,
      const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&
          iflows,
      const std::string& id) const {
    PorosityNucleationModelBase::endTreatment(bd, dsl, sp, iflows, id);
    //
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto mn = this->getModelName();
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    const auto requiresAnalyticalJacobian =
        ((idsl.getSolver().usesJacobian()) &&
         (!idsl.getSolver().requiresNumericalJacobian()));
    // material coefficients initialisation
    const auto parameters =
        PorosityNucleationModel::getVariableId("parameters", id);
    CodeBlock init;
    for (const auto& mc : this->getMaterialCoefficientDescriptions()) {
      const auto mc_n = PorosityNucleationModel::getVariableId(mc.name, id);
      init.code +=
          "this->" + parameters + "." + mc.name + " = this->" + mc_n + ";\n";
    }
    if (this->requiresLimitOnNucleationPorosity()) {
      const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
      init.code += generateMaterialPropertyInitializationCode(dsl, bd, fmax_n,
                                                              this->fmax);
    }
    bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    //
    const auto dfn_n = PorosityNucleationModel::getVariableId("dfn", id);
    const auto dfn_ddp = PorosityNucleationModel::getVariableId("dfn_ddp", id);
    // sum of the equivalent plastic strains
    const auto p = [&iflows] {
      auto first = true;
      auto v = std::string{};
      for (const auto& flow : iflows) {
        if (!first) {
          v += " + ";
        }
        v += "this->p" + flow.first;
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
    // porosity evolution
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    CodeBlock i;
    if (this->porosity_evolution_algorithm ==
        PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
      i.code += "if(";
      i.code += StandardElastoViscoPlasticityBrick::
          computeStandardSystemOfImplicitEquations;
      i.code += "){\n";
    }
    auto call = "compute" + mn + "PorosityIncrementAndDerivative" +  //
                "(this->" + parameters + "," + p + ", " + dp + ", " +
                bd.getClassName() + "::theta)";
    i.code += "const auto [" + dfn_n + ", " + dfn_ddp + "]  = ";
    i.code += call + ";\n";
    if (!requiresAnalyticalJacobian) {
      i.code += "static_cast<void>(" + dfn_ddp + ");\n";
    }
    i.code += "this->dfn" + id + " = " + dfn_n + ";\n";
    if (this->requiresLimitOnNucleationPorosity()) {
      const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
      i.code += "if(this->fn" + id + " + this->dfn" + id + " >= this->" +
                fmax_n + "){\n";
      i.code += "this->dfn" + id + " = this->" + fmax_n + " - this->fn;\n";
      i.code += "f" + f.name + " -= this->dfn" + id + ";\n";
      i.code += "} else {\n";
    }
    i.code += "f" + f.name + " -= this->dfn" + id + ";\n";
    if (requiresAnalyticalJacobian) {
      for (const auto& flow : iflows) {
        i.code += "if(this->dp" + flow.first + " > real(0)){\n";
        i.code += "df" + f.name + "_ddp" + flow.first +  //
                  " -= " + dfn_ddp + ";\n";
        i.code += "}\n";
      }
    }
    if (this->requiresLimitOnNucleationPorosity()) {
      i.code += "}\n";
    }
    if (this->porosity_evolution_algorithm ==
        PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
      i.code += "}\n";
    }
    bd.setCode(uh, BehaviourData::Integrator, i, BehaviourData::CREATEORAPPEND,
               BehaviourData::AT_BEGINNING);
  }  // end of
  // StrainBasedPorosityNucleationModelBase::endTreatment

  std::string StrainBasedPorosityNucleationModelBase::
      updateNextEstimateOfThePorosityIncrement(
          const BehaviourDescription& bd,
          const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&
              iflows,
          const std::string& id) const {
    const auto mn = this->getModelName();
    const auto parameters =
        PorosityNucleationModel::getVariableId("parameters", id);
    // sum of the equivalent plastic strains
    const auto p = [&iflows] {
      auto first = true;
      auto v = std::string{};
      for (const auto& flow : iflows) {
        if (!first) {
          v += " + ";
        }
        v += "this->p" + flow.first;
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
    if (this->requiresLimitOnNucleationPorosity()) {
      const auto dfn_n = PorosityNucleationModel::getVariableId("dfn", id);
      const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
      c += "const auto " + dfn_n + " = ";
      c += "compute" + mn + "PorosityIncrement" +  //
           "(this->" + parameters + "," + p + "," + dp + ", " +
           bd.getClassName() + "::theta);\n";
      c += "this->dfn" + id + " = ";
      c += "std::min(" + dfn_n + ", this->" + fmax_n + " - this->fn);\n";
    } else {
      c += "this->dfn" + id + " = compute" + mn + "PorosityIncrement" +  //
           "(this->" + parameters + "," + p + "," + dp + ", " +
           bd.getClassName() + "::theta);\n";
    }
    c += mfront::StandardElastoViscoPlasticityBrick::
        nextEstimateOfThePorosityIncrement;
    c += " += this->dfn" + id + ";\n";
    return c;
  }  // end of
     // StrainBasedPorosityNucleationModelBase::updateNextEstimateOfThePorosityIncrement

  StrainBasedPorosityNucleationModelBase::
      ~StrainBasedPorosityNucleationModelBase() = default;

}  // end of namespace mfront::bbrick
