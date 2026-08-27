/*!
 * \file   mtest/src/UserDefinedPostProcessing.cxx
 * \brief
 * \author Thomas Helfer
 * \date 27/02/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/UserDefinedPostProcessing.hxx"

namespace mtest {

  UserDefinedPostProcessing::UserDefinedPostProcessing(
      const Behaviour& b,
      const EvolutionManager& e,
      const std::string& f,
      const std::vector<std::string>& ps)
      : evm(e), out(f) {
    if (!this->out) {
      tfel::raise(
          "UserDefinedPostProcessing::UserDefinedPostProcessing: "
          "can't open file '" +
          f + "'");
    }
    auto cevs = buildExternalFunctionManagerFromConstantEvolutions(this->evm);
    for (const auto& p : ps) {
      auto eval = std::make_shared<tfel::math::Evaluator>(p, cevs);
      const auto& vns = eval->getVariablesNames();
      for (const auto& vn : vns) {
        if (isBehaviourVariable(b, vn)) {
          if (this->extractors.count(vn) == 0) {
            this->extractors[vn] = buildValueExtractor(b, vn);
          }
        } else {
          if (this->evm.count(vn) == 0) {
            tfel::raise(
                "UserDefinedPostProcessing::UserDefinedPostProcessing: "
                "no variable named '" +
                vn + "' defined");
          }
        }
      }
      this->postprocessings.push_back(eval);
    }
    this->out << "# first column : time\n";
    auto cnbr = int{2};
    for (const auto& p : ps) {
      this->out << "# " << cnbr << " column : " << p << '\n';
      ++cnbr;
    }
  }  // end of UserDefinedPostProcessing::UserDefinedPostProcessing

  void UserDefinedPostProcessing::exe(const CurrentState& s,
                                      const real t,
                                      const real dt) {
    this->out << t + dt << " ";
    for (const auto& p : this->postprocessings) {
      const auto& vns = p->getVariablesNames();
      for (const auto& vn : vns) {
        auto pe = this->extractors.find(vn);
        if (pe != this->extractors.end()) {
          p->setVariableValue(vn, pe->second(s));
        } else {
          const auto pev = this->evm.find(vn);
          if (pev == this->evm.end()) {
            tfel::raise("UserDefinedPostProcessing::exe: unknown variable '" +
                        vn + "'");
          }
          const auto& ev = *(pev->second);
          p->setVariableValue(vn, ev(t + dt));
        }
      }
      this->out << " " << p->getValue();
    }
    this->out << std::endl;
  }  // end of UserDefinedPostProcessing::exe

  UserDefinedPostProcessing::~UserDefinedPostProcessing() = default;

}  // end of namespace mtest
