/*!
 * \file   CyranoSymbolsGenerator.cxx
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
#include "MFront/CyranoSymbolsGenerator.hxx"

namespace mfront {

  CyranoSymbolsGenerator::CyranoSymbolsGenerator() = default;

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
