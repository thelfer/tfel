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
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StandardBehaviourInterface.hxx"
#include "MFront/CyranoMaterialPropertyInterface.hxx"
#include "MFront/CyranoSymbolsGenerator.hxx"

namespace mfront {

  CyranoSymbolsGenerator::CyranoSymbolsGenerator() = default;

  void CyranoSymbolsGenerator::writeSpecificSymbols(
      std::ostream& os,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& bd,
      const FileDescription&,
      const std::string& name) const {
    auto emps = [&bd, &i, &name] {
      auto names = std::vector<std::string>{};
      if (bd.areElasticMaterialPropertiesDefined()) {
        for (const auto& e : bd.getElasticMaterialPropertiesDescriptions()) {
          CyranoMaterialPropertyInterface imp;
          names.push_back(imp.getCyranoFunctionName(e));
        }
      }
      return names;
    }();
    const auto fn = i.getFunctionNameBasis(name);
    os << "MFRONT_SHAREDOBJ unsigned short " << fn
       << "_nElasticMaterialPropertiesEntryPoints = " << emps.size() << "u;\n";
    this->writeArrayOfStringsSymbol(
        os, fn + "_ElasticMaterialPropertiesEntryPoints", emps);
    auto themps = [&bd, &i, &name] {
      auto names = std::vector<std::string>{};
      if (bd.areThermalExpansionCoefficientsDefined()) {
        for (const auto& e : bd.getThermalExpansionCoefficientsDescriptions()) {
          CyranoMaterialPropertyInterface imp;
          names.push_back(imp.getCyranoFunctionName(e));
        }
      }
      return names;
    }();
    os << "MFRONT_SHAREDOBJ unsigned short " << fn
       << "_nLinearThermalExpansionCoefficientsEntryPoints = "  //
       << themps.size() << "u;\n";
    this->writeArrayOfStringsSymbol(
        os, fn + "_LinearThermalExpansionCoefficientsEntryPoints", themps);
  }  // end of CyranoSymbolsGenerator::writeSpecificSymbols

  void CyranoSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of CyranoSymbolsGenerator::writeAdditionalSymbols

  void CyranoSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "CyranoSymbolsGenerator::writeBehaviourTypeSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (mb.isStrainMeasureDefined()) {
        if (mb.getStrainMeasure() == BehaviourDescription::LINEARISED) {
          out << "1u;\n\n";
        } else if (mb.getStrainMeasure() == BehaviourDescription::HENCKY) {
          out << "2u;\n\n";
        } else {
          throw_if(true,
                   "the cyrano interface only supports:\n"
                   "- small strain behaviours: the only strain measure "
                   "supported is the HPP one (linearised)\n"
                   "- finite strain behaviours based on the Hencky strain "
                   "measure");
        }
      } else {
        out << "1u;\n\n";
      }
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of CyranoSymbolsGenerator::writeBehaviourTypeSymbols

  void CyranoSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        tfel::raise("CyranoSymbolsGenerator::writeBehaviourKinematicSymbols: " +
                    m);
      }
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (mb.isStrainMeasureDefined()) {
        if (mb.getStrainMeasure() == BehaviourDescription::LINEARISED) {
          out << "1u;\n\n";
        } else if (mb.getStrainMeasure() == BehaviourDescription::HENCKY) {
          out << "4u;\n\n";
        } else {
          throw_if(
              true,
              "the cyrano interface only supports:\n"
              "- small strain behaviours: the only strain measure "
              "supported is the HPP one (linearised)\n"
              "- finite strain behaviours based on the Hencky strain measure");
        }
      } else {
        out << "1u;\n\n";
      }
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of CyranoSymbolsGenerator::writeBehaviourKinematicSymbols

  bool CyranoSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of CyranoSymbolsGenerator::handleStrainMeasure

  CyranoSymbolsGenerator::~CyranoSymbolsGenerator() = default;

}  // end of namespace mfront
