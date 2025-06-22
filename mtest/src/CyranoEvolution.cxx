/*!
 * \file  mtest/src/CyranoEvolution.cxx
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
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MTest/CyranoEvolution.hxx"

namespace mtest {

  CyranoEvolution::CyranoEvolution(const std::string& l,
                                   const std::string& fn,
                                   const EvolutionManager& evm_)
      : evm(evm_) {
    using namespace tfel::system;
    using ELM = tfel::system::ExternalLibraryManager;
    auto& elm = ELM::getExternalLibraryManager();
    this->f = elm.getCyranoMaterialProperty(l, fn);
    if (elm.getCyranoMaterialPropertyNumberOfVariables(l, fn) != 0u) {
      elm.getCyranoMaterialPropertyVariables(this->vnames, l, fn);
      this->args.resize(this->vnames.size());
    }
  }  // end of CyranoEvolution::CyranoEvolution

  real CyranoEvolution::operator()(const real t) const {
    using namespace std;
    vector<string>::size_type i;
    for (i = 0; i != this->vnames.size(); ++i) {
      auto pev = this->evm.find(vnames[i]);
      if (pev == this->evm.end()) {
        tfel::raise(
            "CyranoEvolution::operator(): "
            "can't evaluate argument '" +
            vnames[i] + "'");
      }
      const auto& ev = *(pev->second);
      args[i] = ev(t);
    }
    auto s = CyranoOutputStatus{};
    const auto r =
        this->f(&s, &args[0], static_cast<CyranoIntegerType>(args.size()),
                CYRANO_NONE_POLICY);
    if (s.status != 0) {
      tfel::raise("CyranoEvolution::operator(): " + std::string(s.msg));
    }
    return r;
  }  // end of CyranoEvolution::operator()

  bool CyranoEvolution::isConstant() const {
    std::vector<std::string>::size_type i;
    for (i = 0; i != this->vnames.size(); ++i) {
      auto pev = this->evm.find(vnames[i]);
      tfel::raise_if(pev == this->evm.end(),
                     "CyranoEvolution::operator(): "
                     "can't evaluate argument '" +
                         vnames[i] + "'");
      const auto& ev = *(pev->second);
      if (!ev.isConstant()) {
        return false;
      }
    }
    return true;
  }  // end of CyranoEvolution::isConstant

  void CyranoEvolution::setValue(const real) {
    tfel::raise(
        "CyranoEvolution::setValue : "
        "this method does not makes "
        "sense for evolution evolution");
  }

  void CyranoEvolution::setValue(const real, const real) {
    tfel::raise(
        "CyranoEvolution::setValue : "
        "this method does not makes "
        "sense for evolution evolution");
  }

  CyranoEvolution::~CyranoEvolution() = default;

}  // end of namespace mtest
