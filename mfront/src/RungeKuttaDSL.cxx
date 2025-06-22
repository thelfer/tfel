/*!
 * \file  mfront/src/RungeKuttaDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/RungeKuttaDSL.hxx"

namespace mfront {

  RungeKuttaDSL::RungeKuttaDSL() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("RungeKutta");
    // input variables
    this->mb.declareAsASmallStrainStandardBehaviour();
    // Default state variable
    VariableDescription eel("StrainStensor", "eel", 1u, 0u);
    eel.description = "elastic strain";
    this->mb.addStateVariable(h, eel);
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    // driving variables
    for (const auto& v : this->mb.getMainVariables()) {
      const auto& dv = v.first;
      this->mb.addLocalVariable(
          h, VariableDescription(dv.type, dv.name + "_", 1u, 0u));
      this->mb.addLocalVariable(
          h, VariableDescription(SupportedTypes::getTimeDerivativeType(dv.type),
                                 "d" + dv.name + "_", 1u, 0u));
    }
  }  // end of RungeKuttaDSL::RungeKuttaDSL

  std::string RungeKuttaDSL::getName() { return "RungeKutta"; }

  std::string RungeKuttaDSL::getDescription() {
    return "this dsl provides a generic integrator based on one of the many "
           "Runge-Kutta algorithm. Avalailable algorithms are 'euler', 'rk2', "
           "'rk4' "
           "'r42', 'rk54' and 'rkCastem'";
  }  // end of RungeKuttaDSL::getDescription

  RungeKuttaDSL::~RungeKuttaDSL() = default;

}  // end of namespace mfront
