/*!
 * \file  mfront/mtest/Behaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <stdexcept>
#include "MFront/MFrontLogStream.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"

namespace mtest {

  void Behaviour::setOptionalMaterialPropertyDefaultValue(
      EvolutionManager& mp,
      const EvolutionManager& evm,
      const std::string& n,
      const real v) {
    if (evm.find(n) == evm.end()) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
        auto& log = mfront::getLogStream();
        log << "Behaviour::setOptionalMaterialPropertiesDefaultValues : "
            << "set material property '" << n << "' to default value\n";
      }
      if (!mp.insert({n, make_evolution(v)}).second) {
        throw(std::runtime_error(
            "Behaviour::setOptionalMaterialPropertiesDefaultValues: "
            "default value for material property '" +
            n + "' already declared"));
      }
    }
  }  // end of Behaviour::setOptionalMaterialPropertyDefaultValue

  Behaviour::~Behaviour() = default;

  std::function<real(const CurrentState&)> buildValueExtractor(
      const Behaviour& b, const std::string& n) {
    const auto enames = b.getDrivingVariablesComponents();
    auto p = std::find(enames.begin(), enames.end(), n);
    if (p != enames.end()) {
      const auto pos = static_cast<unsigned short>(p - enames.begin());
      return [pos](const CurrentState& s) { return s.e1[pos]; };
    }
    const auto snames = b.getThermodynamicForcesComponents();
    p = std::find(snames.begin(), snames.end(), n);
    if (p != snames.end()) {
      const auto pos = static_cast<unsigned short>(p - snames.begin());
      return [pos](const CurrentState& s) { return s.s1[pos]; };
    }
    const auto isvnames = b.expandInternalStateVariablesNames();
    p = std::find(isvnames.begin(), isvnames.end(), n);
    if (p != isvnames.end()) {
      const auto pos = static_cast<unsigned short>(p - isvnames.begin());
      return [pos](const CurrentState& s) { return s.iv1[pos]; };
    }
    const auto esvnames = b.getExternalStateVariablesNames();
    p = std::find(esvnames.begin(), esvnames.end(), n);
    if (p != esvnames.end()) {
      const auto pos = static_cast<unsigned short>(p - esvnames.begin());
      return [pos](const CurrentState& s) { return s.esv0[pos] + s.desv[pos]; };
    }
    throw(std::runtime_error("buildValueExtractor: no variable name '" + n +
                             "'"));
  }  // end of buildValueExtractor

}  // end of namespace mtest
