/*!
 * \file   mtest/src/CastemUmatSmallStrainBehaviour.cxx
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
#include "MTest/CastemUmatSmallStrainBehaviour.hxx"

namespace mtest {

  std::shared_ptr<Behaviour>
  CastemUmatSmallStrainBehaviour::buildCastemUmatSmallStrainBehaviour(
      const std::string& l,
      const std::string& f,
      const Parameters& p,
      const Hypothesis h) {
    return CastemUmatStandardBehaviour::buildCastemUmatStandardBehaviour(
        l, f, p, 1u, h);
  }  // end of buildCastemUmatSmallStrainBehaviour

  CastemUmatSmallStrainBehaviour::CastemUmatSmallStrainBehaviour(
      const StandardBehaviourDescription& bd, const std::string& cn)
      : CastemSmallStrainBehaviour(bd) {
    tfel::raise_if(
        cn.size() != 16u,
        "CastemUmatSmallStrainBehaviour::CastemUmatSmallStrainBehaviour: "
        "invalid size for the material name");
    std::copy(cn.begin(), cn.end(), this->mname);
  }  // end of CastemUmatSmallStrainBehaviour

  std::vector<std::string>
  CastemUmatSmallStrainBehaviour::getOptionalMaterialProperties() const {
    return CastemUmatStandardBehaviour::getOptionalMaterialProperties(
        this->stype, this->getHypothesis());
  }  // end of getOptionalMaterialProperties

  void
  CastemUmatSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    CastemUmatStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(
        mp, evm, this->stype, this->getHypothesis());
  }  // end of setOptionalMaterialPropertiesDefaultValues

  const char*
  CastemUmatSmallStrainBehaviour::getBehaviourNameForUMATFunctionCall() const {
    return this->mname;
  }  // end of getBehaviourNameForUMATFunctionCal

  CastemUmatSmallStrainBehaviour::~CastemUmatSmallStrainBehaviour() = default;

}  // end of namespace mtest
