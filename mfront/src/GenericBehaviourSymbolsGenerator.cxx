/*!
 * \file   GenericBehaviourSymbolsGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourInterfaceBase.hxx"
#include "MFront/GenericBehaviourInterface.hxx"
#include "MFront/GenericBehaviourSymbolsGenerator.hxx"

namespace mfront {

  GenericBehaviourSymbolsGenerator::GenericBehaviourSymbolsGenerator() =
      default;

  void GenericBehaviourSymbolsGenerator::writeSpecificSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string& name) const {
    const auto n = i.getFunctionNameBasis(name);
    const auto s = n + "_api_version";
    const auto api = GenericBehaviourInterface::getAPIVersion();
    exportUnsignedShortSymbol(os, s, api);
  }  // end of writeSpecificSymbols

  void GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourType";
    if (bd.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR) {
      exportUnsignedShortSymbol(os, s, 0u);
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (bd.isStrainMeasureDefined()) {
        const auto fs = bd.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          exportUnsignedShortSymbol(os, s, 1u);
        } else if ((fs == BehaviourDescription::GREENLAGRANGE) ||
                   (fs == BehaviourDescription::HENCKY)) {
          exportUnsignedShortSymbol(os, s, 2u);
        } else {
          tfel::raise(
              "GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols: "
              "unsupported strain measure");
        }
      } else {
        exportUnsignedShortSymbol(os, s, 1u);
      }
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(os, s, 2u);
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      exportUnsignedShortSymbol(os, s, 3u);
    } else {
      tfel::raise(
          "GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols: "
          "unsupported behaviour type");
    }
  }  // end of GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols

  void GenericBehaviourSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourKinematic";
    if (bd.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR) {
      exportUnsignedShortSymbol(os, s, 0u);
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (bd.isStrainMeasureDefined()) {
        const auto fs = bd.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          exportUnsignedShortSymbol(os, s, 1u);
        } else if (fs == BehaviourDescription::GREENLAGRANGE) {
          exportUnsignedShortSymbol(os, s, 3u);
        } else if (fs == BehaviourDescription::HENCKY) {
          exportUnsignedShortSymbol(os, s, 3u);
        } else {
          tfel::raise(
              "GenericBehaviourSymbolsGenerator::"
              "writeBehaviourKinematicSymbols: "
              "unsupported strain measure");
        }
      } else {
        exportUnsignedShortSymbol(os, s, 1u);
      }
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(os, s, 3u);
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      exportUnsignedShortSymbol(os, s, 2u);
    } else {
      tfel::raise(
          "GenericBehaviourSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "unsupported behaviour type");
    }
  }  // end of
     // GenericBehaviourSymbolsGenerator::writeBehaviourKinematicSymbols

  void GenericBehaviourSymbolsGenerator::writeTangentOperatorSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    auto write_impl = [this, &os, &i, &name] {
      const auto fn = i.getFunctionNameBasis(name);
      const auto s = fn + "_nTangentOperatorBlocks";
      exportUnsignedShortSymbol(os, s, 2u);
      this->writeArrayOfStringsSymbol(os, fn + "_TangentOperatorBlocks",
                                      {"Stress", "DeformationGradient"});
    };
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if ((bd.isStrainMeasureDefined()) &&
          (bd.getStrainMeasure() != BehaviourDescription::LINEARISED)) {
        if (this->handleStrainMeasure()) {
          write_impl();
        } else {
          SymbolsGenerator::writeTangentOperatorSymbols(os, i, bd, name);
        }
      } else {
        SymbolsGenerator::writeTangentOperatorSymbols(os, i, bd, name);
      }
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      write_impl();
    } else {
      SymbolsGenerator::writeTangentOperatorSymbols(os, i, bd, name);
    }
  }  // end of GenericBehaviourSymbolsGenerator::writeTangentOperatorSymbols

  void GenericBehaviourSymbolsGenerator::writeInitializeFunctionsSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = bd.getBehaviourData(h);
    const auto fn = this->getSymbolName(i, name, h);
    auto initializes = std::vector<std::string>{};
    for (const auto& ifct : d.getInitializeFunctions()) {
      initializes.push_back(ifct.first);
    }
    exportUnsignedShortSymbol(os, fn + "_nInitializeFunctions",
                              initializes.size());
    this->writeArrayOfStringsSymbol(os, fn + "_InitializeFunctions",
                                    initializes);
    for (const auto& [p, c] : d.getInitializeFunctions()) {
      const auto s = "InitializeFunction_" + p + "_Inputs";
      const auto& initialize_function_variables =
          c.attributes.at(CodeBlock::used_initialize_function_variables)
              .get<std::vector<VariableDescription>>();
      auto variables = VariableDescriptionContainer{};
      variables.insert(variables.end(), initialize_function_variables.begin(),
                       initialize_function_variables.end());
      exportUnsignedShortSymbol(os, this->getSymbolName(i, name, h) + "_n" + s,
                                variables.getNumberOfVariables());
      this->writeExternalNames(os, i, name, h, variables.getExternalNames(), s);
      this->writeVariablesTypesSymbol(os, i, name, h, variables, s);
    }
    os << "\n";
  }  // end of writeInitializeFunctionsSymbols

  void GenericBehaviourSymbolsGenerator::writePostProcessingsSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = bd.getBehaviourData(h);
    const auto fn = this->getSymbolName(i, name, h);
    auto postprocessings = std::vector<std::string>{};
    for (const auto& p : d.getPostProcessings()) {
      postprocessings.push_back(p.first);
    }
    exportUnsignedShortSymbol(os, fn + "_nPostProcessings",
                              postprocessings.size());
    this->writeArrayOfStringsSymbol(os, fn + "_PostProcessings",
                                    postprocessings);
    for (const auto& [p, c] : d.getPostProcessings()) {
      const auto s = "PostProcessing_" + p + "_Outputs";
      const auto& postprocessing_variables =
          c.attributes.at(CodeBlock::used_postprocessing_variables)
              .get<std::vector<VariableDescription>>();
      auto variables = VariableDescriptionContainer{};
      variables.insert(variables.end(), postprocessing_variables.begin(),
                       postprocessing_variables.end());
      exportUnsignedShortSymbol(os, this->getSymbolName(i, name, h) + "_n" + s,
                                variables.getNumberOfVariables());
      this->writeExternalNames(os, i, name, h, variables.getExternalNames(), s);
      this->writeVariablesTypesSymbol(os, i, name, h, variables, s);
    }
    os << "\n";
  }  // end of writePostProcessingsSymbols

  void GenericBehaviourSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of GenericBehaviourSymbolsGenerator::writeAdditionalSymbols

  bool GenericBehaviourSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of GenericBehaviourSymbolsGenerator::handleStrainMeasure

  GenericBehaviourSymbolsGenerator::~GenericBehaviourSymbolsGenerator() =
      default;

}  // end of namespace mfront
