/*!
 * \file   mtest/src/CastemUmatFiniteStrainBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/CastemUmatFiniteStrainBehaviour.hxx"

namespace mtest {

  std::shared_ptr<Behaviour>
  CastemUmatFiniteStrainBehaviour::buildCastemUmatFiniteStrainBehaviour(
      const std::string& l,
      const std::string& f,
      const Parameters& p,
      const Hypothesis h) {
    return CastemUmatStandardBehaviour::buildCastemUmatStandardBehaviour(
        l, f, p, 2u, h);
  }  // end of
     // CastemUmatFiniteStrainBehaviour::buildCastemUmatFiniteStrainBehaviour

  CastemUmatFiniteStrainBehaviour::CastemUmatFiniteStrainBehaviour(
      const StandardBehaviourDescription& bd, const std::string& cn)
      : CastemFiniteStrainBehaviour(bd) {
    tfel::raise_if(
        cn.size() != 16u,
        "CastemUmatFiniteStrainBehaviour::CastemUmatFiniteStrainBehaviour: "
        "invalid size for the material name");
    std::copy(cn.begin(), cn.end(), this->mname);
  }  // end of CastemUmatFiniteStrainBehaviour::CastemUmatFiniteStrainBehaviour

  void
  CastemUmatFiniteStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    CastemUmatStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(
        mp, evm, this->stype, this->getHypothesis());
  }  // end of
     // CastemUmatFiniteStrainBehaviour::setOptionalMaterialPropertiesDefaultValues

  const char*
  CastemUmatFiniteStrainBehaviour::getBehaviourNameForUMATFunctionCall() const {
    return this->mname;
  }  // end of
     // CastemUmatFiniteStrainBehaviour::getBehaviourNameForUMATFunctionCal

  CastemUmatFiniteStrainBehaviour::~CastemUmatFiniteStrainBehaviour() = default;

}  // end of namespace mtest
