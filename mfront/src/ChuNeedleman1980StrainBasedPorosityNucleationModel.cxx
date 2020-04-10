/*!
 * \file   mfront/src/ChuNeedleman1980StrainBasedPorosityNucleationModel.cxx
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
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/ChuNeedleman1980StrainBasedPorosityNucleationModel.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription>
    ChuNeedleman1980StrainBasedPorosityNucleationModel::getOptions() const {
      auto opts = PorosityNucleationModelBase::getOptions();
      opts.emplace_back("fn", "porosity rate coefficient",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("en", "mean value of the gaussian distribution",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("sn", "standard deviation of the gaussian distribution",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of PorosityNucleationModelBase::getOptions()

    void ChuNeedleman1980StrainBasedPorosityNucleationModel::initialize(
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
          "ChuNeedleman1980StrainBasedNucleationModel.hxx\"");
      //
      const auto parameters =
          PorosityNucleationModel::getVariableId("parameters", id);
      addLocalVariable(
          bd, "ChuNeedleman1980StrainBasedNucleationModelParameters<real>",
          parameters);
      //
      const auto fn_n = PorosityNucleationModel::getVariableId("fn", id);
      tfel::raise_if(
          d.count("fn") == 0,
          "ChuNeedleman1980StrainBasedPorosityNucleationModel::initialize: "
          "material property 'fn' is not defined");
      this->fn = getBehaviourDescriptionMaterialProperty(dsl, "fn", d.at("fn"));
      declareParameterOrLocalVariable(bd, this->fn, "real", fn_n);
      //
      const auto en_n = PorosityNucleationModel::getVariableId("en", id);
      tfel::raise_if(
          d.count("en") == 0,
          "ChuNeedleman1980StrainBasedPorosityNucleationModel::initialize: "
          "material property 'en' is not defined");
      this->en = getBehaviourDescriptionMaterialProperty(dsl, "en", d.at("en"));
      declareParameterOrLocalVariable(bd, this->en, "strain", en_n);
      //
      const auto sn_n = PorosityNucleationModel::getVariableId("sn", id);
      tfel::raise_if(
          d.count("sn") == 0,
          "ChuNeedleman1980StrainBasedPorosityNucleationModel::initialize: "
          "material property 'sn' is not defined");
      this->sn = getBehaviourDescriptionMaterialProperty(dsl, "sn", d.at("sn"));
      declareParameterOrLocalVariable(bd, this->sn, "strain", sn_n);
      // porosity factor
      const auto An_n = PorosityNucleationModel::getVariableId("An", id);
      bd.reserveName(uh, An_n);
      bd.reserveName(uh, "d" + An_n + "_dp");
    }  // end of ChuNeedleman1980StrainBasedPorosityNucleationModel::initialize

    void ChuNeedleman1980StrainBasedPorosityNucleationModel::
        completeVariableDeclaration(
            BehaviourDescription&,
            const AbstractBehaviourDSL&,
            const std::map<std::string,
                           std::shared_ptr<bbrick::InelasticFlow>>& iflows,
            const std::string&) const {
      if (iflows.empty()) {
        tfel::raise(
            "ChuNeedleman1980StrainBasedPorosityNucleationModel::"
            "completeVariableDeclaration: no inelastic flow declared");
      }
    }  // end of
       // ChuNeedleman1980StrainBasedPorosityNucleationModel::completeVariableDeclaration

    void ChuNeedleman1980StrainBasedPorosityNucleationModel::endTreatment(
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
            "ChuNeedleman1980StrainBasedPorosityNucleationModel::"
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
      const auto en_n = PorosityNucleationModel::getVariableId("en", id);
      const auto sn_n = PorosityNucleationModel::getVariableId("sn", id);
      const auto An_n = PorosityNucleationModel::getVariableId("An", id);
      const auto dAn_n_dp = "d" + An_n + "_dp";
      // sum of the equivalent plastic strains
      const auto p = [&iflows, &bd] {
        auto first = true;
        auto v = std::string{};
        for (const auto& flow : iflows) {
          if (!first) {
            v += " + ";
          }
          v += "this->p" + flow.first + " + ";
          v += "(" + bd.getClassName() + "::theta) * ";
          v += "(this->dp" + flow.first + ")";
          first = false;
        }
        return v;
      }();
      // sum of the increments of the equivalent plastic strains
      const auto dp = [&iflows, &bd] {
        auto first = true;
        auto v = std::string{};
        for (const auto& flow : iflows) {
          if (!first) {
            v += " + ";
          }
          v += "this->dp" + flow.first;
          first = false;
        }
        return v;
      }();
      //
      auto c = std::string{};
      c += generateMaterialPropertyInitializationCode(dsl, bd, fn_n, this->fn);
      c += generateMaterialPropertyInitializationCode(dsl, bd, en_n, this->en);
      c += generateMaterialPropertyInitializationCode(dsl, bd, sn_n, this->sn);
      c += "this->" + parameters + ".fn = this->" + fn_n + ";\n";
      c += "this->" + parameters + ".en = this->" + en_n + ";\n";
      c += "this->" + parameters + ".sn = this->" + sn_n + ";\n";
      CodeBlock init;
      init.code = c;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      // porosity evolution
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      CodeBlock i;
      const auto call =
          "computeChuNeedleman1980StrainBasedNucleationModelPorosityRateFactor"
          "(" +
          p + ", this->" + parameters + ");\n";
      i.code += "#if __cplusplus >= 201703L\n";
      i.code += "const auto [" + An_n + ", " + dAn_n_dp + "]  = ";
      i.code += call + ";\n";
      if (!requiresAnalyticalJacobian) {
        i.code += "static_cast<void>(" + dAn_n_dp + ");\n";
      }
      i.code += "#else  /* __cplusplus >= 201703L */\n";
      i.code += "auto " + An_n + " = real{};\n";
      if (requiresAnalyticalJacobian) {
        i.code += "auto " + dAn_n_dp + " = real{};\n";
        i.code += "std::tie(" + An_n + ", " + dAn_n_dp + ")  = ";
      } else {
        i.code += "std::tie(this->dfn" + id + ", std::ignore)  = ";
      }
      i.code += call + ";\n";
      i.code += "#endif /* __cplusplus >= 201703L */\n";
      i.code += "this->dfn" + id + " = " + An_n + " * (" + dp + ");\n";
      i.code += "f" + f.name + " -= this->dfn" + id + ";\n";
      if (requiresAnalyticalJacobian) {
        for (const auto& flow : iflows) {
          i.code += "df" + f.name + "_ddp" + flow.first + " -= ";
          i.code += "(" + bd.getClassName() + "::theta) * ";
          i.code += "(" + dAn_n_dp + ") * (" + dp + ") + ";
          i.code += An_n + ";\n";
        }
      }
      bd.setCode(uh, BehaviourData::Integrator, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }  // end of
    // ChuNeedleman1980StrainBasedPorosityNucleationModel::endTreatment

    ChuNeedleman1980StrainBasedPorosityNucleationModel::
        ~ChuNeedleman1980StrainBasedPorosityNucleationModel() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront

