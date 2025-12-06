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
      const std::vector<std::regex>& first_filters,
      const std::vector<std::regex>& second_filters) {
    auto r = VariableDescriptionContainer{};
    for (const auto& v : variables) {
      auto apply_filters = [&v](const std::vector<std::regex>& filters) {
        for (const auto& f : filters) {
          if ((std::regex_match(v.name, f)) ||
              (std::regex_match(v.getExternalName(), f))) {
            return true;
          }
          if (!v.symbolic_form.empty()) {
            if (std::regex_match(v.symbolic_form, f)) {
              return true;
            }
          }
        }
        return false;
      };
      const auto selected = apply_filters(first_filters);
      const auto selected2 = apply_filters(second_filters);
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
      auto apply_filters = [&v](const std::vector<std::regex>& filters) {
        for (const auto& f : filters) {
          if ((std::regex_match(v.name, f)) ||
              (std::regex_match(v.getExternalName(), f))) {
            return true;
          }
          if (!v.symbolic_form.empty()) {
            if (std::regex_match(v.symbolic_form, f)) {
              return true;
            }
          }
        }
        return false;
      };
      const auto shared = apply_filters(shared_filters);
      const auto evaluated = apply_filters(evaluated_filters);
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
    return selectVariablesFromRegularExpression(
        bd.getMaterialProperties(), d.shared_material_properties,
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

  void checkSharedVariableCompatibility(const BehaviourVariableDescription& bv,
                                        const VariableDescription& v1,
                                        const VariableDescription& v2) {
    auto report = [&bv, &v1, &v2](const std::string_view& reason) {
      tfel::raise("The shared variable '" + v1.name +
                  "' declared by behaviour variable '" + bv.name +
                  "' is not compatible with the variable declared by the "
                  "calling behaviour: " +
                  std::string{reason});
    };
    if (v1.getVariableTypeIdentifier() != v2.getVariableTypeIdentifier()) {
      report("unmatched type ('" + v1.type + "' vs '" + v2.type + "')");
    }
    if (v1.arraySize != v2.arraySize) {
      report("unmatched array size (" + std::to_string(v1.arraySize) + " vs " +
             std::to_string(v2.arraySize) + ")");
    }
    if ((v1.hasGlossaryName()) && (v2.hasGlossaryName())) {
      if (v1.getExternalName() != v2.getExternalName()) {
        report("unmatched external names ('" + v1.getExternalName() + "' vs '" +
               v2.getExternalName() + "')");
      }
    }
  }  // end checkSharedVariableCompatibility

}  // end of namespace mfront
