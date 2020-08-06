/*!
 * \file   mfront/src/ChuNeedleman1980StressBasedPorosityNucleationModel.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   05/04/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
#include "MFront/BehaviourBrick/ChuNeedleman1980StressBasedPorosityNucleationModel.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription>
    ChuNeedleman1980StressBasedPorosityNucleationModel::getOptions() const {
      auto opts = PorosityNucleationModelBase::getOptions();
      opts.emplace_back("fn", "porosity rate coefficient",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("sigm", "mean value of the gaussian distribution",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("sn", "standard deviation of the gaussian distribution",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("fmax", "maximum value of the nucleated porosity",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of PorosityNucleationModelBase::getOptions()

    bool ChuNeedleman1980StressBasedPorosityNucleationModel::
        requiresSavingNucleatedPorosity() const {
      return true;
    }  // end of
       // ChuNeedleman1980StressBasedPorosityNucleationModel::requiresSavingNucleatedPorosity()

    void ChuNeedleman1980StressBasedPorosityNucleationModel::initialize(
        BehaviourDescription& bd,
        AbstractBehaviourDSL& dsl,
        const std::string& id,
        const DataMap& d) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      //
      PorosityNucleationModelBase::initialize(bd, dsl, id, d);
      //
      bd.appendToIncludes(
          "#include \"TFEL/Material/"
          "ChuNeedleman1980StressBasedNucleationModel.hxx\"");
      //
      const auto parameters =
          PorosityNucleationModel::getVariableId("parameters", id);
      addLocalVariable(
          bd, "ChuNeedleman1980StressBasedNucleationModelParameters<StressStensor>",
          parameters);
      //
      const auto fn_n = PorosityNucleationModel::getVariableId("fn", id);
      tfel::raise_if(
          d.count("fn") == 0,
          "ChuNeedleman1980StressBasedPorosityNucleationModel::initialize: "
          "material property 'fn' is not defined");
      this->fn = getBehaviourDescriptionMaterialProperty(dsl, "fn", d.at("fn"));
      declareParameterOrLocalVariable(bd, this->fn, "real", fn_n);
      //
      const auto sigm_n = PorosityNucleationModel::getVariableId("sigm", id);
      tfel::raise_if(
          d.count("sigm") == 0,
          "ChuNeedleman1980StressBasedPorosityNucleationModel::initialize: "
          "material property 'sigm' is not defined");
      this->sigm = getBehaviourDescriptionMaterialProperty(dsl, "sigm", d.at("sigm"));
      declareParameterOrLocalVariable(bd, this->sigm, "stress", sigm_n);
      //
      const auto sn_n = PorosityNucleationModel::getVariableId("sn", id);
      tfel::raise_if(
          d.count("sn") == 0,
          "ChuNeedleman1980StressBasedPorosityNucleationModel::initialize: "
          "material property 'sn' is not defined");
      this->sn = getBehaviourDescriptionMaterialProperty(dsl, "sn", d.at("sn"));
      declareParameterOrLocalVariable(bd, this->sn, "strain", sn_n);
      //
      const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
      tfel::raise_if(
          d.count("fmax") == 0,
          "ChuNeedleman1980StressBasedPorosityNucleationModel::initialize: "
          "material property 'fmax' is not defined");
      this->fmax = getBehaviourDescriptionMaterialProperty(dsl, "fmax", d.at("fmax"));
      declareParameterOrLocalVariable(bd, this->fmax, "real", fmax_n);
      // porosity factor
      const auto An_n = PorosityNucleationModel::getVariableId("An", id);
      bd.reserveName(uh, An_n);
      bd.reserveName(uh, "d" + An_n + "_dsig");
    }  // end of ChuNeedleman1980StressBasedPorosityNucleationModel::initialize

    void ChuNeedleman1980StressBasedPorosityNucleationModel::
        completeVariableDeclaration(
            BehaviourDescription&,
            const AbstractBehaviourDSL&,
            const std::map<std::string,
                           std::shared_ptr<bbrick::InelasticFlow>>& iflows,
            const std::string&) const {
      if (iflows.empty()) {
        tfel::raise(
            "ChuNeedleman1980StressBasedPorosityNucleationModel::"
            "completeVariableDeclaration: no inelastic flow declared");
      }
    }  // end of
       // ChuNeedleman1980StressBasedPorosityNucleationModel::completeVariableDeclaration

    void ChuNeedleman1980StressBasedPorosityNucleationModel::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const StressPotential& sp,
        const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&
            iflows,
        const std::string& id) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      PorosityNucleationModelBase::endTreatment(bd, dsl, sp, iflows, id);
      //
      if (iflows.empty()) {
        tfel::raise(
            "ChuNeedleman1980StressBasedPorosityNucleationModel::"
            "completeVariableDeclaration: no inelastic flow declared");
      }
      //
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      const auto requiresAnalyticalJacobian =
          ((idsl.getSolver().usesJacobian()) &&
           (!idsl.getSolver().requiresNumericalJacobian()));
      // variable initialisation
      const auto parameters =
          PorosityNucleationModel::getVariableId("parameters", id);
      const auto fn_n = PorosityNucleationModel::getVariableId("fn", id);
      const auto sigm_n = PorosityNucleationModel::getVariableId("sigm", id);
      const auto sn_n = PorosityNucleationModel::getVariableId("sn", id);
      const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
      const auto An_n = PorosityNucleationModel::getVariableId("An", id);
      const auto dAn_n_dsig = "d" + An_n + "_dsig";
      //
      auto c = std::string{};
      c += generateMaterialPropertyInitializationCode(dsl, bd, fn_n, this->fn);
      c += generateMaterialPropertyInitializationCode(dsl, bd, sigm_n, this->sigm);
      c += generateMaterialPropertyInitializationCode(dsl, bd, sn_n, this->sn);
      c += generateMaterialPropertyInitializationCode(dsl, bd, fmax_n, this->fmax);
      c += "this->" + parameters + ".fn = this->" + fn_n + ";\n";
      c += "this->" + parameters + ".sigm = this->" + sigm_n + ";\n";
      c += "this->" + parameters + ".sn = this->" + sn_n + ";\n";
      CodeBlock init;
      init.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      // porosity evolution
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      // sum of the increments of the equivalent plastic strains
      const auto dp = [&iflows, &bd] {
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
      CodeBlock i;
      const auto call =
          "computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactorAndDerivative"
          "<StressStensor>(this->sig, this->" +
          parameters + ")";
      if (this->porosity_evolution_algorithm ==
          PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        i.code += "if(";
        i.code += StandardElastoViscoPlasticityBrick::
            computeStandardSystemOfImplicitEquations;
        i.code += "){\n";
      }
      i.code += "#if __cplusplus >= 201703L\n";
      i.code += "const auto [" + An_n + ", " + dAn_n_dsig + "]  = ";
      i.code += call + ";\n";
      if (!requiresAnalyticalJacobian) {
        i.code += "static_cast<void>(" + dAn_n_dsig + ");\n";
      }
      i.code += "#else  /* __cplusplus >= 201703L */\n";
      i.code += "auto " + An_n + " = real{};\n";
      if (requiresAnalyticalJacobian) {
        i.code += "auto " + dAn_n_dsig + " = Stensor{};\n";
        i.code += "std::tie(" + An_n + ", " + dAn_n_dsig + ")  = ";
      } else {
        i.code += "std::tie(this->dfn" + id + ", std::ignore)  = ";
      }
      i.code += call + ";\n";
      i.code += "#endif /* __cplusplus >= 201703L */\n";
      i.code += "this->dfn" + id + " = " + An_n + " * (" + dp + ");\n";
      i.code +=
          "if(this->fn" + id + " + this->dfn" + id + " >= this->" + fmax_n + "){\n";
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
                "(" + dp + ") * (" + dAn_n_dsig + " | " + dsig_ddv + ");\n";
          } else {
            tfel::raise(
                "ChuNeedleman1980StressBasedPorosityNucleationModel::"
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
      bd.setCode(uh, BehaviourData::Integrator, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }  // end of
    // ChuNeedleman1980StressBasedPorosityNucleationModel::endTreatment

    std::string ChuNeedleman1980StressBasedPorosityNucleationModel::
        updateNextEstimateOfThePorosityIncrement(
            const BehaviourDescription& bd,
            const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&
                iflows,
            const std::string& id) const {
      const auto parameters =
          PorosityNucleationModel::getVariableId("parameters", id);
      const auto An_n = PorosityNucleationModel::getVariableId("An", id);
      const auto fmax_n = PorosityNucleationModel::getVariableId("fmax", id);
      // sum of the increments of the equivalent plastic strains
      const auto dp = [&iflows, &bd] {
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
      const auto call =
          "computeChuNeedleman1980StressBasedNucleationModelPorosityRateFactor"
          "<StressStensor>(this->sig , this->" +
          parameters + ")";
      //
      auto c = std::string{};
      c += "const auto " + An_n + " = ";
      c += call + ";\n";
      c += "this->dfn" + id + " = std::min(" + An_n + " * (" + dp + "), this->" +
           fmax_n + " - this->fn);\n";
      c += mfront::StandardElastoViscoPlasticityBrick::
          nextEstimateOfThePorosityIncrement;
      c += " += this->dfn" + id + ";\n";
      return c;
    }  // end of
       // ChuNeedleman1980StressBasedPorosityNucleationModel::updateNextEstimateOfThePorosityIncrement

    ChuNeedleman1980StressBasedPorosityNucleationModel::
        ~ChuNeedleman1980StressBasedPorosityNucleationModel() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront

