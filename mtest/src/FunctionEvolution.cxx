/*!
 * \file  mfront/mtest/FunctionEvolution.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 14 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MTest/FunctionEvolution.hxx"

namespace mtest {

  FunctionEvolution::FunctionEvolution(const std::string& f_,
                                       const EvolutionManager& evm_)
      : evm(evm_), f(f_) {}  // end of FunctionEvolution::FunctionEvolution

  real FunctionEvolution::operator()(const real t) const {
    const auto& args = this->f.getVariablesNames();
    std::vector<std::string>::size_type i;
    for (i = 0; i != args.size(); ++i) {
      if (args[i] == "t") {
        this->f.setVariableValue("t", t);
      } else {
        auto pev = evm.find(args[i]);
        tfel::raise_if(pev == evm.end(),
                       "FunctionEvolution::operator(): "
                       "can't evaluate argument '" +
                           args[i] + "'");
        const auto& ev = *(pev->second);
        this->f.setVariableValue(args[i], ev(t));
      }
    }
    return this->f.getValue();
  }  // end of FunctionEvolution::operator()

  bool FunctionEvolution::isConstant() const {
    const auto& args = this->f.getVariablesNames();
    std::vector<std::string>::size_type i;
    for (i = 0; i != args.size(); ++i) {
      if (args[i] == "t") {
        return false;
      } else {
        auto pev = evm.find(args[i]);
        tfel::raise_if(pev == evm.end(),
                       "FunctionEvolution::operator(): "
                       "can't evaluate argument '" +
                           args[i] + "'");
        const auto& ev = *(pev->second);
        if (!ev.isConstant()) {
          return false;
        }
      }
    }
    return true;
  }  // end of FunctionEvolution::isConstant

  void FunctionEvolution::setValue(const real) {
    tfel::raise(
        "FunctionEvolution::setValue: "
        "this method does not makes sense "
        "for function evolution");
  }

  void FunctionEvolution::setValue(const real, const real) {
    tfel::raise(
        "FunctionEvolution::setValue: "
        "this method does not makes sense "
        "for function evolution");
  }

  FunctionEvolution::~FunctionEvolution() = default;

}  // end of namespace mtest
