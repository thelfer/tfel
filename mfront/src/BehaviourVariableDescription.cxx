/*!
 * \file   mfront/src/BehaviourVariableDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06/01/2025
 */

#include "TFEL/Raise.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "MFront/DSLBase.hxx"
#include "MFront/BehaviourVariableDescription.hxx"

namespace mfront {

  static std::string addPrefixAndSuffix(const std::string& n,
                                        const std::string& prefix,
                                        const std::string& suffix) {
    return prefix + n + suffix;
  }  // end of addPrefixAndSuffix

  static VariableDescriptionContainer selectVariablesFromRegularExpression(
      const VariableDescriptionContainer& variables,
      const std::vector<std::regex>& filters,
      const std::vector<std::regex>& second_filters) {
    auto r = VariableDescriptionContainer{};
    for (const auto& v : variables) {
      const auto selected = [&v, &filters] {
        for (const auto& f : filters) {
          if ((std::regex_search(v.name, f)) ||
              (std::regex_search(v.getExternalName(), f))) {
            return true;
          }
        }
        return false;
      }();
      const auto selected2 = [&v, &second_filters] {
        for (const auto& f : second_filters) {
          if ((std::regex_search(v.name, f)) ||
              (std::regex_search(v.getExternalName(), f))) {
            return true;
          }
        }
        return false;
      }();
      if (selected && selected2) {
        tfel::raise("variable '" + v.getExternalName() + "' ('" + v.name +
                    "') is both shared and evaluated");
      }
      if (selected) {
        r.push_back(v);
      }
    }
    return r;
  }  // end of selectVariablesFromRegularExpression

  static VariableDescriptionContainer selectUnSharedNorEvaluatedVariables(
      const VariableDescriptionContainer& variables,
      const std::vector<std::regex>& shared_filters,
      const std::vector<std::regex>& evaluated_filters) {
    auto r = VariableDescriptionContainer{};
    for (const auto& v : variables) {
      const auto shared = [&v, &shared_filters] {
        for (const auto& f : shared_filters) {
          if ((std::regex_search(v.name, f)) ||
              (std::regex_search(v.getExternalName(), f))) {
            return true;
          }
        }
        return false;
      }();
      const auto evaluated = [&v, &evaluated_filters] {
        for (const auto& f : evaluated_filters) {
          if ((std::regex_search(v.name, f)) ||
              (std::regex_search(v.getExternalName(), f))) {
            return true;
          }
        }
        return false;
      }();
      if (shared && evaluated) {
        tfel::raise("variable '" + v.getExternalName() + "' ('" + v.name +
                    "') is both shared and evaluated");
      }
      if (!(shared || evaluated)) {
        r.push_back(v);
      }
    }
    return r;
  }  // end of selectUnSharedNorEvaluatedVariables

  VariableDescription applyNamesChanges(const BehaviourVariableDescription& d,
                                        const VariableDescription& v) {
    auto nv = v;
    // applied variables_prefix, etc.
    nv.name = addPrefixAndSuffix(
        v.name, tfel::unicode::getMangledString(d.variables_prefix),
        tfel::unicode::getMangledString(d.variables_suffix));
    if (!isValidUserDefinedVariableName(nv.name)) {
      tfel::raise("applying behaviour prefix or suffix to variable name '" +
                  v.name + "' results in an invalid variable name '" + nv.name +
                  "'");
    }
    if (!v.symbolic_form.empty()) {
      nv.symbolic_form = addPrefixAndSuffix(v.symbolic_form, d.variables_prefix,
                                            d.variables_suffix);
    }
    if ((v.hasGlossaryName()) || (v.hasEntryName())) {
      const auto ename =
          addPrefixAndSuffix(v.getExternalName(), d.external_names_prefix,
                             d.external_names_suffix);
      const auto& g = tfel::glossary::Glossary::getGlossary();
      if (g.contains(ename)) {
        nv.resetGlossaryName(ename);
      } else {
        nv.resetEntryName(ename);
      }
    }
    return nv;
  }  // end of applyNamesChanges

  VariableDescriptionContainer getSharedMaterialProperties(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectVariablesFromRegularExpression(bd.getMaterialProperties(),
                                                d.shared_material_properties,
                                                d.evaluated_material_properties);
  }  // end of getSharedMaterialProperties

  VariableDescriptionContainer getEvaluatedMaterialProperties(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectVariablesFromRegularExpression(bd.getMaterialProperties(),
                                                d.evaluated_material_properties,
                                                d.shared_material_properties);
  }  // end of getEvaluatedMaterialProperties

  VariableDescriptionContainer getUnSharedNorEvaluatedMaterialProperties(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectUnSharedNorEvaluatedVariables(bd.getMaterialProperties(),
                                               d.shared_material_properties,
                                               d.evaluated_material_properties);
  }  // end of getUnSharedNorEvaluatedMaterialProperties

  VariableDescriptionContainer getSharedExternalStateVariables(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectVariablesFromRegularExpression(
        bd.getExternalStateVariables(), d.shared_external_state_variables,
        d.evaluated_external_state_variables);
  }  // end of getSharedExternalStateVariables

  VariableDescriptionContainer getEvaluatedExternalStateVariables(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectVariablesFromRegularExpression(
        bd.getExternalStateVariables(), d.evaluated_external_state_variables,
        d.shared_external_state_variables);
  }  // end of getEvaluatedExternalStateVariables

  VariableDescriptionContainer getUnSharedNorEvaluatedExternalStateVariables(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectUnSharedNorEvaluatedVariables(
        bd.getExternalStateVariables(), d.shared_external_state_variables,
        d.evaluated_external_state_variables);
  }  // end of getUnSharedNorEvaluatedExternalStateVariables

  std::string getBehaviourWrapperClassName(
      const BehaviourVariableDescription& d) {
    return "MFront" + d.behaviour.getClassName() + "Wrapper_" + d.name;
  }  // end of getBehaviourWrapperClassName

  std::string getBehaviourVariableFactoryClassName(
      const BehaviourVariableDescription& d) {
    return "MFront" + d.behaviour.getClassName() +  //
           "BehaviourVariableFactory_" + d.name;
  }  // end of getBehaviourWrapperClassName

}  // end of namespace mfront
