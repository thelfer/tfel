/*!
 * \file   BrickUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
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

    BehaviourDescription::MaterialProperty
    getBehaviourDescriptionMaterialProperty(AbstractBehaviourDSL& dsl,
                                            const std::string& n,
                                            const tfel::utilities::Data& d) {
      if (d.is<double>()) {
        BehaviourDescription::ConstantMaterialProperty cmp;
        cmp.value = d.get<double>();
        return cmp;
      } else if (d.is<int>()) {
        BehaviourDescription::ConstantMaterialProperty cmp;
        cmp.value = static_cast<double>(d.get<int>());
        return cmp;
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
        return emp;
      }
      BehaviourDescription::AnalyticMaterialProperty amp;
      amp.f = mp;
      return amp;
    }  // end of getBehaviourDescriptionMaterialProperty_impl

    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        BehaviourDescription::MaterialProperty& mp,
        const std::string& n) {
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
        auto& cmp = mp.get<BehaviourDescription::ConstantMaterialProperty>();
        cmp.name = n;
        // declare associated parameter
        VariableDescription m("real", n, 1u, 0u);
        bd.addParameter(h, m);
        bd.setParameterDefaultValue(h, n, cmp.value);
      } else {
        VariableDescription m("real", n, 1u, 0u);
        bd.addLocalVariable(h, m);
      }
    } // end of declareParameterOrLocalVariable

    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        BehaviourDescription::MaterialProperty& mp,
        const std::string& n,
        const std::string& en) {
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      declareParameterOrLocalVariable(bd, mp, n);
      if (mp.is<BehaviourDescription::ConstantMaterialProperty>()) {
        bd.setEntryName(h,n,en);
      }
    } // end of declareParameterOrLocalVariable

    void declareParameterOrLocalVariable(
        BehaviourDescription& bd,
        BehaviourDescription::MaterialProperty& mp,
        const std::string& n,
        const tfel::glossary::GlossaryEntry& g) {
      constexpr const auto h =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      declareParameterOrLocalVariable(bd, mp, n);
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

  }  // end of namespace bbrick

}  // end of namespace mfront
