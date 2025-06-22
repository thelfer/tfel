/*!
 * \file   CastemSymbolsGenerator.cxx
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
#include "MFront/CastemMaterialPropertyInterface.hxx"
#include "MFront/CastemInterface.hxx"
#include "MFront/CastemSymbolsGenerator.hxx"

namespace mfront {

  CastemSymbolsGenerator::CastemSymbolsGenerator() = default;

  void CastemSymbolsGenerator::writeMainVariablesSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const std::string&) const {
  }  // end of CastemSymbolsGenerator::writeMainVariablesSymbols

  void CastemSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const std::string&) const {
  }  // end of CastemSymbolsGenerator::writeBehaviourTypeSymbols

  void CastemSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const std::string&) const {
  }  // end of CastemSymbolsGenerator::writeBehaviourKinematicSymbols

  void CastemSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of CastemSymbolsGenerator::writeAdditionalSymbols

  void CastemSymbolsGenerator::writeSpecificSymbols(
      std::ostream& os,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& bd,
      const FileDescription&,
      const std::string& name) const {
    if (CastemInterface::usesGenericPlaneStressAlgorithm(bd)) {
      os << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
         << "_UsesGenericPlaneStressAlgorithm = 1u;\n\n";
    } else {
      os << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
         << "_UsesGenericPlaneStressAlgorithm = 0u;\n\n";
    }
    const auto fn = i.getFunctionNameBasis(name);
    // elastic material properties
    auto emps = [&bd] {
      auto names = std::vector<std::string>{};
      if (bd.areElasticMaterialPropertiesDefined()) {
        for (const auto& e : bd.getElasticMaterialPropertiesDescriptions()) {
          CastemMaterialPropertyInterface imp;
          names.push_back(imp.getCastemFunctionName(e));
        }
      }
      return names;
    }();
    os << "MFRONT_SHAREDOBJ unsigned short " << fn
       << "_nElasticMaterialPropertiesEntryPoints = " << emps.size() << "u;\n";
    this->writeArrayOfStringsSymbol(
        os, fn + "_ElasticMaterialPropertiesEntryPoints", emps);
    // material properties associated with the thermal expansion coefficients
    auto themps = [&bd] {
      auto names = std::vector<std::string>{};
      if (bd.areThermalExpansionCoefficientsDefined()) {
        for (const auto& e : bd.getThermalExpansionCoefficientsDescriptions()) {
          CastemMaterialPropertyInterface imp;
          names.push_back(imp.getCastemFunctionName(e));
        }
      }
      return names;
    }();
    os << "MFRONT_SHAREDOBJ unsigned short " << fn
       << "_nLinearThermalExpansionCoefficientsEntryPoints = "  //
       << themps.size() << "u;\n";
    this->writeArrayOfStringsSymbol(
        os, fn + "_LinearThermalExpansionCoefficientsEntryPoints", themps);
  }  // end of CastemSymbolsGenerator::writeSpecificSymbols

  bool CastemSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of CastemSymbolsGenerator::handleStrainMeasure

  CastemSymbolsGenerator::~CastemSymbolsGenerator() = default;

}  // end of namespace mfront
