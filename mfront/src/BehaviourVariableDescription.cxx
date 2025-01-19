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

  static VariableDescriptionContainer selectSharedVariables(
      const VariableDescriptionContainer& variables,
      const std::vector<std::regex>& filters) {
    auto r = VariableDescriptionContainer{};
    for (const auto& v : variables) {
      const auto shared = [&v, &filters] {
        for (const auto& f : filters) {
          if ((std::regex_search(v.name, f)) ||
              (std::regex_search(v.getExternalName(), f))) {
            return true;
          }
        }
        return false;
      }();
      if (shared) {
        r.push_back(v);
      }
    }
    return r;
  }  // end of selectSharedVariables

  static VariableDescriptionContainer selectUnSharedVariables(
      const VariableDescriptionContainer& variables,
      const std::vector<std::regex>& filters) {
    auto r = VariableDescriptionContainer{};
    for (const auto& v : variables) {
      const auto shared = [&v, &filters] {
        for (const auto& f : filters) {
          if ((std::regex_search(v.name, f)) ||
              (std::regex_search(v.getExternalName(), f))) {
            return true;
          }
        }
        return false;
      }();
      if (!shared) {
        r.push_back(v);
      }
    }
    return r;
  }  // end of selectUnSharedVariables

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
    return selectSharedVariables(bd.getMaterialProperties(),
                                 d.shared_material_properties);
  }  // end of getSharedMaterialProperties

  VariableDescriptionContainer getUnSharedMaterialProperties(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectUnSharedVariables(bd.getMaterialProperties(),
                                   d.shared_material_properties);
  }  // end of getUnSharedMaterialProperties

  VariableDescriptionContainer getSharedExternalStateVariables(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectSharedVariables(bd.getExternalStateVariables(),
                                 d.shared_external_state_variables);
  }  // end of getSharedExternalStateVariables

  VariableDescriptionContainer getUnSharedExternalStateVariables(
      const BehaviourVariableDescription& d,
      const BehaviourVariableDescription::Hypothesis& h) {
    const auto& bd = d.behaviour.getBehaviourData(h);
    return selectUnSharedVariables(bd.getExternalStateVariables(),
                                   d.shared_external_state_variables);
  }  // end of getUnSharedExternalStateVariables

  std::string getBehaviourWrapperClassName(
      const BehaviourVariableDescription& d) {
    return d.behaviour.getClassName() + "Wrapper_" + d.name;
  }  // end of getBehaviourWrapperClassName

}  // end of namespace mfront
