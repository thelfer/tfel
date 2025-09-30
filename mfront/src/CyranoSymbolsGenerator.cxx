/*!
 * \file   CyranoSymbolsGenerator.cxx
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
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourInterfaceBase.hxx"
#include "MFront/CyranoMaterialPropertyInterface.hxx"
#include "MFront/CyranoSymbolsGenerator.hxx"

namespace mfront {

  CyranoSymbolsGenerator::CyranoSymbolsGenerator() = default;

  void CyranoSymbolsGenerator::writeSpecificSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const FileDescription&,
      const std::string& name) const {
    auto emps = [&bd] {
      auto names = std::vector<std::string>{};
      if (bd.areElasticMaterialPropertiesDefined()) {
        for (const auto& e : bd.getElasticMaterialPropertiesDescriptions()) {
          CyranoMaterialPropertyInterface imp;
          names.push_back(imp.getFunctionName(e));
        }
      }
      return names;
    }();
    const auto fn = i.getFunctionNameBasis(name);
    exportUnsignedShortSymbol(os, fn + "_nElasticMaterialPropertiesEntryPoints",
                              emps.size());
    this->writeArrayOfStringsSymbol(
        os, fn + "_ElasticMaterialPropertiesEntryPoints", emps);
    auto themps = [&bd] {
      auto names = std::vector<std::string>{};
      if (bd.areThermalExpansionCoefficientsDefined()) {
        for (const auto& e : bd.getThermalExpansionCoefficientsDescriptions()) {
          CyranoMaterialPropertyInterface imp;
          names.push_back(imp.getFunctionName(e));
        }
      }
      return names;
    }();
    exportUnsignedShortSymbol(
        os, fn + "_nLinearThermalExpansionCoefficientsEntryPoints",
        themps.size());
    this->writeArrayOfStringsSymbol(
        os, fn + "_LinearThermalExpansionCoefficientsEntryPoints", themps);
  }  // end of CyranoSymbolsGenerator::writeSpecificSymbols

  void CyranoSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of CyranoSymbolsGenerator::writeAdditionalSymbols

  void CyranoSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    const auto bt = [&bd] {
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          if (bd.getStrainMeasure() == BehaviourDescription::LINEARISED) {
            return 1u;
          } else if (bd.getStrainMeasure() == BehaviourDescription::HENCKY) {
            return 2u;
          } else {
            tfel::raise(
                "CyranoSymbolsGenerator::writeBehaviourTypeSymbols: "
                "the cyrano interface only supports:\n"
                "- small strain behaviours: the only strain measure "
                "supported is the HPP one (linearised)\n"
                "- finite strain behaviours based on the Hencky strain "
                "measure");
          }
        } else {
          return 1u;
        }
      } else {
        tfel::raise(
            "CyranoSymbolsGenerator::writeBehaviourTypeSymbols: "
            "unsupported behaviour type");
      }
    }();
    exportUnsignedShortSymbol(
        os, i.getFunctionNameBasis(name) + "_BehaviourType", bt);
  }  // end of CyranoSymbolsGenerator::writeBehaviourTypeSymbols

  void CyranoSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    const auto bk = [&bd] {
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          if (bd.getStrainMeasure() == BehaviourDescription::LINEARISED) {
            return 1u;
          } else if (bd.getStrainMeasure() == BehaviourDescription::HENCKY) {
            return 4u;
          } else {
            tfel::raise(
                "CyranoSymbolsGenerator::writeBehaviourKinematicSymbols: "
                "the cyrano interface only supports:\n"
                "- small strain behaviours: the only strain measure "
                "supported is the HPP one (linearised)\n"
                "- finite strain behaviours based on the Hencky strain "
                "measure");
          }
        } else {
          return 1u;
        }
      } else {
        tfel::raise(
            "CyranoSymbolsGenerator::writeBehaviourKinematicSymbols: "
            "unsupported behaviour type");
      }
    }();
    exportUnsignedShortSymbol(
        os, i.getFunctionNameBasis(name) + "_BehaviourKinematic", bk);
  }  // end of CyranoSymbolsGenerator::writeBehaviourKinematicSymbols

  bool CyranoSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of CyranoSymbolsGenerator::handleStrainMeasure

  CyranoSymbolsGenerator::~CyranoSymbolsGenerator() = default;

}  // end of namespace mfront
