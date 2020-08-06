/*!
 * \file   BrickUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#include <utility>
#include <sstream>
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"

namespace mfront {

  namespace bbrick {

    std::function<
        std::string(const BehaviourDescription::MaterialPropertyInput&)>
    getMiddleOfTimeStepModifier(const BehaviourDescription& bd) {
      using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
      return [&bd](const MaterialPropertyInput& i) -> std::string {
        if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
            (i.category ==
             MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
            (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
          return "this->" + i.name + "+(this->theta)*(this->d" + i.name + ')';
        } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                   (i.category == MaterialPropertyInput::PARAMETER)) {
          return "this->" + i.name;
        } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
          return bd.getClassName() + "::" + i.name;
        } else {
          tfel::raise("unsupported input type for variable '" + i.name + "'");
        }
      };
    }  // end of getMiddleOfTimeStepModifier

    std::string generateMaterialPropertyInitializationCode(
        const AbstractBehaviourDSL& dsl,
        const BehaviourDescription& bd,
        const std::string& n,
        const BehaviourDescription::MaterialProperty& mp) {
      auto c = std::string{};
      if (!mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
        auto mts = getMiddleOfTimeStepModifier(bd);
        std::ostringstream mps;
        mps << "this->" + n + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, mp, mts);
        mps << ";\n";
        c = mps.str();
      }
      return c;
    }  // end of generateMaterialPropertyInitializationCode

    BehaviourDescription::MaterialProperty
    getBehaviourDescriptionMaterialProperty(AbstractBehaviourDSL& dsl,
                                            const std::string& n,
                                            const tfel::utilities::Data& d) {
      if (d.is<double>()) {
        BehaviourDescription::ConstantMaterialProperty cmp;
        cmp.value = d.get<double>();
        return std::move(cmp);
      } else if (d.is<int>()) {
        BehaviourDescription::ConstantMaterialProperty cmp;
        cmp.value = static_cast<double>(d.get<int>());
        return std::move(cmp);
      }
      if (!d.is<std::string>()) {
        tfel::raise(
            "getBehaviourDescriptionMaterialProperty: "
            "unsupported data type for material property '" +
            n + "'");
      }
      const auto mp = d.get<std::string>();
      if (tfel::utilities::ends_with(mp, ".mfront")) {
        // file name
        BehaviourDescription::ExternalMFrontMaterialProperty emp;
        emp.mpd = dsl.handleMaterialPropertyDescription(mp);
        return std::move(emp);
      }
      BehaviourDescription::AnalyticMaterialProperty amp;
      amp.f = mp;
      return std::move(amp);
    }  // end of getBehaviourDescriptionMaterialProperty_impl

    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        BehaviourDescription::MaterialProperty& mp,
        const std::string& t,
        const std::string& n) {
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
        auto& cmp = mp.get<BehaviourDescription::ConstantMaterialProperty>();
        cmp.name = n;
        // declare associated parameter
        VariableDescription m(t, n, 1u, 0u);
        bd.addParameter(h, m);
        bd.setParameterDefaultValue(h, n, cmp.value);
      } else {
        VariableDescription m(t, n, 1u, 0u);
        bd.addLocalVariable(h, m);
      }
    } // end of declareParameterOrLocalVariable

    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        BehaviourDescription::MaterialProperty& mp,
        const std::string& t,
        const std::string& n,
        const std::string& en) {
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      declareParameterOrLocalVariable(bd, mp, t, n);
      if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
        bd.setEntryName(h,n,en);
      }
    } // end of declareParameterOrLocalVariable

    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        BehaviourDescription::MaterialProperty& mp,
        const std::string& t,
        const std::string& n,
        const tfel::glossary::GlossaryEntry& g) {
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      declareParameterOrLocalVariable(bd, mp, t, n);
      if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
        bd.setGlossaryName(h, n, g);
      }
    } // end of declareParameterOrLocalVariable

    void checkOptionsNames(
        const std::map<std::string, tfel::utilities::Data>& d,
        const std::vector<std::string>& k,
        const std::string& n) {
      for (const auto& de : d) {
        tfel::raise_if(std::find(k.begin(), k.end(), de.first) == k.end(),
                       "checkOptionsNames: '" + n +
                           "' does not expect option '" + de.first + "'");
      }
    }  // end of checkOptionsNames

    void addMaterialPropertyIfNotDefined(BehaviourDescription& bd,
                                         const std::string& t,
                                         const std::string& n,
                                         const tfel::glossary::GlossaryEntry& g,
                                         const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b, "addMaterialPropertyIfNotDefined: " + m);
      };
      // treating material properties
      const auto b = bd.checkVariableExistence(n);
      if (b.first) {
        throw_if(!b.second, "variable '" + n +
                                "' is not declared for all specialisations "
                                "of the behaviour");
        auto r = bd.checkVariableExistence(n, "Parameter", false);
        if (r.first) {
          throw_if(!r.second, "parameter '" + n +
                                  "' is not declared for all specialisations "
                                  "of the behaviour");
        } else {
          r = bd.checkVariableExistence(n, "MaterialProperty", false);
          throw_if(!r.first, "variable '" + n +
                                 "' is neither declared as a parameter nor "
                                 "a material property");
          throw_if(!r.second, "material property '" + n +
                                  "' is not declared"
                                  "for all specialisations of the behaviour");
        }
        bd.checkVariableGlossaryName(n, g);
      } else {
        const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        bd.addMaterialProperty(h, {t, n, s, 0u});
        bd.setGlossaryName(h, n, g);
      }
    }  // end of addMaterialPropertyIfNotDefined

    void addMaterialPropertyIfNotDefined(BehaviourDescription& bd,
                                         const std::string& t,
                                         const std::string& n,
                                         const std::string& e,
                                         const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b, "addMaterialPropertyIfNotDefined: " + m);
      };
      // treating material properties
      const auto b = bd.checkVariableExistence(n);
      if (b.first) {
        throw_if(!b.second, "variable '" + n +
                                "' is not declared for all specialisations "
                                "of the behaviour");
        auto r = bd.checkVariableExistence(n, "Parameter", false);
        if (r.first) {
          throw_if(!r.second, "parameter '" + n +
                                  "' is not declared for all specialisations "
                                  "of the behaviour");
        } else {
          r = bd.checkVariableExistence(n, "MaterialProperty", false);
          throw_if(!r.first, "variable '" + n +
                                 "' is neither declared as a parameter nor "
                                 "a material property");
          throw_if(!r.second, "material property '" + n +
                                  "' is not declared"
                                  "for all specialisations of the behaviour");
        }
        bd.checkVariableEntryName(n, e);
      } else {
        const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        bd.addMaterialProperty(h, {t, n, s, 0u});
        bd.setEntryName(h, n, e);
      }
    }  // end of addMaterialPropertyIfNotDefined

    void addStateVariable(BehaviourDescription& bd,
                          const std::string& t,
                          const std::string& n,
                          const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v(t, n, s, 0u);
      bd.addStateVariable(h, v);
    }  // end of addStateVariable

    void addStateVariable(BehaviourDescription& bd,
                          const std::string& t,
                          const std::string& n,
                          const tfel::glossary::GlossaryEntry& g,
                          const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v(t, n, s, 0u);
      bd.addStateVariable(h, v);
      bd.setGlossaryName(h, n, g);
    }  // end of addStateVariable

    void addStateVariable(BehaviourDescription& bd,
                          const std::string& t,
                          const std::string& n,
                          const std::string& e,
                          const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v(t, n, s, 0u);
      bd.addStateVariable(h, v);
      bd.setEntryName(h, n, e);
    }  // end of addStateVariable

    void addStateVariableIfNotDefined(BehaviourDescription& bd,
                                      const std::string& t,
                                      const std::string& n,
                                      const tfel::glossary::GlossaryEntry& g,
                                      const unsigned short s,
                                      const bool bo) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto b = [bd, &n, &g, &bo] {
        for (const auto& v : bd.getBehaviourData(uh).getStateVariables()) {
          if (v.getExternalName() == g) {
            if ((!bo) && (v.name != n)) {
              tfel::raise(
                  "addStateVariableIfNotDefined: a state variable with the "
                  "given glossary name already has already been registred");
            }
            return true;
          }
        }
        return false;
      }();
      if (!b) {
        auto v = mfront::VariableDescription(t, n, s, 0u);
        v.setGlossaryName(g);
        bd.addStateVariable(uh, v, mfront::BehaviourData::UNREGISTRED);
      }
    }  // end of addStateVariableIfNotDefined

    void addStateVariableIfNotDefined(BehaviourDescription& bd,
                                      const std::string& t,
                                      const std::string& n,
                                      const std::string& e,
                                      const unsigned short s,
                                      const bool bo) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto b = [bd, &n, &e, &bo] {
        for (const auto& v : bd.getBehaviourData(uh).getStateVariables()) {
          if (v.getExternalName() == e) {
            if ((!bo) && (v.name != n)) {
              tfel::raise(
                  "addStateVariableIfNotDefined: a state variable with the "
                  "given entry name already has already been registred");
            }
            return true;
          }
        }
        return false;
      }();
      if (!b) {
        auto v = mfront::VariableDescription(t, n, s, 0u);
        v.setEntryName(e);
        bd.addStateVariable(uh, v, mfront::BehaviourData::UNREGISTRED);
      }
    }

    void addAuxiliaryStateVariableIfNotDefined(BehaviourDescription& bd,
                                      const std::string& t,
                                      const std::string& n,
                                      const tfel::glossary::GlossaryEntry& g,
                                      const unsigned short s,
                                      const bool bo) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto b = [bd, &n, &g, &bo] {
        for (const auto& v : bd.getBehaviourData(uh).getAuxiliaryStateVariables()) {
          if (v.getExternalName() == g) {
            if ((!bo) && (v.name != n)) {
              tfel::raise(
                  "addAuxiliaryStateVariableIfNotDefined: an auxiliary state "
                  "variable with the given glossary name already has already "
                  "been registred");
            }
            return true;
          }
        }
        return false;
      }();
      if (!b) {
        auto v = mfront::VariableDescription(t, n, s, 0u);
        v.setGlossaryName(g);
        bd.addAuxiliaryStateVariable(uh, v, mfront::BehaviourData::UNREGISTRED);
      }
    }  // end of addAuxiliaryStateVariableIfNotDefined

    void addAuxiliaryStateVariableIfNotDefined(BehaviourDescription& bd,
                                      const std::string& t,
                                      const std::string& n,
                                      const std::string& e,
                                      const unsigned short s,
                                      const bool bo) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto b = [bd, &n, &e, &bo] {
        for (const auto& v : bd.getBehaviourData(uh).getAuxiliaryStateVariables()) {
          if (v.getExternalName() == e) {
            if ((!bo) && (v.name != n)) {
              tfel::raise(
                  "addAuxiliaryStateVariableIfNotDefined: an auxiliary state "
                  "variable with the given entry name already has already been "
                  "registred");
            }
            return true;
          }
        }
        return false;
      }();
      if (!b) {
        auto v = mfront::VariableDescription(t, n, s, 0u);
        v.setEntryName(e);
        bd.addAuxiliaryStateVariable(uh, v, mfront::BehaviourData::UNREGISTRED);
      }
    }

    void addExternalStateVariable(BehaviourDescription& bd,
                                  const std::string& t,
                                  const std::string& n,
                                  const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v(t, n, s, 0u);
      bd.addExternalStateVariable(h, v);
    }  // end of addExternalStateVariable

    void addExternalStateVariable(BehaviourDescription& bd,
                                  const std::string& t,
                                  const std::string& n,
                                  const tfel::glossary::GlossaryEntry& g,
                                  const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v(t, n, s, 0u);
      bd.addExternalStateVariable(h, v);
      bd.setGlossaryName(h, n, g);
    }  // end of addExternalStateVariable

    void addExternalStateVariable(BehaviourDescription& bd,
                                  const std::string& t,
                                  const std::string& n,
                                  const std::string& e,
                                  const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v(t, n, s, 0u);
      bd.addExternalStateVariable(h, v);
      bd.setEntryName(h, n, e);
    }  // end of addExternalStateVariable

    void addLocalVariable(BehaviourDescription& bd,
                          const std::string& t,
                          const std::string& n,
                          const unsigned short s) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v(t, n, s, 0u);
      bd.addLocalVariable(h, v);
    }  // end of addLocalVariable

    void addParameter(BehaviourDescription& bd,
                      const std::string& n,
                      const tfel::glossary::GlossaryEntry& g,
                      const double p) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v("real", n, 1u, 0u);
      bd.addParameter(h, v);
      bd.setGlossaryName(h, n, g);
      bd.setParameterDefaultValue(h, n, p);
    }  // end of addParameter

    void addParameter(BehaviourDescription& bd,
                      const std::string& n,
                      const tfel::glossary::GlossaryEntry& g,
                      const unsigned short s,
                      const double p) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v("real", n, s, 0u);
      bd.addParameter(h, v);
      bd.setGlossaryName(h, n, g);
      for (unsigned short i = 0; i != s; ++i) {
        bd.setParameterDefaultValue(h, n, i, p);
      }
    }  // end of addParameter

    void addParameter(BehaviourDescription& bd,
                      const std::string& n,
                      const tfel::glossary::GlossaryEntry& g,
                      const unsigned short s,
                      const std::vector<double>& p) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v("real", n, 1u, 0u);
      bd.addParameter(h, v);
      bd.setGlossaryName(h, n, g);
      for (unsigned short i = 0; i != s; ++i) {
        bd.setParameterDefaultValue(h, n, i, p[i]);
      }
    }  // end of addParameter

    void addParameter(BehaviourDescription& bd,
                      const std::string& n,
                      const std::string& e,
                      const double p) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v("real", n, 1u, 0u);
      bd.addParameter(h, v);
      bd.setEntryName(h, n, e);
      bd.setParameterDefaultValue(h, n, p);
    }  // end of addParameter

    void addParameter(BehaviourDescription& bd,
                      const std::string& n,
                      const std::string& e,
                      const unsigned short s,
                      const double p) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      VariableDescription v("real", n, s, 0u);
      bd.addParameter(h, v);
      bd.setEntryName(h, n, e);
      for (unsigned short i = 0; i != s; ++i) {
        bd.setParameterDefaultValue(h, n, i, p);
      }
    }  // end of addParameter

    void addParameter(BehaviourDescription& bd,
                      const std::string& n,
                      const std::string& e,
                      const unsigned short s,
                      const std::vector<double>& p) {
      using tfel::material::ModellingHypothesis;
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      tfel::raise_if(p.size() != s,
                     "addParameter: "
                     "invalid number of default parameters values");
      VariableDescription v("real", n, s, 0u);
      bd.addParameter(h, v);
      bd.setEntryName(h, n, e);
      for (unsigned short i = 0; i != s; ++i) {
        bd.setParameterDefaultValue(h, n, i, p[i]);
      }
    }  // end of addParameter

    std::string computeElasticLimitInitialValue(
        const std::vector<std::shared_ptr<IsotropicHardeningRule>>& ihrs,
        const std::string& fid) {
      tfel::raise_if(ihrs.empty(),
                     "computeElasticLimitInitialValue: "
                     "empty list of isotropic hardenings");
      if (ihrs.size() == 1) {
        return ihrs[0]->computeElasticPrediction(fid,"");
      }
      auto c = std::string{};
      auto R = std::string{};
      for (decltype(ihrs.size()) i = 0; i != ihrs.size();) {
        const auto id = std::to_string(i);
        c += ihrs[i]->computeElasticPrediction(fid, id);
        R += "Rel" + fid + "_" + id;
        if (++i != ihrs.size()) {
          R += " + ";
        }
      }
      c += "const auto Rel" + fid + " = " + R + ";\n";
      return c;
    }  // end of computeElasticLimitInitialValue

    std::string computeElasticLimit(
        const std::vector<std::shared_ptr<IsotropicHardeningRule>>& ihrs,
        const std::string& fid) {
      tfel::raise_if(ihrs.empty(),
                     "computeElastic: "
                     "empty list of isotropic hardenings");
      if (ihrs.size() == 1) {
        return ihrs[0]->computeElasticLimit(fid, "");
      }
      auto c = std::string{};
      auto R = std::string{};
      for (decltype(ihrs.size()) i = 0; i != ihrs.size();) {
        const auto id = std::to_string(i);
        c += ihrs[i]->computeElasticLimit(fid, id);
        R += "R" + fid + "_" + id;
        if (++i != ihrs.size()) {
          R += " + ";
        }
      }
      c += "const auto R" + fid + " = " + R + ";\n";
      return c;
    }  // end of computeElasticLimit

    std::string computeElasticLimitAndDerivative(
        const std::vector<std::shared_ptr<IsotropicHardeningRule>>& ihrs,
        const std::string& fid) {
      tfel::raise_if(ihrs.empty(),
                     "computeElasticAndDerivative: "
                     "empty list of isotropic hardenings");
      if (ihrs.size() == 1) {
        return ihrs[0]->computeElasticLimitAndDerivative(fid,"");
      }
     auto c = std::string{};
     auto R = std::string{};
     auto dR = std::string{};
     for (decltype(ihrs.size()) i = 0; i != ihrs.size();) {
       const auto id = std::to_string(i);
       c += ihrs[i]->computeElasticLimitAndDerivative(fid, id);
       const auto Ri = "R" + fid + "_" + id;
       const auto dRi = "d" + Ri + "_ddp" + fid;
       R += Ri;
       dR += dRi;
       if (++i != ihrs.size()) {
         R += " + ";
         dR += " + ";
       }
     }
     c += "const auto R" + fid + " = " + R + ";\n";
     c += "const auto dR" + fid + "_ddp" + fid + " = " + dR + ";\n";
     return c;
    }  // end of computeElasticLimitAndDerivative

  }  // end of namespace bbrick

}  // end of namespace mfront
