/*!
 * \file  mtest/src/GenericEvolution.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 6/05/2022
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
#include "MTest/GenericEvolution.hxx"

namespace mtest {

  GenericEvolution::GenericEvolution(const std::string& l,
                                     const std::string& fn,
                                     const EvolutionManager& evm_)
      : evm(evm_) {
    using namespace tfel::system;
    using ELM = tfel::system::ExternalLibraryManager;
    auto& elm = ELM::getExternalLibraryManager();
    this->f = elm.getGenericMaterialProperty(l, fn);
    if (elm.getGenericMaterialPropertyNumberOfVariables(l, fn) != 0u) {
      elm.getGenericMaterialPropertyVariables(this->vnames, l, fn);
      this->args.resize(this->vnames.size());
    }
  }  // end of GenericEvolution::GenericEvolution

  real GenericEvolution::operator()(const real t) const {
    for (std::vector<std::string>::size_type i = 0; i != this->vnames.size();
         ++i) {
      auto pev = this->evm.find(vnames[i]);
      if (pev == this->evm.end()) {
        tfel::raise(
            "GenericEvolution::operator(): "
            "can't evaluate argument '" +
            vnames[i] + "'");
      }
      const auto& ev = *(pev->second);
      args[i] = ev(t);
    }
    auto s = mfront_gmp_OutputStatus{};
    const auto r =
        this->f(&s, &args[0], static_cast<mfront_gmp_size_type>(args.size()),
                GENERIC_MATERIALPROPERTY_NONE_POLICY);
    if (s.status != 0) {
      tfel::raise("GenericEvolution::operator(): " + std::string(s.msg));
    }
    return r;
  }  // end of GenericEvolution::operator()

  bool GenericEvolution::isConstant() const {
    std::vector<std::string>::size_type i;
    for (i = 0; i != this->vnames.size(); ++i) {
      auto pev = this->evm.find(vnames[i]);
      tfel::raise_if(pev == this->evm.end(),
                     "GenericEvolution::operator(): "
                     "can't evaluate argument '" +
                         vnames[i] + "'");
      const auto& ev = *(pev->second);
      if (!ev.isConstant()) {
        return false;
      }
    }
    return true;
  }  // end of GenericEvolution::isConstant

  void GenericEvolution::setValue(const real) {
    tfel::raise(
        "GenericEvolution::setValue : "
        "this method does not makes "
        "sense for evolution evolution");
  }

  void GenericEvolution::setValue(const real, const real) {
    tfel::raise(
        "GenericEvolution::setValue : "
        "this method does not makes "
        "sense for evolution evolution");
  }

  GenericEvolution::~GenericEvolution() = default;

}  // end of namespace mtest
