/*!
 * \file   CastemSymbolsGenerator.cxx
 * \brief
 * \author th202608
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
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StandardBehaviourInterface.hxx"
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
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const FileDescription&,
      const std::string& name) const {
    if (CastemInterface::usesGenericPlaneStressAlgorithm(mb)) {
      out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
          << "_UsesGenericPlaneStressAlgorithm = 1u;\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
          << "_UsesGenericPlaneStressAlgorithm = 0u;\n\n";
    }
  }

  bool CastemSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of CastemSymbolsGenerator::handleStrainMeasure

  CastemSymbolsGenerator::~CastemSymbolsGenerator() = default;

}  // end of namespace mfront
